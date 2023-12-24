#include "logger.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

SpylikeLogger::SpylikeLogger(std::string fname, LogLevel level) : filename(fname), loglevel(level) {
	std::ofstream logfile;
	logfile.open(filename);
	logfile.close();
}
    
