#ifndef JSONENCODER_H
#define JSONENCODER_H

#include "Utilities.h"
#include "../StatisticalAnalyzer/KMeans/CustomTypes/Point.h"
#include <vector>
#include <string>

using namespace std;

class JSONEncoder {
    //make types generic
    public: 
        template <typename T, typename J>
        static string pairToJson(pair<T, J>& pair);

        template <typename T, typename J>        
        static string pairsToJson(vector<pair<T, J>>& pairList);

        template <typename T, typename J>        
        static string mapToJson(map<T, J>& map);
        template <typename T, typename J, typename L>
        static string mapToJson(map<T, pair<J, L>>& mapOfPairs);
        
        static string clustersToJSON(vector<vector<Point>> clusters);
};

//duplication with map, only loop is different
template <class T, class J>        
string JSONEncoder::pairsToJson(vector<pair<T, J>>& pairs)
{
    stringstream returnJSONStr;

    returnJSONStr << "{ \n";
    for(int i = 0; i < pairs.size(); i++)
    {
        //split this up
        if(i < pairs.size() - 1)
            returnJSONStr << '"' << Utilities::genericToStr(pairs[i].first) << '"' << ':' << ' ' << '"' << Utilities::genericToStr(pairs[i].second) << '"' << ',' << endl;  
        else
            returnJSONStr << '"' << Utilities::genericToStr(pairs[i].first) << '"' << ':' << ' ' << '"' << Utilities::genericToStr(pairs[i].second) << '"' << endl;              
    }
    returnJSONStr << " }";
    return returnJSONStr.str();  
}

template <typename T, typename J>
string JSONEncoder::mapToJson(map<T, J>& m)
{
    stringstream returnJSONStr;

    returnJSONStr << "{ \n";

    int counter = 0;
    typename map<T, J>::iterator it;
    for(it = m.begin(); it != m.end(); it++)
    {
        if(counter < (m.size() - 1))
           returnJSONStr << '"' << Utilities::genericToStr(it->first) << '"' << ':' << ' ' << '"' << Utilities::genericToStr(it->second) << '"' << ',' << endl;  
        else
           returnJSONStr << '"' << Utilities::genericToStr(it->first) << '"' << ':' << ' ' << '"' << Utilities::genericToStr(it->second) << '"' << endl;       
        
        counter++;       
    }
    returnJSONStr << " }";
    return returnJSONStr.str();
}

template <typename T, typename J, typename L>
string JSONEncoder::mapToJson(map<T, pair<J, L>>& mapOfPairs)   
{
    //extract as much repetition as possible, like appending and prepending braces.
    stringstream returnJSONStr;

    returnJSONStr << "{ \n";

    int counter = 0;
    typename map<T, pair<J, L>>::iterator it;    
    for(it = mapOfPairs.begin(); it != mapOfPairs.end(); it++)
    {
        J pairL = it->second.first;
        L pairR = it->second.second;        

        returnJSONStr << "{";

        returnJSONStr << "\"name\":\"Student no." << Utilities::genericToStr(it->first) << "\", ";
        returnJSONStr << "\"data\": ";

        returnJSONStr << '"' << Utilities::genericToStr(pairL) << '"' << ':' 
           << ' ' << '"' << Utilities::genericToStr(pairR) << '"';  
        
        returnJSONStr << "}" << endl;
        
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



#endif