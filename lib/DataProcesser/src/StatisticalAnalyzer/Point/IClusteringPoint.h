#ifndef IClusteringPoint_H
#define IClusteringPoint_H

#include "../GenericVector/GenericVector.h"
#include <map>
#include <numeric>

class IClusteringPoint {
    public:
        //const makes sures the getters dont modify the derived object
        virtual GenericVector getVector() const {};
        virtual int getClusterId() const {}; 
        string x(){ return "well shit"; };
 
        virtual void setVector(GenericVector newVector) = 0;
        virtual void setClusterId(int newCentroidId) = 0;

        //bit useless, but necessary to use this in a map
        bool operator < (const IClusteringPoint& other) const
        {
            float myVectorSum = std::accumulate(getVector().values.begin(), getVector().values.end(), 0.0);
            float otherVectorSum = std::accumulate(other.getVector().values.begin(), other.getVector().values.end(), 0.0);
            
            return myVectorSum < otherVectorSum;
        }
};

#endif
