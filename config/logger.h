#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#ifndef DEBUG
	#define DEBUG (false)
#endif

#ifndef INFO
	#define INFO (false)
#endif

#ifndef TESTING
	#define TESTING (false)
#endif

#define ERROR_LOG if(!TESTING) std::cerr << "ERROR " << __FILE__ << ":" << __LINE__ << ": "
#define INFO_LOG if(INFO) std::cerr << "INFO " << __FILE__ << ":" << __LINE__ << ": "
#define DEBUG_LOG if(DEBUG) std::cerr << "DEBUG " << __FILE__ << ":" << __LINE__ << ": "

inline void WEB_LOG(std::string title, std::string text, std::string type)
{
	static int id = 0;
	static time_t last_error_time = 0;

	time_t now;
	time(&now);

	if(type == "error")
	{
		time(&last_error_time);
	}

	if(now + 5 > last_error_time)
	{
		std::ofstream web_log;
		web_log.open("html/log.txt", std::ios::trunc);
		if(web_log.is_open())
		{
			web_log << "{\"id\":" << id++ << ", \"text\":\"" << text << "\", \"title\":\"" << title << "\", \"type\":\"" << type << "\"}";
			web_log.close();
		}
		else
		{
			ERROR_LOG << "couldn't open web log\n";
		}
	}
}

#endif