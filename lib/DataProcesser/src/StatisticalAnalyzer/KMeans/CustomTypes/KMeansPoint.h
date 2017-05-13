#ifndef KMEANSPOINT_H
#define KMEANSPOINT_H

#include "../../GenericVector/GenericVector.h"
#include <map>

struct Centroid {
    int id; 
    bool hasChanged = true;
    GenericVector vector;

    Centroid(int id, GenericVector vector);
    Centroid(){ id = -1; vector = GenericVector(); };
};


class KMeansPoint {
    public:
        KMeansPoint(int cId, GenericVector vector);
        int centroidId;
        GenericVector vector;
        double distanceToCentroid;
};

#endif
