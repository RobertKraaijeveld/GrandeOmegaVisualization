#ifndef DBScanPoint_H
#define DBScanPoint_H

#include "IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include <map>
#include <numeric>
#include <iostream>

class DBScanPoint : public IClusteringPoint
{
    public:
        bool visited = false;
        bool isNoise = false;

        GenericVector vector;
        int clusterId; 


        GenericVector getVector() const { return vector; };
        int getClusterId() const { return clusterId; };

        void setVector(GenericVector newVector) { vector = newVector; };
        void setClusterId(int newClusterId) { clusterId = newClusterId; };

        DBScanPoint(){};
        DBScanPoint(int cId, GenericVector v)
        {
            vector = v;
            clusterId = cId;
        }
};

#endif
