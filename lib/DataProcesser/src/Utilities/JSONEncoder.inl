#ifndef JSONENCODER_INL
#define JSONENCODER_INL

#include "Utilities.h"
#include "../StatisticalAnalyzer/GenericVector/GenericVector.h"
#include "../StatisticalAnalyzer/Point/IClusteringPoint.h"
#include "JSONEncoder.h"

#include <vector>
#include <string>

 
/*
This .inl file is included directly into the JSONEncoder header;
This makes sure we do not get any linker errors on these templates.
*/ 


template <class T, class J> 
std::string JSONEncoder::kvToJson(T& k, J& v)
{
    stringstream returnStr;    
    returnStr << "\"x\"" << ':' << ' ' << '"' << Utilities::genericToStr(k) << '"' << ", ";
    returnStr << "\"y\"" << ':' << ' ' << '"' << Utilities::genericToStr(v) << '"';
    return returnStr.str();
}


template <class T, class J> 
std::string JSONEncoder::pairsToJson(std::vector<pair<T, J>>& pairs)
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
template <class T, class J> 
std::string JSONEncoder::mapToJson(std::map<T, J>& m, bool separateIntoXY)
{
    stringstream returnJSONStr;

    returnJSONStr << "[ \n";

    int counter = 0;
    typename std::map<T, J>::iterator it;
    for(it = m.begin(); it != m.end(); it++)
    {
        returnJSONStr << "{";

        //inserting x: and y: or not
        if(separateIntoXY)
            returnJSONStr << kvToJson(it->first, it->second);
        else
            returnJSONStr << "\"" << it->first << "\"" << ": " << "\"" << it->second << "\"";
            

        if(counter < (m.size() - 1))
            returnJSONStr << "}," << endl;  
        else
            returnJSONStr << "}" << endl;        
        
        counter++;       
    }
    returnJSONStr << " ]";
    return returnJSONStr.str();
}

template <class T, class J, class L>  
std::string JSONEncoder::mapToJson(std::map<T, pair<J, L>>& mapOfPairs)   
{
    //extract as much repetition as possible, like appending and prepending braces.
    stringstream returnJSONStr;

    returnJSONStr << "[ ";

    int counter = 0;
    class std::map<T, pair<J, L>>::iterator it;    
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

    return returnJSONStr.str();
}

#endif