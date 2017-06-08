#include "KNearestNeighbours.h"
#include "../../Utilities/Utilities.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

#include <vector>
#include <limits>
#include <iostream>

std::vector<IClusteringPoint*> KNearestNeighbours::getNearestNeighbours(IClusteringPoint* subjectPoint)
{
    std::map<IClusteringPoint*, float> neighboursAndDistances;

    //used to exclude distances lower than the lowest recorded nearest neighbour distance
    float maximumDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < trainingClusters.size(); i++)
    {
        vector<IClusteringPoint*> currentTrainingCluster = trainingClusters[i];
        cout << "trainingCluster " << i << " has size " << currentTrainingCluster.size() << endl;


        for (size_t z = 0; z < currentTrainingCluster.size(); z++)
        {
            std::cout << currentTrainingCluster[z]->x() << endl;
        }

        for (size_t j = 0; j < currentTrainingCluster.size(); j++)
        {
            auto currTrainingPoint = currentTrainingCluster[j];

            GenericVector subjectsVector = subjectPoint->getVector();
            std::cout << "subjectsVector = " << subjectsVector.ToString() << endl;

            GenericVector currTrainingPointVector = currTrainingPoint->getVector();
            std::cout << "currTrainingPointVector = " << currTrainingPointVector.ToString() << endl;

            float currentDistanceToTrainingPoint = subjectsVector.getEuclidDistance(currTrainingPointVector);

            std::cout << "currentDistanceToTrainingPoint = " << currentDistanceToTrainingPoint << endl;
            

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
                    IClusteringPoint* neighbourWithHighestDistance = Utilities::getHighestOrLowestValueKV(neighboursAndDistances, getHighest).first;

                    if (currentDistanceToTrainingPoint < highestDistance)
                    {
                        neighboursAndDistances.erase(neighbourWithHighestDistance);
                        neighboursAndDistances.insert(make_pair(currTrainingPoint, currentDistanceToTrainingPoint));

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

int KNearestNeighbours::getNewClusterIdByVote(IClusteringPoint* point, std::vector<IClusteringPoint*> nearestNeighbours)
{
    map<int, int> clusterIdsCounts;

    for (size_t i = 0; i < nearestNeighbours.size(); i++)
    {
        IClusteringPoint* currentNeighbour = nearestNeighbours[i];
        clusterIdsCounts[currentNeighbour->getClusterId()] += 1;
    }

    //bit of a waste
    bool getHighest = true;
    int mostCommonNeighbourClusterId = Utilities::getHighestOrLowestValueKV(clusterIdsCounts, getHighest).first;

    return mostCommonNeighbourClusterId;
}

std::vector<std::vector<IClusteringPoint*>> KNearestNeighbours::getClassifiedPoints()
{
    std::vector<std::vector<IClusteringPoint*>> resultingClassifiedPoints;

    for (size_t i = 0; i < trainingClusters.size(); i++)
    {
        std::vector<IClusteringPoint*> newEmptyCluster;
        resultingClassifiedPoints.push_back(newEmptyCluster);
    }
    std::cout << "Set trainingsset" << endl;
    

    for (IClusteringPoint* currPoint : inputPoints)
    {
        std::cout << "for" << endl;

        std::vector<IClusteringPoint*> nearestNeighbours = getNearestNeighbours(currPoint);
        std::cout << "got nn" << endl;        

        int newClusterIndexForThisPoint = getNewClusterIdByVote(currPoint, nearestNeighbours);
        resultingClassifiedPoints[newClusterIndexForThisPoint].push_back(currPoint);
    }
    return resultingClassifiedPoints;
}