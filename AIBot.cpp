/*=====================================================================
AIBot.cpp
---------
Copyright Nicholas Chapman 2023 -
=====================================================================*/


#include "Weather.h"
#include "VolumeControl.h"
#include <ConPrint.h>
#include <Exception.h>
#include <Clock.h>
#include <FileUtils.h>
#include <StringUtils.h>
#include <JSONParser.h>
#include <PlatformUtils.h>
#include <Timer.h>
#include <Parser.h>
#include <ComObHandle.h>
#include <maths/mathstypes.h>
#include <webserver/Escaping.h>
#include <networking/Networking.h>
#include <networking/HTTPClient.h>
#include <whisper.cpp/whisper.h>
#include <SDL.h>
#include <sapi.h> // Speech API
#include <sphelper.h>


static inline void throwOnError(HRESULT hres)
{
	if(FAILED(hres))
		throw glare::Exception("Error: " + PlatformUtils::COMErrorString(hres));
}


static void audioCallback(
	void* userdata,
	Uint8* stream,
	int len
)
{
	std::vector<float>* audio_data_ptr = (std::vector<float>*)userdata;

	std::vector<float>& audio_data = *audio_data_ptr;

	const int num_samples = len / sizeof(float);
	const size_t write_i = audio_data.size();
	audio_data.resize(audio_data.size() + num_samples);

	const float* data = (const float*)stream;
	for(size_t i=0; i<num_samples; ++i)
	{
		audio_data[write_i + i] = data[i];
	}
}


static void check_result(HRESULT hr)
{
	if(FAILED(hr))
		throw glare::Exception("Call failed: " + PlatformUtils::getLastErrorString());
}


// Adapted from https://stackoverflow.com/questions/16547349/sapi-speech-to-text-example among other sources
const ULONGLONG grammarId = 0;
const wchar_t* ruleName1 = L"rule1";

/**
* Create and initialize the Grammar.
* Create a rule for the grammar.
* Add word to the grammar.
*/
ISpRecoGrammar* init_grammar(ISpRecoContext* recoContext, const std::string& command)
{
	HRESULT hr;
	SPSTATEHANDLE rule_state;

	ISpRecoGrammar* recoGrammar;
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	check_result(hr);

	// deactivate the grammar to prevent premature recognitions to an "under-construction" grammar
	hr = recoGrammar->SetGrammarState(SPGS_DISABLED);
	check_result(hr);

	//WORD langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	//WORD langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	hr = recoGrammar->ResetGrammar(GetUserDefaultUILanguage());
	check_result(hr);

	// Create rules
	hr = recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, /*fCreateIfNotExist=*/true, &rule_state);
	check_result(hr);

	// Add a word
	hr = recoGrammar->AddWordTransition(rule_state, 
		NULL, // hToState - set to null make this a transition to the terminal state.
		StringUtils::UTF8ToPlatformUnicodeEncoding(command).c_str(), // word
		L" ", // transition word separation characters
		SPWT_LEXICAL, 
		1.f, // weight
		NULL); // pPropInfo
	check_result(hr);

	// Commit changes
	hr = recoGrammar->Commit(0);
	check_result(hr);

	// activate the grammar since "construction" is finished,
	//    and ready for receiving recognitions  (see https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ms723630(v=vs.85))
	hr = recoGrammar->SetGrammarState(SPGS_ENABLED);
	check_result(hr);


	// activate the e-mail rule to begin receiving recognitions
	hr = recoGrammar->SetRuleState(ruleName1, NULL, SPRS_ACTIVE);
	check_result(hr);

	return recoGrammar;
}


struct ChatMessage
{
	std::string role;
	std::string content;
};


struct VoiceCommandContext
{
	SDL_AudioDeviceID audio_dev_id;
	const SDL_AudioSpec* obtained_spec;
	std::vector<float>* audio_data;
	struct whisper_context* whisper_ctx;
	std::string openai_api_key;
	ISpVoice* voice;
	std::string current_weather;

	//std::string query;
	std::string base_prompt;

	std::vector<ChatMessage> chat_messages;
};


