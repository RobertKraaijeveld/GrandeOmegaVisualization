#ifndef JSONENCODER_H
#define JSONENCODER_H

#include <vector>
#include <string>

using namespace std;

//temporary stuff until ruby types are fixed or I use a lib
class JSONEncoder {
    //make types generic
    public: 
        static string pairsToJson(vector<pair<string, int>> pairList);
};

#endif