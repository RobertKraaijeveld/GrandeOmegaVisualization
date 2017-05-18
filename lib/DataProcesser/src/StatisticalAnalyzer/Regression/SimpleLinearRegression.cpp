#include <vector>
#include <map>
#include <iostream>

#include "IRegression.h"
#include "SimpleLinearRegression.h"
#include "../StatisticalTools/StatisticalTools.h"

using namespace std;

pair<GenericVector, GenericVector> SimpleLinearRegression::convertPairsToGVs(map<string, pair<float, float>> pairsMap)
{
    pair<GenericVector, GenericVector> returnGVs;

    map<string, pair<float, float>>::iterator it;
    for (it = pairsMap.begin(); it != pairsMap.end(); ++it)
    {
        returnGVs.first.values.push_back(it->second.first);
        returnGVs.second.values.push_back(it->second.second);
    }
    return returnGVs;
}

pair<float, float> SimpleLinearRegression::getXYMeans()
{
    GenericVector& xVector = xAndYVectors.first;
    GenericVector& yVector = xAndYVectors.second;

    float xMean = StatisticalTools::getMean(xVector.values);
    float yMean = StatisticalTools::getMean(yVector.values);

    return make_pair(xMean, yMean);
}

float SimpleLinearRegression::getSlope()
{
    pair<float, float> xyMeans = getXYMeans();

    float xValsSD = StatisticalTools::getStandardDeviation(xAndYVectors.first.values, xyMeans.first);
    float yValsSD = StatisticalTools::getStandardDeviation(xAndYVectors.second.values, xyMeans.second);

    float correlation = StatisticalTools::getPearsonCoefficient(xAndYVectors.first, xAndYVectors.second);

    return (yValsSD / xValsSD) * correlation;
}

float SimpleLinearRegression::getIntercept()
{
    pair<float, float> xyMeans = getXYMeans();

    float slope = getSlope();
    return xyMeans.first - (slope * xyMeans.second);
}

vector<pair<float, float>> SimpleLinearRegression::getRegression(vector<float> xValues)
{
    vector<pair<float, float>> returnXYvaluesForLine;

    for (float &xValue : xValues)
    {
        float currSlope = getSlope();
        float currIntercept = getIntercept();

        float yValue = (currSlope * xValue) + currIntercept;

        cout << "Adding regression line pair " << xValue << "," << yValue << endl;
        returnXYvaluesForLine.push_back(make_pair(xValue, yValue));
    }
    return returnXYvaluesForLine;
}
