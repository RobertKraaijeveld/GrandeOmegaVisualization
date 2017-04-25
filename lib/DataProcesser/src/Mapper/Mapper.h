#ifndef MAPPER_H
#define MAPPER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "../YamlParser/YamlObject.h"
#include "../YamlParser/YamlParser.h"

using namespace std; 

class Mapper {
    private:
        vector<YamlObject>& emailYamlObjects;
        vector<YamlObject>& anonymousYamlObjects;
        vector<vector<string>>& emailAdressesPerClass;

        map<string, string> studentIdsAndEmails; 
        map<string, string> emailsAndClasses;         
        map<string, string> studentIdsAndClasses;

        void writeOutYamlFile(ofstream& outfile, vector<YamlObject>& yamlObjects);
        void writeOutYamlObject(ofstream& outfile, YamlObject& object, size_t indentationLevel);

        vector<map<string, string>> mapStudentIdsToGradesAsKVs(vector<pair<string,string>>& numsAndGrades, map<string, string>& studentIdsAndEmails);
        map<string, string> mapStudentIdsToEmailsAdresses();
        map<string, string> mapEmailsToClasses();
        map<string, string> mapStudentIdsToClasses();
        
        pair<string, string> getClassAndEmailPair(string& emailAdress);        
        vector<string> getListOfMapValues(map<string, string>& map);
        void normalizeGrade(pair<string, string>& gradePair);
   
     public:
        Mapper(vector<YamlObject>& emailYaml, vector<YamlObject>& anonymousYaml, vector<vector<string>>& classEmails)
        : emailYamlObjects(emailYaml), anonymousYamlObjects(anonymousYaml), emailAdressesPerClass (classEmails) {}

        void createYamlFilesWithClasses();
        void createGradesYamlFile(ofstream& outfile); 

        void createYamlFileFromMap(map<string, string>& keysAndValues, string identifierKey);
};

#endif