#ifndef SPYLIKE_LOGGING_H
#define SPYLIKE_LOGGING_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

#ifdef USE_NCURSESW
#include <locale>
#include <codecvt>
#endif

enum LogLevel {DEBUG=0, ERROR=1, CRITICAL=2};

class SpylikeLogger {
	public:
        SpylikeLogger(std::string fname, LogLevel level);
        template<class T> void log(T text, LogLevel level) { 
			if (loglevel >= level) {
				std::string levelname;
				switch(level) {
					case DEBUG:
						levelname = "DEBUG";
						break;
					case ERROR:
						levelname = "ERROR";
						break;
					case CRITICAL:
						levelname = "CRITICAL";
						break;
				}
			std::ofstream logfile {filename, std::ios_base::app};
			logfile << '[' << levelname << "] " << text << std::endl;
			logfile.close();
			}
		}
		#ifdef USE_NCURSESW
		// Based on https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
		void log(std::wstring text, LogLevel level) { 
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			std::string converted = converter.to_bytes(text);
			log(converted, level);
		}
		#endif
    private:
		std::string filename;
		LogLevel loglevel;
};
        
#endif
