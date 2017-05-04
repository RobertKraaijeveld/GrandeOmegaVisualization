#ifndef UTCREADER_H
#define UTCREADER_H

#include <string>
#include <vector>

#include "UtcTime.h" 


class UtcReader {
    private:
        static bool allTimeValuesPresent(vector<std::string> ymdVector, vector<std::string> hmsVector);
    public: 
        static UtcTime toUtcTime(std::string& s);
}; 

#endif