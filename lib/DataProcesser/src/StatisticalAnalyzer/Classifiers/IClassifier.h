#ifndef IClassifier_H
#define IClassifier_H

#include "../GenericVector/GenericVector.h"
#include "../Point/IClusteringPoint.h"

#include <map>
#include <vector>
#include <numeric>
#include <memory>

/**
  Each derived class IClassifier overrides the virtual 'getClassifiedPoints' and 'initialize' methods.
  'initialize' simulates a virtual constructor (which does not exist in C++) 
 */

class IClassifier
{
  public:
    virtual std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedPoints() = 0;
    virtual void initialize(std::vector<std::shared_ptr<IClusteringPoint>> input, vector<vector<std::shared_ptr<IClusteringPoint>>> tc) = 0;

};



#endif
