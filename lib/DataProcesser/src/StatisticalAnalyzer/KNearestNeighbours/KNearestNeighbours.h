#ifndef KNEARESTNEIGHBOURS_H
#define KNEARESTNEIGHBOURS_H

#include "../../Utilities/Utilities.h"
#include "../KMeans/CustomTypes/ClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <map>


class KNearestNeighbours
{
  private:
    int maxAmountOfNeighbours;
    std::vector<ClusteringPoint> inputPoints;
    std::vector<std::vector<ClusteringPoint>> trainingClusters;

    std::vector<ClusteringPoint> getNearestNeighbours(ClusteringPoint point);
    int getNewClusterIdByVote(ClusteringPoint& point, std::vector<ClusteringPoint>& nearestNeighbours);

  public:
    std::vector<std::vector<ClusteringPoint>> getClassifiedPoints();

    KNearestNeighbours(std::map<string, std::pair<int, int>> inputValues, vector<vector<ClusteringPoint>> tc, int K)
    {
        inputPoints = Utilities::convertMapOfPairsToPoints(inputValues);
        trainingClusters = tc;
        maxAmountOfNeighbours = K;
    }
};

#endif