void doVoiceCommand(VoiceCommandContext& context)
{
#if 1
	// Record a few seconds of audio
	context.audio_data->clear();
	SDL_PauseAudioDevice(context.audio_dev_id, /*pause_on=*/SDL_FALSE); // Start recording

	conPrint("-----------------------Recording...-----------------------");

	const double desired_num_secs = 4;
	while(context.audio_data->size() < context.obtained_spec->freq * desired_num_secs)
	{
		PlatformUtils::Sleep(1);
	}

	SDL_PauseAudioDevice(context.audio_dev_id, /*pause_on=*/SDL_TRUE); // Pause recording
	conPrint("-----------------------Recording stopped.-----------------------");




	struct whisper_full_params whisper_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
	whisper_params.n_threads = 8; // myMax(1u, PlatformUtils::getNumLogicalProcessors());
	whisper_params.print_special = false;
	whisper_params.suppress_blank = true;
	//whisper_params.duration_ms = 1000;
	//whisper_params.speed_up = true;

	Timer timer;

	if(whisper_full(context.whisper_ctx, whisper_params, context.audio_data->data(), (int)context.audio_data->size()) != 0) {
	//if(whisper_full_parallel(context.whisper_ctx, whisper_params, context.audio_data->data(), (int)context.audio_data->size(), whisper_params.n_threads) != 0) {
		throw glare::Exception("failed to process audio");
	}

	conPrint("Whisper inference took " + timer.elapsedString());

	std::string combined_text = "";

	const int n_segments = whisper_full_n_segments(context.whisper_ctx);
	//printVar(n_segments);
	for (int i = 0; i < n_segments; ++i)
	{
		std::string segment_text = whisper_full_get_segment_text(context.whisper_ctx, i);
		conPrint(segment_text);

		combined_text += segment_text;
		if(i + 1 < n_segments)
			combined_text += " ";
	}
#else
	//std::string combined_text = "What is one plus two?";
	std::string combined_text = "Set the volume to 0.1";
#endif


	context.chat_messages.push_back(ChatMessage({"user", combined_text}));

	//context.query += "User: " + combined_text + " \n";
	//context.query += "Project 2501: ";
	const std::string use_prompt = context.base_prompt; // StringUtils::replaceCharacter(context.base_prompt, '\n', ' ');

	std::string messages_json = "[{\"role\": \"system\", \"content\": \"" + web::Escaping::JSONEscape(use_prompt) + "\"},";
	for(size_t z=0; z<context.chat_messages.size(); ++z)
	{
		messages_json += "{\"role\": \"" + context.chat_messages[z].role + "\", \"content\": \"" + web::Escaping::JSONEscape(context.chat_messages[z].content) + "\"}";
		if(z + 1 < context.chat_messages.size())
			messages_json += ",";
	}
	messages_json += "]";


	conPrint("========================================================");
	conPrint("messages_json: " + messages_json);
	conPrint("========================================================");

	//return;

	{
		HTTPClient http_client;
		http_client.additional_headers.push_back("Content-Type: application/json");
		http_client.additional_headers.push_back("Authorization: Bearer " + context.openai_api_key);

		//const std::string escaped_query = web::Escaping::JSONEscape(context.query);
		//const std::string post_content = "{\"model\": \"text-davinci-003\", \"prompt\": \"" + escaped_query + "\", \"temperature\": 0, \"max_tokens\": 100}";
		const std::string post_content = 
		"{" \
		"	\"model\": \"gpt-3.5-turbo\"," \
		"	\"messages\": " + messages_json + 
		"}";

		conPrint(post_content);

		std::string data;
		HTTPClient::ResponseInfo response_info = http_client.sendPost(
			"https://api.openai.com/v1/chat/completions", // "https://api.openai.com/v1/completions", 
			post_content, 
			"application/json",
			data);

		conPrint(response_info.response_message);
		conPrint(data);

		/*
		Example response:
		{
		"id":"cmpl-6pcyOPK1Y1YEHEZpxRz1sFs2ZOcGB",
		"object":"text_completion",
		"created":1677762560,
		"model":"text-davinci-003",
		"choices":[
			{"text":"\n\nSubstrata VR is a virtual reality","index":0,"logprobs":null,"finish_reason":"length"}
		],
		"usage":{"prompt_tokens":10,"completion_tokens":10,"total_tokens":20}
		}
		*/

		if(response_info.response_code >= 200 && response_info.response_code < 300)
		{
			JSONParser json_parser;
			json_parser.parseBuffer(data.c_str(), data.size());

			const JSONNode& root = json_parser.nodes[0];
			checkNodeType(root, JSONNode::Type_Object);

			const JSONNode& choices_node = root.getChildArray(json_parser, "choices");

			for(size_t i=0; i<choices_node.child_indices.size(); ++i)
			{
				const JSONNode& choice_node = json_parser.nodes[choices_node.child_indices[i]];

				const JSONNode message_node = choice_node.getChildObject(json_parser, "message");

				const std::string role = message_node.getChildStringValue(json_parser, "role");
				const std::string content = message_node.getChildStringValue(json_parser, "content");
				//const std::string text = choice_node.getChildStringValue(parser, "text");

				conPrint("role: " + role);
				conPrint("content: " + content);

				//context.query += text + "\n";
				context.chat_messages.push_back(ChatMessage({"assistant", content}));

				// Process response
				const size_t set_volume_pos = content.find("set-volume");
				if(set_volume_pos != std::string::npos)
				{
					Parser parser(content);
					parser.setCurrentPos(set_volume_pos);
					bool res = parser.parseCString("set-volume");
					assert(res);
					parser.parseWhiteSpace();
					float volume;
					if(parser.parseFloat(volume))
					{
						volume = myClamp(volume, 0.f, 0.4f); // TEMP
						setSystemVolume(volume);

						//const std::string msg_to_speak = "The volume has been set to " + doubleToStringMaxNDecimalPlaces(volume, 2);
						//HRESULT hr = context.voice->Speak(StringUtils::UTF8ToWString(msg_to_speak).c_str(), 0, NULL);
						//if(FAILED(hr))
						//	throw glare::Exception("voice->Speak failed.");

					}
					else
					{
						conPrint("Failed to parse volume from '" + content + "'");
					}
				}
				else
				{
					
				}

				// Speak the response text
				HRESULT hr = context.voice->Speak(StringUtils::UTF8ToWString(content).c_str(), 0, NULL);
				if(FAILED(hr))
					throw glare::Exception("voice->Speak failed.");
				
				break;
			}
		}
		else
			throw glare::Exception("non-200 HTTP response code: " + toString(response_info.response_code) + ", " + response_info.response_message);
	}
}


