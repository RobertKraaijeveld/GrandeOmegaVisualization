#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
 
using namespace std;
 

class Utilities {
    public: 
        static vector<string> getListOfNumberedFilesForBaseFile(string baseFileString);

        static void removeYamlReferences(string& str);
        static string createStrWithWhiteSpaces(int amount);
        static void removeStrWhiteSpace(string& str);
        static bool isNCharsWhiteSpace(int N, string& str);

        static bool isInVector(string& value, vector<string>& vector);
};

#endif