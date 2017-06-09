#ifndef IClusteringPoint_H
#define IClusteringPoint_H

#include "../GenericVector/GenericVector.h"
#include <map>
#include <numeric>
#include <iostream>

class IClusteringPoint
{
  public:
    //const makes sures the getters dont modify the derived object, which is required for the operator overload
    virtual GenericVector getVector() const = 0;
    virtual int getClusterId() const = 0;

    virtual void setVector(GenericVector newVector) = 0;
    virtual void setClusterId(int newCentroidId) = 0;

    //bit useless, but necessary to use this in a map
    bool operator<(const IClusteringPoint &other) const
    {
        float myVectorSum = std::accumulate(getVector().values.begin(), getVector().values.end(), 0.0);
        float otherVectorSum = std::accumulate(other.getVector().values.begin(), other.getVector().values.end(), 0.0);

        return myVectorSum < otherVectorSum;
    }

    //done in order to make STL vector searching algorithms for this type work
    bool operator==(const IClusteringPoint &other) const
    {
        if (getVector().values == other.getVector().values)
            return true;
        else
            return false;
    }
};



#endif
