#ifndef UTCREADER_H
#define UTCREADER_H

#include <string>

#include "UtcTime.h" 


class UtcReader {
    public: 
        static UtcTime toUtcTime(std::string& s);
}; 

#endif