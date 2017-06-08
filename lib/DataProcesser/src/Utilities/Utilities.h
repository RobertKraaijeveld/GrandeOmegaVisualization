#ifndef UTILITIES_H
#define UTILITIES_H

#include "../StatisticalAnalyzer/Point/IClusteringPoint.h"
#include "../StatisticalAnalyzer/Point/KMeansPoint.h"

#include <vector>
#include <string>
#include <sstream>
#include <pqxx/pqxx>
#include <iostream>

using namespace std;

class Utilities
{
  public:
    //Yaml related
    static vector<string> getListOfNumberedFilesForBaseFile(string baseFileString);
    static void removeYamlReferences(string &str);
    static string createStrWithWhiteSpaces(int amount);
    static void removeStrWhiteSpace(string &str);
    static bool isNCharsWhiteSpace(int N, string &str);

    //Statistical tools related
    static bool AreFloatsEqual(float f1, float f2);
    static int computeAverage(vector<int> &values);
    static bool isInVector(string &value, vector<string> &vector);

    //Vector tools
    template <class T, class J>
    static vector<T> getKeysOfMap(map<T, J> m);
    template <class T, class J>
    static vector<J> getValuesOfMap(map<T, J> m);
    template <class T, class J>
    static pair<T, J> getHighestOrLowestValueKV(map<T, J> m, bool getHighest);

    //Filter related
    static vector<pqxx::result::tuple> toListOfPqxxTuples(pqxx::result &r);

    //Misc.
    template <class T>
    static string genericToStr(const T &val);
    static vector<string> toArrayByDelim(string &s, char delim);

    //needs to get some kind of type covariance
    static vector<IClusteringPoint*> convertMapOfPairsToPoints(map<string, pair<int, int>> inputValues);
    static vector<KMeansPoint*> convertMapOfPairsToKMeansPoints(map<string, pair<int, int>> inputValues);
};

//these have to be declared here because of the templates they use

template <class T, class J>
vector<T> Utilities::getKeysOfMap(map<T, J> m)
{
    typename std::vector<T> returnVector;
    //typename necessary because c++ cannot distinguish between var and type here without non-local information
    for (typename std::map<T, J>::iterator it = m.begin(); it != m.end(); ++it)
    {
        returnVector.push_back(it->first);
    }
    return returnVector;
}

template <class T, class J>
vector<J> Utilities::getValuesOfMap(map<T, J> m)
{
    typename std::vector<J> returnVector;
    //typename necessary because c++ cannot distinguish between var and type here without non-local information
    for (typename std::map<T, J>::iterator it = m.begin(); it != m.end(); ++it)
    {
        returnVector.push_back(it->second);
    }
    return returnVector;
}

template <class T, class J>
pair<T, J> Utilities::getHighestOrLowestValueKV(map<T, J> m, bool getHighest)
{
    typename std::pair<T, J> resultKV;
    J mostFitValueYet = 0;

    for (typename std::map<T, J>::iterator it = m.begin(); it != m.end(); ++it)
    {
        //make comparator
        if ((getHighest == true && it->second > mostFitValueYet) || (getHighest == false && it->second < mostFitValueYet))
        {
            mostFitValueYet = it->second;
            resultKV = make_pair(it->first, it->second);
        }
    }
    return resultKV;
}

template <class T>
string Utilities::genericToStr(const T &val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

#endif