#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class CsvParser
{
    public:
      static vector<vector<string>> parseClassMatrix(ifstream& str);
      static vector<pair<string, string>> parseGradeFile(ifstream& str);
};

#endif