#ifndef BASICS_H
#define BASICS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct AnalysisFilter {
  double timeBetweenAssignmentsThreshold = 0.0;

  AnalysisFilter();
  AnalysisFilter(double t) 
  {
    timeBetweenAssignmentsThreshold = t;
  };  
};

class BasicAnalyzer
{
    private:
      AnalysisFilter& filter;   
      bool isValidAssignmentTime(string previousTime, string currTime);
      map<string, vector<int>> getGradesWithSameClass(multimap<string, int> classesAndGrades);

    public:
      map<string, pair<int, int>> getAmountOfExercisesAndGradesStartedPerStudent();
      map<string, int> getAmountOfStartedExcersisesPerStudent();
      vector<pair<string, int>> getGradeAvgPerClass();

      BasicAnalyzer(AnalysisFilter& f) : filter(f) {}; 
};

#endif