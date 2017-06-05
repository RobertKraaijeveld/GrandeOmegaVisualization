#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <sstream>
#include <pqxx/pqxx>

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
    static vector<J> getValuesOfMap(std::map<T, J> m); 

    //Filter related
    static vector<pqxx::result::tuple> toListOfPqxxTuples(pqxx::result &r);

    //Misc.
    template <class T>
    static string genericToStr(const T &val);
    static vector<string> toArrayByDelim(string &s, char delim);
};

//these have to be declared here because of the templates they use

template <class T, class J>
vector<J> Utilities::getValuesOfMap(map<T, J> m)
{
    vector<J> returnVector;
    //typename necessary because c++ cannot distinguish between var and type here without non-local information
    for (typename std::map<T,J>::iterator it = m.begin(); it != m.end(); ++it) {
        returnVector.push_back(it->second);
    }
    return returnVector;
}



template <class T>
string Utilities::genericToStr(const T &val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

#endif