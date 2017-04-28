#include "Utilities.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
 

using namespace std;


vector<string> Utilities::toArrayByDelim(string& s, char delim)
{
    vector<string> result;
    stringstream sstr(s);

    string token;
    while(getline(sstr, token, delim))
    {
        result.push_back(token);        
    }
    return result;
}

vector<string> Utilities::getListOfNumberedFilesForBaseFile(string baseFileString)
{
    vector<string> returnVector;

	for(int i = 1; i < 66; i++)
	{
		returnVector.push_back(baseFileString + to_string(i) + ".yaml");
    }
    return returnVector;
}

void Utilities::removeYamlReferences(string& str)
{
    //TODO: Make constants out of these magic numbers. Also, decide whether to remove the first value whitespaces or not
    str.erase(0, str.find(" 2"));
}

string Utilities::createStrWithWhiteSpaces(int amount)
{
    size_t size = amount; 
    return string(size, ' ');
}

void Utilities::removeStrWhiteSpace(string& str)
{
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}


bool Utilities::isNCharsWhiteSpace(int N, string& str)
{
    for (int i = 0; i < N - 1; i++)
    {
        if(!isspace(str[i]))
            return false;
    }
    return true;
}

int Utilities::computeAverage(vector<int>& values)
{
    int totalValues = 0; 
    for(auto v: values)
    {
        totalValues += v;
    }
    
    if(values.size() > 0 )
        return totalValues / values.size();
    else
        return totalValues / 1;
}

bool Utilities::isInVector(string& value, vector<string>& vector)
{
    return find(vector.begin(), vector.end(), value) != vector.end();
}