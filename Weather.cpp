/*=====================================================================
Weather.cpp
-----------
Copyright Nicholas Chapman 2023 -
=====================================================================*/
#include "Weather.h"


#include <networking/HTTPClient.h>
#include <utils/ConPrint.h>
#include <utils/JSONParser.h>
#include <utils/Exception.h>
#include <utils/StringUtils.h>


// NOTE: Location is currently hardcoded to Wellington, NZ.
std::string getCurrentWeather()
{
	HTTPClient http_client;
	std::string data;
	HTTPClient::ResponseInfo response_info = http_client.downloadFile(
		"https://api.open-meteo.com/v1/forecast?latitude=-41.33&longitude=174.78&daily=weathercode,temperature_2m_max,windspeed_10m_max&timezone=Pacific%2FAuckland&current_weather=true", 
		data);

	/*
	Example response:
	{
	"latitude": -41.25,
	"longitude": 174.75,
	"generationtime_ms": 0.3609657287597656,
	"utc_offset_seconds": 46800,
	"timezone": "Pacific/Auckland",
	"timezone_abbreviation": "NZDT",
	"elevation": 97,
	"current_weather": {
	"temperature": 15.2,
	"windspeed": 8.4,
	"winddirection": 20,
	"weathercode": 2,
	"time": "2023-03-27T01:00"
	},
	"daily_units": {
	"time": "iso8601",
	"weathercode": "wmo code",
	"temperature_2m_max": "°C",
	"windspeed_10m_max": "km/h"
	},
	"daily": {
	"time": [
	"2023-03-27",
	"2023-03-28",
	"2023-03-29",
	"2023-03-30",
	"2023-03-31",
	"2023-04-01",
	"2023-04-02"
	],
	"weathercode": [
	3,
	61,
	3,
	2,
	3,
	3,
	3
	],
	"temperature_2m_max": [
	17.8,
	18.2,
	9.9,
	12.8,
	16.8,
	18.1,
	19
	],
	"windspeed_10m_max": [
	36.2,
	43.7,
	42.6,
	33.6,
	30.4,
	37.1,
	18.4
	]
	}
	}
	*/
	if(response_info.response_code >= 200 && response_info.response_code < 300)
	{
		JSONParser parser;
		parser.parseBuffer(data.c_str(), data.size());

		const JSONNode& root = parser.nodes[0];
		checkNodeType(root, JSONNode::Type_Object);

		std::string temp_summary;
		{
			const JSONNode& current_weather_node = root.getChildObject(parser, "current_weather");

			const double temperature = current_weather_node.getChildDoubleValue(parser, "temperature");
			const double windspeed = current_weather_node.getChildDoubleValue(parser, "windspeed");

			temp_summary += "current temperature: " + toString(temperature) + " celcius, current wind speed: " + toString(windspeed) + " kph\n";
		}
		{
			const JSONNode& daily_node = root.getChildObject(parser, "daily");

			const JSONNode& temperature_2m_max_node = daily_node.getChildArray(parser, "temperature_2m_max");
			std::vector<double> daily_temps(7);
			temperature_2m_max_node.parseDoubleArrayValues(parser, /*expected_num_elems=*/7, &daily_temps[0]);

			const JSONNode& windspeed_10m_max = daily_node.getChildArray(parser, "windspeed_10m_max");
			std::vector<double> daily_windspeed(7);
			windspeed_10m_max.parseDoubleArrayValues(parser, /*expected_num_elems=*/7, &daily_windspeed[0]);

			for(int i=1; i<7; ++i)
			{
				temp_summary += "Temperature in " + toString(i) + " day(s) from now: " + doubleToStringMaxNDecimalPlaces(daily_temps[i], 1) + " celcius, " +
					"windspeed in " + toString(i) + " day(s) from now: " + doubleToStringMaxNDecimalPlaces(daily_windspeed[i], 1) + " kph\n";
			}
		}

		return temp_summary;
	}
	else
		throw glare::Exception("non-200 HTTP response code: " + toString(response_info.response_code) + ", " + response_info.response_message);
}
