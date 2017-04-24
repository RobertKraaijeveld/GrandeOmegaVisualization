#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include "JSONEncoder.h"
#include "../StatisticalAnalyzer/KMeans/CustomTypes/Point.h"


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

//DUPLICATION WITH ABOVE
string JSONEncoder::mapToJson(map<string, int> m)
{
    stringstream returnJSONStr;

    returnJSONStr << "{ \n";

    int counter = 0;
    map<string, int>::iterator it;
    for(it = m.begin(); it != m.end(); it++)
    {
        if(counter < (m.size() - 1))
           returnJSONStr << '"' << it->first << '"' << ':' << ' ' << '"' << to_string(it->second) << '"' << ',' << endl;  
        else
           returnJSONStr << '"' << it->first << '"' << ':' << ' ' << '"' << to_string(it->second) << '"' << endl;       
        
        counter++;       
    }
    returnJSONStr << " }";
    return returnJSONStr.str();
}

string JSONEncoder::clustersToJSON(vector<vector<Point>> clusters)
{
    stringstream returnJSONStr;

    returnJSONStr << "{ \n";

    for(int i = 0; i < clusters.size(); i++)
    {
        for(int j = 0; j < clusters[i].size(); j++)
        {
            for(int z = 0; z < clusters[i][j].vector.values.size(); z++)
            {
                returnJSONStr << '"' << clusters[i][j].vector.values[z];

                if(z == 0)
                    returnJSONStr << '"' << ": ";                
                else if (i < clusters[i].size() - 1)
                    returnJSONStr << '"' << ", ";
                else 
                    returnJSONStr << '"' << ", ";
            }
        } 
    }
    returnJSONStr << " }";
    return returnJSONStr.str();
}

