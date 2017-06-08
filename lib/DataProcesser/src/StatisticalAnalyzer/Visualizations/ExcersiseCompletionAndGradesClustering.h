#ifndef EXCERSISECOMPLETIONANDGRADECLUSTERING_H
#define EXCERSISECOMPLETIONANDGRADECLUSTERING_H

#include "IVisualization.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../KMeans/CustomTypes/KMeansPoint.h"
#include "../Filter/GradePercentageFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"

#include <pqxx/pqxx>
#include <map>
#include <vector>

class ExcersiseCompletionAndGradesClustering : public IVisualization
{
private:
  std::shared_ptr<IFilter> gradeFilter;
  std::shared_ptr<IFilter> assignmentIntervalFilter;
  std::shared_ptr<ITimeFilter> timeFilter;

  std::map<std::string, int> getAmountOfCompletedExcersisesPerStudent();

  std::map<std::string, std::pair<int, int>>
  createExcersiseCompletionAmountAndGradesPairs(std::vector<pqxx::result::tuple> filteredIdsAndGrades,
                                                std::map<std::string, int> excersiseAmountPerStudent);

public:
  virtual std::string getVisualizationAsJSON();

  //Handy for other clusterers, should really have its own' class
  //std::vector<std::vector<IClusteringPoint*>> getExcersiseCompletionAndGradesClusters();
  std::vector<std::vector<KMeansPoint*>> getExcersiseCompletionAndGradesClusters();
  
  std::map<std::string, std::pair<int, int>> getAmountOfExercisesCompletedAndGradesPerStudent();

  ExcersiseCompletionAndGradesClustering(std::shared_ptr<IFilter> gf,
                                         std::shared_ptr<IFilter> af,
                                         std::shared_ptr<ITimeFilter> df)
  {
    gradeFilter = gf;
    assignmentIntervalFilter = af;
    timeFilter = df;
  };
};

#endif
