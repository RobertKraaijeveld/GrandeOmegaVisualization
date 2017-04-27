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

    //array opening brace
    returnJSONStr << "[" << "\n";

    //all clusters
    for(int i = 0; i < clusters.size(); i++)
    {
        size_t lastClusterIndex = clusters.size();
        size_t clusterCounter = i;
        returnJSONStr << "{\"name\":\"Cluster " + to_string(clusterCounter + 1) + "\", \"data\": {";

            //vectors in a single cluster
        for(int j = 0; j < clusters[i].size(); j++)
        {
            //values in a single vector
            for(int z = 0; z < clusters[i][j].vector.values.size(); z++)
            {
                size_t currVectorSize = clusters[i][j].vector.values.size();

                //todo: generify for > 2 dimensional KMeans 
                if(z == 0)
                    returnJSONStr << "\"" <<  clusters[i][j].vector.values[z] << "\"" << ':';
                else
                    returnJSONStr << clusters[i][j].vector.values[z];
            }
            size_t lastClusterVectorIndex = clusters[i].size() - 1; 

            //comma if theres clusters left to go                
            if(i != lastClusterIndex && j != lastClusterVectorIndex)
                returnJSONStr << ", ";
        }
        //closing brace of "data" :            
        returnJSONStr << "}";

        //duplication
        //closing brace of entire cluster
        if(i != (lastClusterIndex- 1))
            returnJSONStr << "}," << "\n";
        else
            returnJSONStr << "}" << "\n";
    }
    //final json array closing brace 
    returnJSONStr << " ]";
    return returnJSONStr.str();
}

