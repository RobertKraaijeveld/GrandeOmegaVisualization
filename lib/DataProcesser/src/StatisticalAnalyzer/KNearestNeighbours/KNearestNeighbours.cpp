#include "KNearestNeighbours.h"
#include "../../Utilities/Utilities.h"
#include "../KMeans/CustomTypes/ClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <limits>

std::vector<ClusteringPoint> KNearestNeighbours::getNearestNeighbours(ClusteringPoint subjectPoint)
{
    std::map<ClusteringPoint, float> neighboursAndDistances;

    //used to exclude distances lower than the lowest recorded nearest neighbour distance
    float maximumDistance = std::numeric_limits<float>::max();


    for (size_t i = 0; i < trainingClusters.size(); i++)
    {
        vector<ClusteringPoint> &currentTrainingCluster = trainingClusters[i];
        for (size_t j = 0; j < currentTrainingCluster.size(); j++)
        {
            ClusteringPoint currTrainingPoint = currentTrainingCluster[j];
            float currentDistanceToTrainingPoint = subjectPoint.vector.getEuclidDistance(currTrainingPoint.vector);

            if (currentDistanceToTrainingPoint < maximumDistance)
            {
                if (neighboursAndDistances.size() < maxAmountOfNeighbours)
                {
                    neighboursAndDistances.insert(make_pair(currTrainingPoint, currentDistanceToTrainingPoint));
                }
                else
                {
                    bool getHighest = true;
                    float highestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).second;
                    ClusteringPoint neighbourWithHighestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).first;

                    if(currentDistanceToTrainingPoint > highestDistance)
                    {
                        neighboursAndDistances.erase(neighbourWithHighestDistance);
                        neighboursAndDistances.insert({currTrainingPoint, currentDistanceToTrainingPoint});

                        //updating maximumDistance since the neighbours are now updated
                        maximumDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).second;
                    }
                }
            }

            if (neighboursAndDistances.size() == maxAmountOfNeighbours)
            {
                bool getHighest = true;
                float highestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).second;
                maximumDistance = highestDistance;
            }
        }
    }
    return Utilities::getKeysOfMap(neighboursAndDistances);
}

int KNearestNeighbours::getNewClusterIdByVote(ClusteringPoint &point, std::vector<ClusteringPoint> &nearestNeighbours)
{
    map<int, int> clusterIdsCounts;

    for (size_t i = 0; i < nearestNeighbours.size(); i++)
    {
        ClusteringPoint &currentNeighbour = nearestNeighbours[i];
        clusterIdsCounts[currentNeighbour.centroidId] += 1;
    }

    //bit of a waste
    bool getHighest = true;
    int mostCommonNeighbourClusterId = Utilities::getHighestOrLowestValueKV(clusterIdsCounts, getHighest).first;

    return clusterIdsCounts[mostCommonNeighbourClusterId];
}

std::vector<std::vector<ClusteringPoint>> KNearestNeighbours::getClassifiedPoints()
{
    //memorywaste
    std::vector<std::vector<ClusteringPoint>> resultingClassifiedPoints = trainingClusters;

    for (ClusteringPoint &currPoint : inputPoints)
    {
        //resultingClassifiedPoints
        std::vector<ClusteringPoint> nearestNeighbours = getNearestNeighbours(currPoint);
        int newClusterIndexForThisPoint = getNewClusterIdByVote(currPoint, nearestNeighbours);

        resultingClassifiedPoints[newClusterIndexForThisPoint].push_back(currPoint);
    }
    return resultingClassifiedPoints;
}