#include "KMeansIteration.h"
#include "CustomTypes/ClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include <iostream>
#include <limits>
#include <math.h>
#include <map>

using namespace std;

//shitty param, get rid of it
bool KMeansIteration::centroidsHaveChanged()
{
    for(int i = 0; i < clusterAmount; i++)
    {
        if(centroids[i].hasChanged)
            return true;   
    }    
    return false;
}

vector<Centroid> KMeansIteration::createRandomCentroids()
{
    //todo: make this dynamic? YOU IDIOT
    vector<Centroid> returnVector;

    for(int i = 0; i < clusterAmount; i++)
    {
        GenericVector randomGv = GenericVector::getRandomVector(dimension);
        returnVector.push_back(Centroid(i, randomGv));
    }
    return returnVector;
}

void KMeansIteration::reassignPointClusters()
{
    for(int i = 0; i < points.size(); i++)
    {
        pair<Centroid, float> closestCentroidAndDistance = getClosestCentroidAndDistance(points[i]);  
        points[i].centroidId = closestCentroidAndDistance.first.id;
        points[i].distanceToCentroid = closestCentroidAndDistance.second;
    }
}

pair<Centroid, float> KMeansIteration::getClosestCentroidAndDistance(ClusteringPoint p)
{
    //duplicated in kmeanscontroller
    float MAX_float = 1.7976931348623158e+308;
    Centroid closestCentroid;
    pair<Centroid, float> closestCentroidAndDistance = make_pair(closestCentroid, MAX_float);

    for(int i = 0; i < centroids.size(); i++)
    {
        float distance = p.vector.getEuclidDistance(centroids[i].vector); 

        if(distance < closestCentroidAndDistance.second)
            closestCentroidAndDistance = make_pair(centroids[i], distance);                        
            
    }
    return closestCentroidAndDistance;
}


void KMeansIteration::recomputeCentroids()
{
    //sum all points within a cluster, divide the resulting vector by the amount of points.
    for(int i = 0; i < centroids.size(); i++)
    {
        GenericVector oldVector = centroids[i].vector;
        vector<ClusteringPoint> pointsForThisCluster = KMeansIteration::getPointsOfCluster(centroids[i].id);

        for(int j = 0; j < pointsForThisCluster.size(); j++)
        {
            GenericVector& vectorForPoint = pointsForThisCluster[j].vector;
            centroids[i].vector.sumWith(vectorForPoint);            
        }
        centroids[i].vector.divide(pointsForThisCluster.size());      

        recordVectorChange(oldVector, centroids[i]);
    }
}

vector<ClusteringPoint> KMeansIteration::getPointsOfCluster(int centroidId)
{
    vector<ClusteringPoint> pointsForCluster;
    for(int i = 0; i < points.size(); i++)
    {
       if(points[i].centroidId == centroidId)
       {
           pointsForCluster.push_back(points[i]);
       }
    }
    return pointsForCluster;
}

void KMeansIteration::recordVectorChange(GenericVector& oldCentroidVector, Centroid& centroid)
{
    centroid.hasChanged = oldCentroidVector.isNotEqual(centroid.vector);
}  



/*
REPORTING RESULTS
*/

float KMeansIteration::computeSSE()
{
    float SSE = 0;
    for(int i = 0; i < points.size(); i++)
    {
        SSE += powf(points[i].distanceToCentroid, 2.0);
    }
    return SSE;
}


void KMeansIteration::runIteration()
{
    centroids = createRandomCentroids(); 
    
    int count = 0;
    while(count < 25)
    {
        reassignPointClusters();    
        recomputeCentroids();
        count++;

        if(centroidsHaveChanged() == false)
            break;
    }
    sumOfSquaredErrors = computeSSE();
}
