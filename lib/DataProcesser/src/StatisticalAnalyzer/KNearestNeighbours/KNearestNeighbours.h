#ifndef KNEARESTNEIGHBOURS_H
#define KNEARESTNEIGHBOURS_H

#include "../../Utilities/Utilities.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <map>


class KNearestNeighbours
{
  private:
    int maxAmountOfNeighbours;
    std::vector<IClusteringPoint*> inputPoints;
    std::vector<std::vector<KMeansPoint*>> trainingClusters;

    std::vector<KMeansPoint*> getNearestNeighbours(IClusteringPoint* point);
    int getNewClusterIdByVote(IClusteringPoint* point, std::vector<KMeansPoint*> nearestNeighbours);

  public:
    std::vector<std::vector<IClusteringPoint*>> getClassifiedPoints();

    KNearestNeighbours(std::vector<IClusteringPoint*> input, vector<vector<KMeansPoint*>> tc, int K)
    {
        inputPoints = input;
        trainingClusters = tc;
        maxAmountOfNeighbours = K;
    }
};

#endif