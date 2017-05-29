#ifndef EXCERSISECOMPLETIONANDGRADECLUSTERING_H
#define EXCERSISECOMPLETIONANDGRADECLUSTERING_H

#include "IVisualization.h"
#include "../KMeans/CustomTypes/KMeansPoint.h"
#include "../Filter/GradePercentageFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"

#include <pqxx/pqxx>
#include <map>
#include <vector>

class ExcersiseCompletionAndGradesClustering : public IVisualization
{
private:
  std::shared_ptr<GradePercentageFilter> gradeFilter;
  std::shared_ptr<AssignmentIntervalFilter> assignmentIntervalFilter;

  std::vector<std::vector<KMeansPoint>> getExcersiseCompletionAndGradesClusters();
  std::map<std::string, std::pair<int, int>> getAmountOfExercisesCompletedAndGradesPerStudent();

  std::map<std::string, int> getAmountOfCompletedExcersisesPerStudent();

  std::map<std::string, std::pair<int, int>>
  createExcersiseCompletionAmountAndGradesPairs(std::vector<pqxx::result::tuple> filteredIdsAndGrades,
                                                std::map<std::string, int> excersiseAmountPerStudent);

public:
  std::string getVisualizationAsJSON();
  ExcersiseCompletionAndGradesClustering(std::shared_ptr<GradePercentageFilter> gf,
                                         std::shared_ptr<AssignmentIntervalFilter> af)
  {
    gradeFilter = gf;
    assignmentIntervalFilter = af;
  };
};

#endif
