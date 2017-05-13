#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <sstream>
 
using namespace std;
 

class Utilities {
    public: 
        static vector<string> toArrayByDelim(string& s, char delim);

        static vector<string> getListOfNumberedFilesForBaseFile(string baseFileString);

        static void removeYamlReferences(string& str);
        static string createStrWithWhiteSpaces(int amount);
        static void removeStrWhiteSpace(string& str);
        static bool isNCharsWhiteSpace(int N, string& str);

        template<class T>
        static string genericToStr(const T& val);

        static bool AreFloatsEqual(float f1, float f2);

        static int computeAverage(vector<int>& values);
        static bool isInVector(string& value, vector<string>& vector);
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