#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include "YamlObject.h" 
#include "YamlParser.h" 
#include "../Utilities/Utilities.h" 
 
using namespace std; 


/*
  YamlParser
*/

vector<YamlObject> YamlParser::parseListOfFiles(string basefileStr)
{
	vector<YamlObject> returnVector;
    vector<string> assignmentFilesNames = Utilities::getListOfNumberedFilesForBaseFile(basefileStr);

    for(int i = 0; i < assignmentFilesNames.size(); i++)  
	{
        cout << "YAMLPARSER: Parsing yaml file with name " << assignmentFilesNames[i] << endl;
		file = ifstream(assignmentFilesNames[i]);
		vector<YamlObject> currentFileYaml = parseYaml();

		returnVector.reserve(returnVector.size() + currentFileYaml.size() ); // preallocate memory
		returnVector.insert(returnVector.end(), currentFileYaml.begin(), currentFileYaml.end() );
	}	
	return returnVector;
} 


vector<YamlObject> YamlParser::parseYaml()
{ 
    vector<YamlObject> allYamlObjects; 

    int objectCount = 0;      
    string line;
    YamlObject currentYamlObject; 

    while(getline(file, line))
    {
        if(isBeginOfFile(line) == false)
        {
            string nextLine = peekNextLine();                                            
            if(reachedNewYamlObject(nextLine, currentYamlObject) || atEndOfFile(nextLine)) 
            {
                parseLine(line, nextLine, currentYamlObject);                                
                allYamlObjects.push_back(currentYamlObject);
                currentYamlObject = YamlObject();
            }
            else
            {
                parseLine(line, nextLine, currentYamlObject);
            }
        }
    }
    return allYamlObjects;
}

void YamlParser::parseLine(string& cl, string& nextLine, YamlObject& currentYamlObject)
{
    string currentline = cl;
    stringstream currentLineStr (currentline);    
           
    if(isStartOfChildObject(currentline, nextLine))
    {
        //duplication with else
        pair<string, string> newKVPair = getKvForLine(currentLineStr);
        currentYamlObject.values.insert(newKVPair);

        currentYamlObject.children.insert(createNewChildPair(newKVPair));
    }
    else if(isChildObjectValue(currentline))
    {
        pair<string, string> childValues = getKvForLine(currentLineStr);
        string lastAddedChildObjectKey = getKeyOfLastChild(currentYamlObject.children);
        currentYamlObject.children[lastAddedChildObjectKey].values.insert(childValues);
    }
    else
    {
        pair<string, string> newKVPair = getKvForLine(currentLineStr);
        currentYamlObject.values.insert(newKVPair);                                 
    }
}



/*
  CHECKING
*/

bool YamlParser::isBeginOfFile(string& currentline)
{
    return currentline == "---";
}

//TODO this would also be triggered on single empty lines
bool YamlParser::atEndOfFile(string& nextLine)
{
    return all_of(nextLine.begin(),nextLine.end(),::isspace);
}

bool YamlParser::isStartOfChildObject(string& currentline, string& nextline)
{
    if(isChildObjectValue(nextline) && !(isChildObjectValue(currentline)))
        return true;
    else
        return false;
}

//recursively check for yamlObject childValues
bool YamlParser::isChildObjectValue(string& line)
{
    //get amount of spaces
    //if amount of spaces % 4 is 0 
    return Utilities::isNCharsWhiteSpace(4, line);
}

//not completely generic since it uses - id but what the hell; fuckoff, fix this you cunt
bool YamlParser::reachedNewYamlObject(string& nextline, YamlObject& currentObj)
{
    return nextline[0] == '-'  && currentObj.getIdentifier() != "";
}


/*
   CREATION
*/

pair<string, string> YamlParser::getKvForLine(stringstream& linestr)
{
    pair<string, string> returnpair("", "");
    string line;

    while(getline(linestr, line))
    {
        string key = line.substr(0, line.find(':'));
        string value = line.substr(line.find(':') + 1); 

        //Only remove whitespace on non-identifier keys
        if(key.find(YamlObject::identifierPattern) == string::npos)
            Utilities::removeStrWhiteSpace(key);

        //erasing the space after the colon
        value.erase(0,1);

        get<0>(returnpair) = key;
        get<1>(returnpair) = value;                
    }
    return returnpair;
}

pair<string, YamlObject> YamlParser::createNewChildPair(pair<string, string>& kvPairForLine)
{
    pair<string, YamlObject> newChildEntry = make_pair(kvPairForLine.first, YamlObject());
    return newChildEntry;
}


/*
   UTILITY
*/


string YamlParser::getKeyOfLastChild(map<string, YamlObject>& children)
{
    map<string, YamlObject>::reverse_iterator i = children.rbegin();
    return i->first;
} 

string YamlParser::peekNextLine()
{
    string nextLine;
    int positionBeforePeek = file.tellg();

    getline(file, nextLine);

    //reset file stream position
    file.seekg(positionBeforePeek, ios_base::beg);

    return nextLine;
}
