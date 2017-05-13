#ifndef STATISTICALTOOLS_H
#define STATISTICALTOOLS_H

#include "../GenericVector/GenericVector.h"

class StatisticalTools
{
  private:
    static float raiseVectorValues(vector<float> values, int exponent);

  public:
    static float getStandardDeviation(vector<float> vals, float mean);
    static float getMean(vector<float> vals);
    static float getPearsonCoefficient(GenericVector x, GenericVector y);
};

#endif