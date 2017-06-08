#ifndef ClusteringPoint_H
#define ClusteringPoint_H

#include "../../Point/IClusteringPoint.h"
#include "../../GenericVector/GenericVector.h"
#include <map>
#include <numeric>

struct Centroid {
    int id; 
    bool hasChanged = true;
    GenericVector vector;

    Centroid(int id, GenericVector vector);
    Centroid(){ id = -1; vector = GenericVector(); };
};


class KMeansPoint : public IClusteringPoint
{  
    public:
        GenericVector vector; 
        int centroidId;
        double distanceToCentroid;

        GenericVector getVector() const { return vector; };
        int getClusterId() const { return centroidId; };

        void setVector(GenericVector newVector) { vector = newVector; };
        void setClusterId(int newCentroidId) { centroidId = newCentroidId; };


        KMeansPoint(){};
        KMeansPoint(int cId, GenericVector vector);
};

#endif
