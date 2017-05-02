#ifndef BASICS_H
#define BASICS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "AnalysisFilter.h"

using namespace std;

class BasicAnalyzer
{
    private:
      AnalysisFilter& filter;   
      map<string, vector<int>> getGradesWithSameClass(multimap<string, int> classesAndGrades);
      int getTotalAmountOfGrades();

    public:
      map<string, pair<int, int>> getAmountOfExercisesCompletedAndGradesPerStudent();
      map<string, int> getAmountOfCompletedExcersisesPerStudent();
      map<int, int> getGradesAndSuccessRates();
      vector<pair<string, int>> getGradeAvgPerClass();

      BasicAnalyzer(AnalysisFilter& f) : filter(f) {}; 
};

#endif