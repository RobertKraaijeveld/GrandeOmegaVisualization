#ifndef YAMLOBJECT_H
#define YAMLOBJECT_H
 
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <map>
 
using namespace std;
 
class YamlObject {
    public: 
        static string identifierPattern;
        map<string, string> values;
        map<string, YamlObject> children;

        string getIdentifier();
        YamlObject(){};
};


#endif