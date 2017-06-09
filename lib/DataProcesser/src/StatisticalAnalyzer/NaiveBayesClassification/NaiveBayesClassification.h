#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H

#include "../../Utilities/Utilities.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <map>
#include <memory>

class NaiveBayesClassification
{
private:

  std::vector<std::shared_ptr<IClusteringPoint>> inputPoints;
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClusters;

  int getPredictedClusterForPoint(std::shared_ptr<IClusteringPoint>& subjectPoint);

  std::map<int, float> createNeighbourLikelihoodMap(std::shared_ptr<IClusteringPoint>& subjectPoint);
  std::map<int, float> createPriorProbabilityMap();

public:
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedPoints();

  NaiveBayesClassification(std::vector<std::shared_ptr<IClusteringPoint>> input, vector<vector<std::shared_ptr<IClusteringPoint>>> trainingset)
  {
    inputPoints = input;
    trainingClusters = trainingset;
  }
};

#endif