#ifndef BASICS_H
#define BASICS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../../YamlParser/YamlObject.h"

using namespace std;

class BasicAnalyses
{
    private:
      static map<string, vector<int>> getGradesWithSameClass(multimap<string, int> classesAndGrades);
    public:
      static vector<pair<string, int>> getGradeAvgPerClass();

};

#endif