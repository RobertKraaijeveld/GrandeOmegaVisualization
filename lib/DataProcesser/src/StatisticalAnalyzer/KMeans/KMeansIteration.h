#ifndef KMEANSITERATION_H
#define KMEANSITERATION_H

#include "CustomTypes/ClusteringPoint.h"
#include "../GenericVector/GenericVector.h"

class KMeansIteration {
    private:
        int dimension;
        int clusterAmount;

        vector<Centroid> centroids;        

        bool centroidsHaveChanged();
        vector<Centroid> createRandomCentroids();
        void reassignPointClusters();
        pair<Centroid, float> getClosestCentroidAndDistance(ClusteringPoint p);

        vector<ClusteringPoint> getPointsOfCluster(int centroidId);
        void recomputeCentroids();
        void recordVectorChange(GenericVector& oldCentroidVector, Centroid& centroid);

        float computeSSE();


    public:
        int iterationId;
        float sumOfSquaredErrors;

        vector<ClusteringPoint> &points;        

        KMeansIteration(vector<ClusteringPoint>& p, int& c, int& i, int& d) : points(p), clusterAmount(c),  iterationId(i), dimension(d) {};
        void runIteration();
};


#endif