#include "KNearestNeighbours.h"
#include "../../Utilities/Utilities.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <limits>
#include <iostream>

std::vector<std::shared_ptr<IClusteringPoint>> KNearestNeighbours::getNearestNeighbours(std::shared_ptr<IClusteringPoint>& subjectPoint, int maxNeighboursAmount,
                                                                                        std::vector<std::vector<std::shared_ptr<IClusteringPoint>>>& clusters)
{
    std::map<std::shared_ptr<IClusteringPoint>, float> neighboursAndDistances;

    //used to exclude distances lower than the lowest recorded nearest neighbour distance
    float maximumDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < clusters.size(); i++)
    {
        vector<std::shared_ptr<IClusteringPoint>> currentTrainingCluster = clusters[i];

        for (size_t j = 0; j < currentTrainingCluster.size(); j++)
        {
            auto currTrainingPoint = currentTrainingCluster[j];

            GenericVector subjectsVector = (*subjectPoint).getVector();
            GenericVector currTrainingPointVector = (*currTrainingPoint).getVector();

            float currentDistanceToTrainingPoint = subjectsVector.getEuclidDistance(currTrainingPointVector);
            

            if (currentDistanceToTrainingPoint < maximumDistance)
            {
                if (neighboursAndDistances.size() < maxNeighboursAmount)
                {
                    neighboursAndDistances.insert(make_pair(currTrainingPoint, currentDistanceToTrainingPoint));
                }
                else
                {
                    bool getHighest = true;
                    float highestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).second;
                    std::shared_ptr<IClusteringPoint> neighbourWithHighestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).first;

                    if (currentDistanceToTrainingPoint < highestDistance)
                    {
                        neighboursAndDistances.erase(neighbourWithHighestDistance);
                        neighboursAndDistances.insert(make_pair(currTrainingPoint, currentDistanceToTrainingPoint));

                        //updating maximumDistance since the neighbours are now updated
                        maximumDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).second;
                    }
                }
            }

            if (neighboursAndDistances.size() == maxNeighboursAmount)
            {
                bool getHighest = true;
                float highestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).second;

                maximumDistance = highestDistance;
            }
        }
    }
    return Utilities::getKeysOfMap(neighboursAndDistances);
}



int KNearestNeighbours::getNewClusterIdByVote(std::shared_ptr<IClusteringPoint> point, std::vector<std::shared_ptr<IClusteringPoint>> nearestNeighbours)
{
    map<int, int> clusterIdsCounts;

    for (size_t i = 0; i < nearestNeighbours.size(); i++)
    {
        std::shared_ptr<IClusteringPoint> currentNeighbour = nearestNeighbours[i];
        clusterIdsCounts[currentNeighbour->getClusterId()] += 1;
    }

    //bit of a waste
    bool getHighest = true;
    int mostCommonNeighbourClusterId = Utilities::getHighestOrLowestValueKV(clusterIdsCounts, getHighest).first;

    return mostCommonNeighbourClusterId;
} 

std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> KNearestNeighbours::getClassifiedPoints()
{
    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> resultingClassifiedPoints;

    //Prepping resultingClassifiedPoints 2d vector
    for (size_t i = 0; i < trainingClusters.size(); i++)
    {
        std::vector<std::shared_ptr<IClusteringPoint>> newEmptyCluster;
        resultingClassifiedPoints.push_back(newEmptyCluster);
    }

    for (std::shared_ptr<IClusteringPoint> currPoint : inputPoints)
    {
        std::vector<std::shared_ptr<IClusteringPoint>> nearestNeighbours = KNearestNeighbours::getNearestNeighbours(currPoint, maxAmountOfNeighbours, trainingClusters);
        
        int newClusterIndexForThisPoint = getNewClusterIdByVote(currPoint, nearestNeighbours);
        resultingClassifiedPoints[newClusterIndexForThisPoint].push_back(currPoint);
    }
    return resultingClassifiedPoints;
}