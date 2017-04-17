#ifndef YAMLPARSER_H
#define YAMLPARSER_H

#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <map>
#include "YamlObject.h" 
 
using namespace std;
 
class YamlParser {
    public:
         YamlParser(ifstream & filestr) : file(filestr) { }
         vector<YamlObject> parseListOfFiles(string basefileStr);
         vector<YamlObject> parseYaml();

         ifstream& file;
         
    private:
         void parseLine(string& currentLine, string& nextline, YamlObject& currentYamlObject);

         bool atEndOfFile(string& nextLine);      
         bool isBeginOfFile(string& currentline); 
         bool isStartOfChildObject(string& currentline, string& nextline);
         bool isChildObjectValue(string& line);     
         bool reachedNewYamlObject(string& nextline, YamlObject& currentYamlObject);

         void handleNewYamlObject();

         pair<string, string> getKvForLine(stringstream& linestr);
         pair<string, YamlObject> createNewChildPair(pair<string, string>& kvPairForLine);

         string getKeyOfLastChild(map<string, YamlObject>& children); 
         string peekNextLine();
};

#endif