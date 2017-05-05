#ifndef BASICS_H
#define BASICS_H

#include <vector>
#include <string>
#include <pqxx/pqxx>
#include <fstream>
#include <iostream>

#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "AnalysisFilter.h"

using namespace std;

class BasicAnalyzer
{
    private:
      AnalysisFilter filter;   

      //TODO All of these using mktime() and my own UtcTime
      map<string, int> getExcersisesInWeekDaysAmount();
      map<string, int> getExcersisesInWeekendAmount();
      map<string, int> getExcersisesInDayAmount();                  
      map<string, int> getExcersisesInNightAmount();                  

    public:
      map<string, int> getExceriseDateTimeMeasurements();
      map<string, pair<int, int>> getAmountOfExercisesCompletedAndGradesPerStudent();
      map<string, int> getAmountOfCompletedExcersisesPerStudent();
      map<string, pair<int, int>> getGradesAndSuccessRates();
      vector<pair<string, int>> getGradeAvgPerClass();

      BasicAnalyzer(AnalysisFilter f) { filter = f; }; 
};

#endif