#include "KMeansController.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm> 

using namespace std;


KMeansIteration KMeansController::getIterationWithBestSSE()
{
    pair<int&, float> bestIterationIdAndSSE (iterations[0].iterationId, iterations[0].sumOfSquaredErrors);

    for (int i = 0; i < iterations.size(); i++)
    {
        if(iterations[i].sumOfSquaredErrors <  bestIterationIdAndSSE.second)
        {
            bestIterationIdAndSSE.first = iterations[i].iterationId;
            bestIterationIdAndSSE.second =  iterations[i].sumOfSquaredErrors;
        }
    }

    for(int j =0; j < iterations.size(); j++)
    {
        if(iterations[j].iterationId == bestIterationIdAndSSE.first)
            return iterations[j];            
    }
}


void KMeansController::run()
{
    int iterationsCount = 0;

    while(iterationsCount < algorithmIterationAmount)
    {
        KMeansIteration kmeansIteration(points, clusterAmount, iterationsCount, dimension);
        kmeansIteration.runIteration();

        iterations.push_back(kmeansIteration);
        iterationsCount++;
    }
    finalClusters = getClustersOfBestIteration();
}

vector<vector<ClusteringPoint>> KMeansController::getClustersOfBestIteration()
{
    vector<vector<ClusteringPoint>> returnClusters;

    for(int i = 0; i <= clusterAmount; i++)
    {
        vector<ClusteringPoint> fillerVector;
        returnClusters.push_back(fillerVector);
    }

    KMeansIteration bestIteration = getIterationWithBestSSE();
    
    for(auto p : bestIteration.points)
    {
        int clusterIndex = p.centroidId;
        returnClusters[clusterIndex].push_back(p);
    }
    return returnClusters;
}

vector<vector<ClusteringPoint>> KMeansController::getFinalNonEmptyClusters()
{
    vector<vector<ClusteringPoint>> nonEmptyClusters;

    for(vector<ClusteringPoint> cluster : finalClusters)
    {
        if(cluster.size() > 0)
            nonEmptyClusters.push_back(cluster);
    }
    return nonEmptyClusters;
}

