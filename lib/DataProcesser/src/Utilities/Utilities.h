#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <sstream>
#include <pqxx/pqxx>
 
using namespace std;
 

class Utilities {
    public: 
        //Yaml related
        static vector<string> getListOfNumberedFilesForBaseFile(string baseFileString);
        static void removeYamlReferences(string& str);
        static string createStrWithWhiteSpaces(int amount);
        static void removeStrWhiteSpace(string& str);
        static bool isNCharsWhiteSpace(int N, string& str);

        //Statistical tools related
        static bool AreFloatsEqual(float f1, float f2);
        static int computeAverage(vector<int>& values);
        static bool isInVector(string& value, vector<string>& vector);

        //Filter related
        static vector<pqxx::result::tuple> toListOfPqxxTuples(pqxx::result& r);

        //Misc.
        template<class T>
        static string genericToStr(const T& val);

        static vector<string> toArrayByDelim(string& s, char delim);

};

//has to be declared here because of the template for it
template<class T>
string Utilities::genericToStr(const T& val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}


#endif