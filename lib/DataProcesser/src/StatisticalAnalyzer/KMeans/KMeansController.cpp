#include "KMeansController.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm> 

using namespace std;


//MUST BE ABLE TO BE CONVERTED TO FLOATS. RAISE EXCEPTION IF NOT?
void KMeansController::convertGradesAndExcersiseMapToPoints()
{    
    //maybe also add something so we can see studentnumbers without using them in the clustering
    map<string, pair<int, int>>::iterator it;
    size_t counter = 0;

    for(it=inputValues.begin(); it!=inputValues.end(); ++it)
    {
        vector<float> valuesForGV;
        valuesForGV.push_back((float) it->second.first);
        valuesForGV.push_back((float) it->second.second);

        GenericVector newGv (valuesForGV);
        
        Point newPoint (counter, newGv);
        points.push_back(newPoint);

        counter++;
    }
}

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

vector<vector<Point>> KMeansController::getClustersOfBestIteration()
{
    vector<vector<Point>> returnClusters;

    //TEST TEMP
    for(int i = 0; i <= clusterAmount; i++)
    {
        vector<Point> fillerVector;
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

vector<vector<Point>> KMeansController::getFinalNonEmptyClusters()
{
    vector<vector<Point>> nonEmptyClusters;

    for(vector<Point> cluster : finalClusters)
    {
        if(cluster.size() > 0)
            nonEmptyClusters.push_back(cluster);
    }
    return nonEmptyClusters;
}

