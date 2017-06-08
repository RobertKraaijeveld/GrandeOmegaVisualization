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

vector<vector<KMeansPoint*>> KMeansController::getFinalNonEmptyClusters()
{
    vector<vector<KMeansPoint*>> nonEmptyClustersOfPtrs;

    int nonEmptyClusterCount = 0;
    for (size_t i = 0; i < finalClusters.size(); i++)
    {
        vector<KMeansPoint> currCluster = finalClusters[i];

        if (currCluster.size() > 0)
        {
            //ensures we dont get nullpointers when using clusterIds as indexes
            setPointCentroidIdsToClusterIndexes(nonEmptyClusterCount, currCluster);

            std::vector<KMeansPoint*> currClusterPtrs;
            for (size_t i = 0; i < currCluster.size(); i++)
            {
                auto currKMeansPointPtr = new KMeansPoint(currCluster[i].getClusterId(), currCluster[i].getVector());
                currClusterPtrs.push_back(currKMeansPointPtr);
            }

            for (size_t i = 0; i < currClusterPtrs.size(); i++)
            {
                std::cout << "0th level " << (*currClusterPtrs[i]).getClusterId() << endl;
            }            

            nonEmptyClustersOfPtrs.push_back(currClusterPtrs);
            nonEmptyClusterCount++;
        }
    }

    //moar test
    vector<vector<KMeansPoint*>> xPtrs;

    KMeansPoint* randomPoint  = new KMeansPoint(1, GenericVector::getRandomVector(2));
    vector<KMeansPoint*> xptr;

    xptr.push_back(randomPoint);

    xPtrs.push_back(xptr);

    for (size_t i = 0; i < xPtrs[0].size(); i++)
        std::cout << "xptrs test: " << (*xPtrs[0][i]).getClusterId() << endl;


    //WHYYYYYYYYYYYYY
    for (size_t i = 0; i < nonEmptyClustersOfPtrs[0].size(); i++)
        {
                std::cout << "1st level " << (*nonEmptyClustersOfPtrs[0][i]).getClusterId() << endl;
            } 
    
    return nonEmptyClustersOfPtrs;
}

void KMeansController::setPointCentroidIdsToClusterIndexes(int clusterIndex, vector<KMeansPoint> &pointsOfCluster)
{
    for (size_t i = 0; i < pointsOfCluster.size(); i++)
    {
        pointsOfCluster[i].centroidId = clusterIndex;
    }
}