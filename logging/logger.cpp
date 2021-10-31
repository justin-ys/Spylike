#include "logger.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;
#include <string>

SpylikeLogger::SpylikeLogger(string fname, LogLevel level) : filename(fname), loglevel(level) {
	ofstream logfile;
	logfile.open(filename);
	logfile.close();
}

void SpylikeLogger::log(string text, LogLevel level) { 
    if (loglevel >= level) {
        string levelname;
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
        char mbuffer[levelname.length() + text.length() + 3];
        sprintf(mbuffer, "[%s] %s", levelname.c_str(), text.c_str());
        string message = mbuffer;
        ofstream logfile {filename, ios_base::app};
        logfile << message + '\n';
        logfile.close();
    }
}
    
