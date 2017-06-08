#ifndef WeekDayExcersiseCompletionAndGradesClassification_H
#define WeekDayExcersiseCompletionAndGradesClassification_H

#include "ExcersiseCompletionAndGradesClustering.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../KNearestNeighbours/KNearestNeighbours.h"
#include "../KMeans/KMeansController.h"
#include "../KMeans/CustomTypes/KMeansPoint.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>

class WeekDayExcersiseCompletionAndGradesClassification : public IVisualization
{
private:
  std::shared_ptr<IFilter> gradeFilter;
  std::shared_ptr<IFilter> assignmentIntervalFilter;
  std::shared_ptr<ITimeFilter> timeFilter;
  bool filterOnWeekendOnly;

  std::vector<std::vector<KMeansPoint*>> getTrainingData();
  std::vector<std::vector<IClusteringPoint*>> getClassifiedData();

public:
  std::string getVisualizationAsJSON();
  WeekDayExcersiseCompletionAndGradesClassification(std::shared_ptr<IFilter> gf,
                                                    std::shared_ptr<IFilter> af,
                                                    std::shared_ptr<ITimeFilter> df,
                                                    bool filterOnWeekendOnly
                                                    )
  {
    gradeFilter = gf;
    assignmentIntervalFilter = af;
    timeFilter = df;
    this->filterOnWeekendOnly = filterOnWeekendOnly;
  };
};

#endif
