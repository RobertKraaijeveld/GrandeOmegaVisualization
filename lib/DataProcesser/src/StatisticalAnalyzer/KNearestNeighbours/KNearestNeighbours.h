#ifndef KNEARESTNEIGHBOURS_H
#define KNEARESTNEIGHBOURS_H

#include "../../Utilities/Utilities.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <map>
#include <memory>

class KNearestNeighbours
{
private:
  int maxAmountOfNeighbours;
  std::vector<std::shared_ptr<IClusteringPoint>> inputPoints;
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClusters;

  std::vector<std::shared_ptr<IClusteringPoint>> getNearestNeighbours(std::shared_ptr<IClusteringPoint>point);
  int getNewClusterIdByVote(std::shared_ptr<IClusteringPoint>point, std::vector<std::shared_ptr<IClusteringPoint>> nearestNeighbours);

public:
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedPoints();

  KNearestNeighbours(std::vector<std::shared_ptr<IClusteringPoint>> input, vector<vector<std::shared_ptr<IClusteringPoint>>> tc, int K)
  {
    inputPoints = input;
    trainingClusters = tc;
    maxAmountOfNeighbours = K;
  }
};

#endif