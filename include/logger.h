#ifndef SPYLIKE_LOGGING_H
#define SPYLIKE_LOGGING_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

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
    private:
		std::string filename;
		LogLevel loglevel;
};
        
#endif
