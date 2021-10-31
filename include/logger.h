#ifndef SPYLIKE_LOGGING_H
#define SPYLIKE_LOGGING_H

#include <string>

enum LogLevel {DEBUG=0, ERROR=1, CRITICAL=2};

class SpylikeLogger {
	public:
        SpylikeLogger(std::string fname, LogLevel level);
        void log(std::string text, LogLevel level);
    private:
		std::string filename;
		LogLevel loglevel;
};
        
#endif
