

#include <ConPrint.h>
#include <Exception.h>
#include <Clock.h>
#include <FileUtils.h>
#include <StringUtils.h>
#include <JSONParser.h>
#include <PlatformUtils.h>
#include <Timer.h>
#include <ComObHandle.h>
#include <maths/mathstypes.h>
#include <webserver/Escaping.h>
#include <networking/Networking.h>
#include <networking/HTTPClient.h>
#include <whisper.cpp/whisper.h>
#include <SDL.h>
#include <sapi.h> // Speech API


static void checkNodeType(const JSONNode& node, JSONNode::Type type)
{
	if(node.type != type)
		throw glare::Exception("Expected type " + JSONNode::typeString(type) + ", got type " + JSONNode::typeString(node.type) + ".");
}


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


int main(int /*argc*/, char** /*argv*/)
{
	Clock::init();
	Networking::createInstance();

	if(true)
	try
	{
		const std::string openai_api_key = ::stripHeadAndTailWhitespace(FileUtils::readEntireFile("N:\\aibot\\trunk\\openai_API_key.txt"));

		//----------------------------- Initialise whisper ------------------------------------
		struct whisper_context* whisper_ctx = whisper_init_from_file("N:\\aibot\\trunk\\ggml-base.en.bin");


		//----------------------------- Initialise speech API (for text to speech) ------------------------------------
		if(FAILED(::CoInitialize(NULL)))
			throw glare::Exception("COM init failed");

		ISpVoice* voice = NULL;
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&voice);
		if(FAILED(hr))
			throw glare::Exception("CoCreateInstance failed for creating voice object: " + PlatformUtils::getLastErrorString());


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

		
		//----------------------------------------------------------------------------------------
		std::string query = "The assistant is helpful, creative, clever, and very friendly.";

		while(1)
		{
			PlatformUtils::Sleep(2000);

			// Record a few seconds of audio
			audio_data.clear();
			SDL_PauseAudioDevice(audio_dev_id, /*pause_on=*/SDL_FALSE); // Start recording

			conPrint("-----------------------Recording...-----------------------");
		
			const double desired_num_secs = 4;
			while(audio_data.size() < obtained_spec.freq * desired_num_secs)
			{
				PlatformUtils::Sleep(1);
			}
			
			SDL_PauseAudioDevice(audio_dev_id, /*pause_on=*/SDL_TRUE); // Pause recording
			conPrint("-----------------------Recording stopped.-----------------------");




			struct whisper_full_params whisper_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
			whisper_params.n_threads = myMax(1u, PlatformUtils::getNumLogicalProcessors() / 2);
			whisper_params.print_special = false;
			whisper_params.suppress_blank = true;
			//whisper_params.duration_ms = 1000;

			Timer timer;

			if(whisper_full(whisper_ctx, whisper_params, audio_data.data(), (int)audio_data.size()) != 0) {
			//if(whisper_full_parallel(whisper_ctx, whisper_params, audio_data.data(), (int)audio_data.size(), whisper_params.n_threads) != 0) {
				throw glare::Exception("failed to process audio");
			}

			conPrint("Whisper inference took " + timer.elapsedString());

			std::string combined_text = "";

			const int n_segments = whisper_full_n_segments(whisper_ctx);
			//printVar(n_segments);
			for (int i = 0; i < n_segments; ++i)
			{
				std::string segment_text = whisper_full_get_segment_text(whisper_ctx, i);
				conPrint(segment_text);

				combined_text += segment_text;
				if(i + 1 < n_segments)
					combined_text += " ";
			}



			query += "You: " + combined_text + " \n";
			query += "The assistant: ";

			conPrint("========================================================");
			conPrint("query: " + query);

			

			{
				HTTPClient http_client;
				http_client.additional_headers.push_back("Content-Type: application/json");
				http_client.additional_headers.push_back("Authorization: Bearer " + openai_api_key);
			
				const std::string escaped_query = web::Escaping::JSONEscape(query);
				const std::string post_content = "{\"model\": \"text-davinci-003\", \"prompt\": \"" + escaped_query + "\", \"temperature\": 0, \"max_tokens\": 100}";

				std::string data;
				HTTPClient::ResponseInfo response_info = http_client.sendPost(
					"https://api.openai.com/v1/completions", 
					post_content, 
					"application/json",
					data);

				conPrint(response_info.response_message);
				conPrint(data);

				/*
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
					JSONParser parser;
					parser.parseBuffer(data.c_str(), data.size());

					const JSONNode& root = parser.nodes[0];
					checkNodeType(root, JSONNode::Type_Object);

					const JSONNode& choices_node = root.getChildArray(parser, "choices");

					for(size_t i=0; i<choices_node.child_indices.size(); ++i)
					{
						const JSONNode& choice_node = parser.nodes[choices_node.child_indices[i]];

						const std::string text = choice_node.getChildStringValue(parser, "text");

						conPrint(text);

						query += text + "\n";

						// Speak the response text
						hr = voice->Speak(StringUtils::UTF8ToWString(text).c_str(), 0, NULL);
						if(FAILED(hr))
							throw glare::Exception("voice->Speak failed.");
					}
				}
				else
					throw glare::Exception("non-200 HTTP response code: " + toString(response_info.response_code) + ", " + response_info.response_message);
			}
		} // end while(1) loop
	}
	catch(glare::Exception& e)
	{
		conPrint(e.what());
	}

	return 0;
}
