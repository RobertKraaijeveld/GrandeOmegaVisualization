#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include "YamlObject.h"
#include "YamlConverter.h" 
#include "../Utilities/Utilities.h" 
 
using namespace std;

YamlObject YamlConverter::convertToBasicYamlObject(map<string, string>& m)
{
    YamlObject returnObject;
    map<string, string>::iterator iter;

    for(iter = m.begin(); iter != m.end(); iter++)
    {
        if(iter->first == preferredIdentifier)
		{
			string newYamlObjectKey = YamlObject::identifierPattern + iter->first;
            returnObject.values.insert(make_pair(newYamlObjectKey, iter->second));
		}
        else
            returnObject.values.insert(make_pair(iter->first, iter->second));
    }
	return returnObject;
}

void YamlConverter::writeOutYamlFile(ofstream& outfile, vector<YamlObject>& yamlObjects)
{ 
	outfile << "---" << endl;
	size_t initialIndentationLevel = 2;

	for(int i = 0; i < yamlObjects.size(); i++)
	{
		writeOutYamlObject(outfile, yamlObjects[i], initialIndentationLevel);
	}
	outfile.close();

	cout << "Done writing to file..." << endl;
}
 
void YamlConverter::writeOutYamlObject(ofstream& outfile, YamlObject& object, size_t indentationLevel)
{
	map<string, string>::iterator valuesIterator;
 
	for(valuesIterator = object.values.begin(); valuesIterator != object.values.end(); valuesIterator++)
	{
		if(valuesIterator->first.find(YamlObject::identifierPattern) != string::npos)
		{
			outfile << valuesIterator->first << ": " << valuesIterator->second << endl;
		}
		else if(valuesIterator->first == "student_id" && generateClassHashes)
		{
			outfile << Utilities::createStrWithWhiteSpaces(indentationLevel) << valuesIterator->first << ": " << valuesIterator->second << endl;
			outfile <<  "  class: " << studentIdsAndClasses[valuesIterator->second] << endl;				
		}											
		else
		{
			outfile << Utilities::createStrWithWhiteSpaces(indentationLevel) << valuesIterator->first << ": " << valuesIterator->second << endl;											
			
			if(object.children.count(valuesIterator->first))
				writeOutYamlObject(outfile, object.children[valuesIterator->first], indentationLevel + 2);								
		}
	}
}
