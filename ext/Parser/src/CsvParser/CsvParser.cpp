#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <tuple> 
#include <algorithm>
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include "CsvParser.h"
#include "../Utilities/Utilities.h"


vector<vector<string>> CsvParser::parseClassMatrix(ifstream& fileStr)
{ 
    vector<vector<string>> allAdressesInClassOrder;
    
    string line;

    while( getline(fileStr, line) )
    {
        stringstream linestr (line);
        string cell;

        int cellCount = 0;
        vector<string> adressesForThisClass;

        while(getline(linestr, cell, ';'))
        {
            cellCount++;

            //we skip the first cell since it is the class letter  
            if(cellCount != 1 && !(all_of(cell.begin(),cell.end(), ::isspace)))
            {
                cell.erase(remove(cell.begin(), cell.end(), '\"' ), cell.end());                
                Utilities::removeStrWhiteSpace(cell);

                adressesForThisClass.push_back(cell);             
            }
        }
        allAdressesInClassOrder.push_back(adressesForThisClass);
    }
    return allAdressesInClassOrder;  
}

vector<pair<string, string>> CsvParser::parseGradeFile(ifstream& str)
{
    vector<pair<string, string>> numberAndGradePairs;
    
    int lineCount = 0; 
    string line;

    while( getline(str, line) )
    {
        lineCount++;        

        //first line is headers
        if(lineCount == 0)
            continue; 

        stringstream linestr (line);
        string cell;

        int cellCount = 0;
        pair<string, string> numberAndGrade = make_pair("", "");

        while( getline(linestr, cell, ',') )
        {
            if(!(all_of(cell.begin(),cell.end(), ::isspace)))
            {
                cell.erase(remove(cell.begin(), cell.end(), '\"' ), cell.end());                
                Utilities::removeStrWhiteSpace(cell);

                if(cellCount == 0)  
                    numberAndGrade.first = cell;
                else
                    numberAndGrade.second = cell;                    
            }
            cellCount++;
        }
        numberAndGradePairs.push_back(numberAndGrade);
    }
    str.close();
    return numberAndGradePairs;  
}



