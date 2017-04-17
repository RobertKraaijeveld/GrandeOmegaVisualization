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
#include "../Utilities/Utilities.h" 
 
using namespace std; 


string YamlObject::identifierPattern = "- ";

string YamlObject::getIdentifier()
{
    map<string, string>::iterator valuesIterator;
 
	for(valuesIterator = values.begin(); valuesIterator != values.end(); valuesIterator++)
	{
        string key = valuesIterator->first; 
        if(key[0] == '-' && key[1] == ' ')
            return key;
    }
    return "";
}