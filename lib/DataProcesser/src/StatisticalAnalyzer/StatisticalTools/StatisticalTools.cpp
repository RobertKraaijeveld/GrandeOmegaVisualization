#include <numeric>
#include <cmath>
#include <vector>

#include "StatisticalTools.h"
#include "../GenericVector/GenericVector.h"

//TEST THESE METHODS

float StatisticalTools::getStandardDeviation(vector<float> vals, float mean)
{
    float sumOfSquaredMeanDistances = 0.0f;
    for(auto& val : vals)
    {
        sumOfSquaredMeanDistances += pow(fabs(val - mean), 2);
    }
    return sqrt(sumOfSquaredMeanDistances / vals.size());
}

float StatisticalTools::getMean(vector<float> vals)
{
    float total = 0.0f;
    for(auto& val : vals)
    {
        total += val;
    }
    return total / vals.size();
}

float StatisticalTools::raiseVectorValues(std::vector<float> values, int exponent)
{
    float returnVal = 0.0f;
    for (float& val : values)
    {
        returnVal += pow(val, exponent);
    }    
    return returnVal;
}

float StatisticalTools::getPearsonCoefficient(GenericVector x, GenericVector y)
{
    //Saves us a few size() calls :)
    int xValuesSize = x.values.size();
    int yValuesSize = y.values.size();

    //Make this raise an exception on unequal dimensions
    float xyMultiplication = x.dotProduct(y);

    //Note that the type of accumulate's last literal arg determines its returntype.
    int xSum = std::accumulate(x.values.begin(), x.values.end(), 0);
    int ySum = std::accumulate(y.values.begin(), y.values.end(), 0); 
    int sumsDividedByDimension = (xSum * ySum) / xValuesSize;

    float xValsSquared = StatisticalTools::raiseVectorValues(x.values, 2);
    float yValsSquared = StatisticalTools::raiseVectorValues(y.values, 2); 

    float xValsSumDividedByDimension = pow(xSum, 2) / xValuesSize;      
    float yValsSumDividedByDimension = pow(ySum, 2) / yValuesSize;     
     
    float dividend = xyMultiplication - sumsDividedByDimension;
    float divisorFirstHalf = sqrt(xValsSquared - xValsSumDividedByDimension);
    float divisorSecondHalf = sqrt(yValsSquared - yValsSumDividedByDimension);      

    return (dividend / (divisorFirstHalf * divisorSecondHalf));
}


