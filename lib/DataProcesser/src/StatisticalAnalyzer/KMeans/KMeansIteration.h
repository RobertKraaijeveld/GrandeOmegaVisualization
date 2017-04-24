#ifndef KMEANSITERATION_H
#define KMEANSITERATION_H

#include "CustomTypes/Point.h"
#include "CustomTypes/GenericVector.h"

class KMeansIteration {
    private:
        int dimension;
        int clusterAmount;

        vector<Centroid> centroids;        

        bool centroidsHaveChanged();
        vector<Centroid> createRandomCentroids();
        void reassignPointClusters();
        pair<Centroid, float> getClosestCentroidAndDistance(Point p);

        vector<Point> getPointsOfCluster(int centroidId);
        void recomputeCentroids();
        void recordVectorChange(GenericVector& oldCentroidVector, Centroid& centroid);

        float computeSSE();


    public:
        int iterationId;
        float sumOfSquaredErrors;

        vector<Point> &points;        

        KMeansIteration(vector<Point>& p, int& c, int& i, int& d) : points(p), clusterAmount(c),  iterationId(i), dimension(d) {};
        void runIteration();
};


#endif