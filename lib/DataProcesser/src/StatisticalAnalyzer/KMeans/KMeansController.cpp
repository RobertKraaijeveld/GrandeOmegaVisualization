#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>

#include "KMeansController.h"
#include "../../Utilities/Utilities.h"


using namespace std;

KMeansIteration KMeansController::getIterationWithBestSSE()
{
    pair<int &, float> bestIterationIdAndSSE(iterations[0].iterationId, iterations[0].sumOfSquaredErrors);

    for (int i = 0; i < iterations.size(); i++)
    {
        if (iterations[i].sumOfSquaredErrors < bestIterationIdAndSSE.second)
        {
            bestIterationIdAndSSE.first = iterations[i].iterationId;
            bestIterationIdAndSSE.second = iterations[i].sumOfSquaredErrors;
        }
    }

    for (int j = 0; j < iterations.size(); j++)
    {
        if (iterations[j].iterationId == bestIterationIdAndSSE.first)
            return iterations[j];
    }
}

void KMeansController::run()
{
    int iterationsCount = 0;

    while (iterationsCount < algorithmIterationAmount)
    {
        KMeansIteration kmeansIteration(points, clusterAmount, iterationsCount, dimension);

        kmeansIteration.runIteration();

        iterations.push_back(kmeansIteration);
        iterationsCount++;
    }
    finalClusters = getClustersOfBestIteration();
}

vector<vector<KMeansPoint>> KMeansController::getClustersOfBestIteration()
{
    vector<vector<KMeansPoint>> returnClusters;

    for (int i = 0; i <= clusterAmount; i++)
    {
        vector<KMeansPoint> fillerVector;
        returnClusters.push_back(fillerVector);
    }

    KMeansIteration bestIteration = getIterationWithBestSSE();

    for (auto p : bestIteration.points)
    {
        int clusterIndex = p.centroidId;
        returnClusters[clusterIndex].push_back(p);
    }
    return returnClusters;
}

vector<vector<shared_ptr<IClusteringPoint>>> KMeansController::getFinalNonEmptyClusters()
{
    vector<vector<shared_ptr<IClusteringPoint>>> nonEmptyClustersOfPtrs;

    int nonEmptyClusterCount = 0;
    for (size_t i = 0; i < finalClusters.size(); i++)
    {
        vector<KMeansPoint> currCluster = finalClusters[i];

        if (currCluster.size() > 0)
        {
            //ensures we dont get nullpointers when using clusterIds as indexes
            setPointCentroidIdsToClusterIndexes(nonEmptyClusterCount, currCluster);

            //bit of duplication with dbscan
            vector<shared_ptr<IClusteringPoint>> currClusterPtrs;
            for (size_t i = 0; i < currCluster.size(); i++)
            {
                //long live smart pointers, allowing us true polymorphism
                shared_ptr<IClusteringPoint> currKMeansPointPtr (new KMeansPoint(currCluster[i].getClusterId(), currCluster[i].getVector()));
                currClusterPtrs.push_back(currKMeansPointPtr);
            }

            nonEmptyClustersOfPtrs.push_back(currClusterPtrs);
            nonEmptyClusterCount++;
        }
    }
    return nonEmptyClustersOfPtrs;
}

void KMeansController::setPointCentroidIdsToClusterIndexes(int clusterIndex, vector<KMeansPoint> &pointsOfCluster)
{
    for (size_t i = 0; i < pointsOfCluster.size(); i++)
    {
        pointsOfCluster[i].setClusterId(clusterIndex);
    }
}