

#include <ConPrint.h>
#include <Exception.h>
#include <Clock.h>
#include <networking/Networking.h>
#include <networking/HTTPClient.h>

int main(int /*argc*/, char** /*argv*/)
{
	Clock::init();
	Networking::createInstance();
	try
	{
		{
			HTTPClient http_client;
			http_client.additional_headers.push_back("Content-Type: application/json");
			http_client.additional_headers.push_back("Authorization: Bearer XXXXXX");
			
			const std::string post_content = "{\"model\": \"text-davinci-003\", \"prompt\": \"Say this is a test\", \"temperature\": 0, \"max_tokens\": 7}";

			std::string data;
			HTTPClient::ResponseInfo response_info = http_client.sendPost(
				"https://api.openai.com/v1/completions", 
				post_content, 
				"application/json", //  
				data);

			conPrint(response_info.response_message);
			conPrint(data);

		}



	}
	catch(glare::Exception& e)
	{
		conPrint(e.what());
	}

	return 0;
}
