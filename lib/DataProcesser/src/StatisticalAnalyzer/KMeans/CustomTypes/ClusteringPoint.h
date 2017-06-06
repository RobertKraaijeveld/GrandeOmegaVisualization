#ifndef ClusteringPoint_H
#define ClusteringPoint_H

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


class ClusteringPoint {
    public:
        int centroidId;
        GenericVector vector;
        double distanceToCentroid;

        ClusteringPoint(){};
        ClusteringPoint(int cId, GenericVector vector);
        
        //bit useless, but necessary to use this in a map
        bool operator<( const ClusteringPoint& other) const
        {
            float myVectorSum = std::accumulate(vector.values.begin(), vector.values.end(), 0.0);
            float otherVectorSum = std::accumulate(other.vector.values.begin(), other.vector.values.end(), 0.0);
            
            return myVectorSum < otherVectorSum;
        }
};

#endif
