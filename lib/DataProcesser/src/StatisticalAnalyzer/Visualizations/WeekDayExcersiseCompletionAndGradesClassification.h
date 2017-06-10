#ifndef WeekDayExcersiseCompletionAndGradesClassification_H
#define WeekDayExcersiseCompletionAndGradesClassification_H

#include "IClassificationVisualization.h"
#include "ExcersiseCompletionAndGradesClustering.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../Classifiers/KNearestNeighbours/KNearestNeighbours.h"
#include "../KMeans/KMeansController.h"
#include "../Point/IClusteringPoint.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>

class WeekDayExcersiseCompletionAndGradesClassification : public IClassificationVisualization
{
private:
  std::shared_ptr<IClassifier> classifier;
  std::shared_ptr<IFilter> gradeFilter;
  std::shared_ptr<IFilter> assignmentIntervalFilter;
  std::shared_ptr<ITimeFilter> timeFilter;
  bool filterOnWeekendOnly;

  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getTrainingData();
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedData();

public:
  WeekDayExcersiseCompletionAndGradesClassification(std::shared_ptr<IClassifier> cl,
                                                    std::shared_ptr<IFilter> gf,
                                                    std::shared_ptr<IFilter> af,
                                                    std::shared_ptr<ITimeFilter> df,
                                                    bool filterOnWeekendOnly
                                                    )
  {
    classifier = cl;
    gradeFilter = gf;
    assignmentIntervalFilter = af;
    timeFilter = df;
    this->filterOnWeekendOnly = filterOnWeekendOnly;
  };
};

#endif
