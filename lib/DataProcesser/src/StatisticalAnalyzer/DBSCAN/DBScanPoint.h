#ifndef ClusteringPoint_H
#define ClusteringPoint_H

#include "../../GenericVector/GenericVector.h"
#include <map>
#include <numeric>

class DBScanPoint {
    public:
        ClusteringPoint(){};
        ClusteringPoint(int cId, GenericVector vector);
};

#endif
