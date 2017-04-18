#ifndef POINT_H
#define POINT_H

#include "GenericVector.h"
#include <map>

struct Centroid {
    int id; 
    bool hasChanged = true;
    GenericVector vector;

    Centroid(int id, GenericVector vector);
    Centroid(){ id = -1; vector = GenericVector(); };
};


class Point {
    public:
        Point(int cId, GenericVector vector);
        int centroidId;
        GenericVector vector;
        double distanceToCentroid;
};

#endif
