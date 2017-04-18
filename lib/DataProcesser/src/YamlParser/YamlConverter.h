#ifndef YAMLCONVERTER_H
#define YAMLCONVERTER_H

#include <vector>
#include "YamlObject.h" 


class YamlConverter {
    public:
        map<string, string> studentIdsAndClasses;
        bool generateClassHashes;
        string preferredIdentifier = "- id";

        YamlObject convertToBasicYamlObject(map<string, string>& m);
        void writeOutYamlFile(ofstream& outfile, vector<YamlObject>& yamlObjects);

    private:
        void writeOutYamlObject(ofstream& outfile, YamlObject& object, size_t indentationLevel);
    

};

#endif