int main(int /*argc*/, char** /*argv*/)
{
	Clock::init();
	Networking::createInstance();

	try
	{
		const std::string current_weather = getCurrentWeather();

		const std::string openai_api_key = ::stripHeadAndTailWhitespace(FileUtils::readEntireFile("N:\\aibot\\trunk\\openai_API_key.txt"));


		//----------------------------- Initialise whisper ------------------------------------
		struct whisper_context* whisper_ctx = whisper_init_from_file("N:\\aibot\\trunk\\ggml-base.en.bin");


		//----------------------------- Initialise loopback or microphone Audio capture ------------------------------------
		if(SDL_Init(SDL_INIT_AUDIO) != 0)
			throw glare::Exception("SDL_Init Error: " + std::string(SDL_GetError()));

		const int device_count = SDL_GetNumAudioDevices(/*is capture=*/SDL_TRUE);

		for(int i=0; i<device_count; ++i)
		{
			const char* dev_name = SDL_GetAudioDeviceName(i, /*is capture=*/SDL_TRUE);
			conPrint("Device " + toString(i) + ": " + std::string(dev_name));
		}

		const char* dev_name = SDL_GetAudioDeviceName(/*index=*/0, /*is capture=*/SDL_TRUE);

		
		std::vector<float> audio_data;
		const int audio_num_samples = 256;

		SDL_AudioSpec desired_spec;
		desired_spec.freq = 16000; // 48000;
		desired_spec.format = AUDIO_F32;
		desired_spec.channels = 1;
		desired_spec.samples = audio_num_samples;
		desired_spec.callback = audioCallback;
		desired_spec.userdata = &audio_data;
		SDL_AudioSpec obtained_spec;
		SDL_AudioDeviceID audio_dev_id = SDL_OpenAudioDevice(dev_name, /*is capture=*/SDL_TRUE, &desired_spec, &obtained_spec, /*allowed changes=*/SDL_TRUE);
		if(audio_dev_id == 0)
			throw glare::Exception("Failed to open audio device: " + std::string(SDL_GetError()));

		printVar(obtained_spec.freq);
		if(obtained_spec.format == AUDIO_F32)
			conPrint("AUDIO_F32");
		printVar(obtained_spec.format);
		printVar(obtained_spec.channels);
		printVar(obtained_spec.samples);


		//----------------------------- Initialise speech API (for text to speech) ------------------------------------
		if(FAILED(::CoInitialize(NULL)))
			throw glare::Exception("COM init failed");

		ISpVoice* voice = NULL;
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&voice);
		if(FAILED(hr))
			throw glare::Exception("CoCreateInstance failed for creating voice object: " + PlatformUtils::getLastErrorString());


		// Initialise speech recoginition
		ComObHandle<ISpRecognizer> recognizer;
		hr = CoCreateInstance(CLSID_SpInprocRecognizer, nullptr, CLSCTX_ALL, IID_ISpRecognizer, (void**)(&recognizer.ptr));
		if(FAILED(hr))
			throw glare::Exception("CoCreateInstance failed for creating recogniser object: " + PlatformUtils::getLastErrorString());


		ComObHandle<ISpAudio> audio_stream;
		hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &audio_stream.ptr);
		check_result(hr);

		// Set the audio input to our object.
		hr = recognizer->SetInput(audio_stream.ptr, TRUE);
		check_result(hr);

		ComObHandle<ISpRecoContext> recog_context;
		hr = recognizer->CreateRecoContext(&recog_context.ptr);
		if(FAILED(hr))
			throw glare::Exception("CreateRecoContext failed: " + PlatformUtils::getLastErrorString());

		recog_context->Pause(0);


		ISpRecoGrammar* recoGrammar = init_grammar(recog_context.ptr, "hey twenty");

		hr = recog_context->SetNotifyWin32Event();
		check_result(hr);


		HANDLE recognition_event;
		recognition_event = recog_context->GetNotifyEventHandle();
		if(recognition_event == INVALID_HANDLE_VALUE)
			throw glare::Exception("GetNotifyEventHandle failed.");
			

		ULONGLONG interest;
		interest = SPFEI(SPEI_RECOGNITION);
		hr = recog_context->SetInterest(interest, interest);
		check_result(hr);

		// Activate Grammar
		hr = recoGrammar->SetRuleState(ruleName1, 0, SPRS_ACTIVE);
		check_result(hr);

		// Enable context
		hr = recog_context->Resume(0);
		check_result(hr);

		VoiceCommandContext context;
		context.audio_dev_id = audio_dev_id;
		context.obtained_spec = &obtained_spec;
		context.audio_data = &audio_data;
		context.whisper_ctx = whisper_ctx;
		context.openai_api_key = openai_api_key;
		context.voice = voice;
		context.current_weather = current_weather;

		std::string base_prompt;
		base_prompt += "You are a helpful assistant who can also execute commands.\n";
		base_prompt += "The user input is from voice recognition so may be recognised incorrectly.\n";
		base_prompt += "The current time is " + Clock::getAsciiTime() + ".\n";
		base_prompt += "The current weather is " + context.current_weather + "\n";
		base_prompt += getSystemVolumeDescription();
		//base_prompt += "Project 2501 is helpful, creative, clever, and very friendly.\n";
		//base_prompt += "If the user wants to set the volume, Project 2501 should reply with \n";
		base_prompt += "If the user wants to set the volume, execute the command set-volume x, where x is the requested volume.\n";
		base_prompt += "Example: Please set to the volume to 0.3.\n";
		base_prompt += "Output: set-volume 0.3.\n";
		//base_prompt += "Allowed volume values range from 0 to 1.\n";
		//context.query = base_prompt;
		context.base_prompt = base_prompt;

		//doVoiceCommand(context);
		//return 0;
		while(1)
		{
			DWORD result = WaitForSingleObject(recognition_event, 1000);
			if(result == WAIT_OBJECT_0)
			{
				conPrint("Recognised word!");
				
				recognizer->SetRecoState(SPRST_INACTIVE); // Pause trigger word detection while we do a voice command and speak the response.

				doVoiceCommand(context);

				recognizer->SetRecoState(SPRST_ACTIVE);
			}
			else
			{
				conPrintStr(".");
			}
		}

	}
	catch(glare::Exception& e)
	{
		conPrint(e.what());
	}

	return 0;
}
