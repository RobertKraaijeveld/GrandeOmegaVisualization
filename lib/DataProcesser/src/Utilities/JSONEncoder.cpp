#include <vector>
#include <string>
#include <sstream>
#include <iostream> 
#include "JSONEncoder.h"

using namespace std;

//make arg types generic
string JSONEncoder::pairsToJson(vector<pair<string, int>> pairs)
{
    stringstream returnJSONStr;

    returnJSONStr << "{ \n";
    for(int i = 0; i < pairs.size(); i++)
    {
        //split this up
        if(i < pairs.size() - 1)
            returnJSONStr << '"' << pairs[i].first << '"' << ':' << ' ' << '"' << to_string(pairs[i].second) << '"' << ',' << endl;  
        else
            returnJSONStr << '"' << pairs[i].first << '"' << ':' << ' ' << '"' << to_string(pairs[i].second) << '"' << endl;              
    }
    returnJSONStr << " }";
    return returnJSONStr.str();
}

