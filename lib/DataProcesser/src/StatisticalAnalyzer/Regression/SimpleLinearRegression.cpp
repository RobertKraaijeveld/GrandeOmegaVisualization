#include <vector>
#include <map>

#include "IRegression.h" 
#include "SimpleLinearRegression.h" 
#include "../StatisticalTools/StatisticalTools.h" 


pair<GenericVector, GenericVector> SimpleLinearRegression::convertPairsToGVs(map<string, pair<float, float>> pairsMap)
{
    pair<GenericVector, GenericVector> returnGVs;

    map<string, pair<float, float>>::iterator it;
    for(it = pairsMap.begin(); it != pairsMap.end(); ++it)
    {
        returnGVs.first.values.push_back(it->second.first);
        returnGVs.second.values.push_back(it->second.second);        
    }
    return returnGVs;
}

float SimpleLinearRegression::getSlope()
{
    /*
    The mean of the x values
    The mean of the y values
    The standard deviation of the x values (denoted sx)
    The standard deviation of the y values (denoted sy)
    The correlation between X and Y (denoted r)

    he formula for the slope, m, of the best-fitting line is

    where r is the correlation between X and Y, and sx and sy are the standard deviations of the x-values and the y-values, respectively.
    You simply divide sy by sx and multiply the result by r.
    */
    float xMean = StatisticalTools::getMean(xAndY.first.values);
    float yMean = StatisticalTools::getMean(xAndY.second.values);    

    
}