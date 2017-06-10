#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H

#include "../IClassifier.h"
#include "../../../Utilities/Utilities.h"
#include "../../Point/IClusteringPoint.h"
#include "../../GenericVector/GenericVector.h"

#include <vector>
#include <map>
#include <memory>

class NaiveBayesClassification : public IClassifier
{
private:
  std::vector<std::shared_ptr<IClusteringPoint>> inputPoints;
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClusters;

  int getPredictedClusterForPoint(std::shared_ptr<IClusteringPoint>& subjectPoint);

  std::map<int, float> createNeighbourLikelihoodMap(std::shared_ptr<IClusteringPoint>& subjectPoint);
  std::map<int, float> createPriorProbabilityMap();

public:
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedPoints();

  void initialize(std::vector<std::shared_ptr<IClusteringPoint>> input, vector<vector<std::shared_ptr<IClusteringPoint>>> trainingset)
  {
    inputPoints = input;
    trainingClusters = trainingset;
  }
};

#endif