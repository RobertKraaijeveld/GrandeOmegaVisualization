#ifndef KmeansPoint_H
#define KmeansPoint_H

#include "IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include <map>
#include <numeric>

struct Centroid {
    int id; 
    bool hasChanged = true;
    GenericVector vector;

    Centroid(){ id = -1; vector = GenericVector(); };
    Centroid(int i, GenericVector gv)
    {
        id = i;
        vector = gv;
    };
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

        KMeansPoint(int cId, GenericVector gv)
        {
            centroidId = cId;
            vector = gv;
        }
};

#endif
