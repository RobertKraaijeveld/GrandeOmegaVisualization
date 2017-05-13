#ifndef JSONENCODER_H
#define JSONENCODER_H

#include "Utilities.h"
#include "../StatisticalAnalyzer/GenericVector/GenericVector.h"
#include "../StatisticalAnalyzer/KMeans/CustomTypes/KMeansPoint.h"
#include <vector>
#include <string>

using namespace std;

class JSONEncoder {
    //make types more, generic this will still sometimes fail
    private:
        template <typename T, typename J>    
        static string kvToJson(T& k, J& v);

    public: 
        template <typename T, typename J>
        static string pairToJson(pair<T, J>& pair);

        template <typename T, typename J>        
        static string pairsToJson(vector<pair<T, J>>& pairList);

        template <typename T, typename J>        
        static string mapToJson(map<T, J>& map);
        template <typename T, typename J, typename L>
        static string mapToJson(map<T, pair<J, L>>& mapOfPairs);
        
        static string clustersToJSON(vector<vector<KMeansPoint>> clusters);
};

template <typename T, typename J>    
string JSONEncoder::kvToJson(T& k, J& v)
{
    stringstream returnStr;    
    returnStr << "\"x\"" << ':' << ' ' << '"' << Utilities::genericToStr(k) << '"' << ", ";
    returnStr << "\"y\"" << ':' << ' ' << '"' << Utilities::genericToStr(v) << '"';
    return returnStr.str();
}


//duplication with map, only loop is different
template <class T, class J>        
string JSONEncoder::pairsToJson(vector<pair<T, J>>& pairs)
{
    stringstream returnJSONStr;

    returnJSONStr << "[ \n";
    for(int i = 0; i < pairs.size(); i++)
    {
        returnJSONStr << "{ ";

        returnJSONStr << kvToJson(pairs[i].first, pairs[i].second);

        //add comma after object if not at last pair
        if(i < pairs.size() - 1)
             returnJSONStr << "}," << endl;  
        else
             returnJSONStr << "}" << endl;          
    }
    returnJSONStr << " ]";
    return returnJSONStr.str();  
}

//TODO: eerily familiar with above method
template <typename T, typename J>
string JSONEncoder::mapToJson(map<T, J>& m)
{
    stringstream returnJSONStr;

    returnJSONStr << "[ \n";

    int counter = 0;
    typename map<T, J>::iterator it;
    for(it = m.begin(); it != m.end(); it++)
    {
        returnJSONStr << "{";
        returnJSONStr << kvToJson(it->first, it->second);
        
        if(counter < (m.size() - 1))
            returnJSONStr << "}," << endl;  
        else
            returnJSONStr << "}" << endl;        
        
        counter++;       
    }
    returnJSONStr << " ]";
    return returnJSONStr.str();
}

template <typename T, typename J, typename L>
string JSONEncoder::mapToJson(map<T, pair<J, L>>& mapOfPairs)   
{
    //extract as much repetition as possible, like appending and prepending braces.
    stringstream returnJSONStr;

    returnJSONStr << "[ ";

    int counter = 0;
    typename map<T, pair<J, L>>::iterator it;    
    for(it = mapOfPairs.begin(); it != mapOfPairs.end(); it++)
    {
        J pairL = it->second.first;
        L pairR = it->second.second;        

        returnJSONStr << "{";

        returnJSONStr << "\"name\":\"" << Utilities::genericToStr(it->first) << "\", ";
        returnJSONStr << "\"data\": {";

        returnJSONStr << "\"x\": " << '"'  << Utilities::genericToStr(pairL) << '"'  << ',' 
           << " \"y\": " << '"' << Utilities::genericToStr(pairR) << '"';  

        //if not at last pair, add comma
        if(counter < (mapOfPairs.size() - 1))
        {
            returnJSONStr << "}}," << endl;
        }        
        else
        {
            returnJSONStr << "}}" << endl;            
        }
        counter++;       
    }
    returnJSONStr << " ]";

    cout << "returnJSONStr.str() = " << returnJSONStr.str() << endl; 

    return returnJSONStr.str();
}

string JSONEncoder::clustersToJSON(vector<vector<KMeansPoint>> clusters)
{
    stringstream returnJSONStr;

    //obj opening brace
    returnJSONStr << "[" << "\n";

    //all clusters
    for(int i = 0; i < clusters.size(); i++)
    {
        size_t lastClusterIndex = clusters.size();
        size_t clusterCounter = i;

        //opening brace for cluster
        returnJSONStr << "{ \"name\": \"" << clusterCounter << "\", \"data\": [" << endl;

        //vectors in a single cluster
        for(int j = 0; j < clusters[i].size(); j++)
        {
            //opening brace for vector pair            
            returnJSONStr << "{";
            
            //values in a single vector
            for(int z = 0; z < clusters[i][j].vector.values.size(); z++)
            {
                size_t currVectorSize = clusters[i][j].vector.values.size();

                //todo: generify for > 2 dimensional KMeans 
                if(z == 0)
                    returnJSONStr << "\"x\": " << '"' << clusters[i][j].vector.values[z] << '"' << ',';
                else
                    returnJSONStr << "\"y\": " << '"' << clusters[i][j].vector.values[z] << '"';
            }
            size_t lastClusterVectorIndex = clusters[i].size() - 1; 

            //closing brace for vector pair
            returnJSONStr << "}";

            //comma if theres clusters left to go                
            if(i != lastClusterIndex && j != lastClusterVectorIndex)
                returnJSONStr << ", ";
        }
        //closing brace of entire cluster
        if(i != (lastClusterIndex- 1))
            returnJSONStr << "] }," << endl;
        else
            returnJSONStr << "] }" << endl;            
    }
    //final json obj closing brace 
    returnJSONStr << " ]";
    return returnJSONStr.str();
}



#endif