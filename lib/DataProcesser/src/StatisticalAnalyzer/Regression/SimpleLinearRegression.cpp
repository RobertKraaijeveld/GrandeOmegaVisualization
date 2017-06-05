#include <vector>
#include <map>
#include <cmath>
#include <numeric>
#include <iostream>

#include "IRegression.h"
#include "SimpleLinearRegression.h"
#include "../../Utilities/JSONEncoder.h"

using namespace std;


float SimpleLinearRegression::getIntercept()
{
    float slope = getSlope();
    float amountOfValues = xAndYVectors.second.values.size(); 

    float xValuesSum = accumulate(xAndYVectors.first.values.begin(), xAndYVectors.first.values.end(), 0);
    float yValuesSum = accumulate(xAndYVectors.second.values.begin(), xAndYVectors.second.values.end(), 0);
    
    float yValueMinusSlopeTimesXValuesSum = yValuesSum - (slope * xValuesSum); 

    return yValueMinusSlopeTimesXValuesSum / amountOfValues;
}

float SimpleLinearRegression::getSlope()
{
    float amountOfValues = xAndYVectors.first.values.size(); 
    float finalSlope = 0.0;

    float xValuesSum = accumulate(xAndYVectors.first.values.begin(), xAndYVectors.first.values.end(), 0);
    float yValuesSum = accumulate(xAndYVectors.second.values.begin(), xAndYVectors.second.values.end(), 0);


    float divisorFirstHalf = 0.0;
    float divisorSecondHalf = xValuesSum * yValuesSum;
    
    float dividendFirstHalf = 0.0;
    float dividendSecondHalf = pow(xValuesSum, 2);
    
    for(int i = 0; i < amountOfValues; i++)
    {
        float& currX = xAndYVectors.first.values[i];
        float& currY = xAndYVectors.second.values[i];

        divisorFirstHalf += (currX * currY); 

        dividendFirstHalf += pow(currX, 2);
    }
    float fullDivisor = (amountOfValues * divisorFirstHalf) - divisorSecondHalf;
    float fullDividend = (amountOfValues * dividendFirstHalf) - dividendSecondHalf;  

    return fullDivisor/fullDividend;
}


vector<pair<float, float>> SimpleLinearRegression::getRegression()
{
    vector<pair<float, float>> returnXYvaluesForLine;
    vector<float>& xValues = xAndYVectors.first.values;
    float xValuesAmount = xAndYVectors.first.values.size();

    float slope = getSlope();
    float intercept = getIntercept();    

    for (float &xValue : xValues)
    {
        float regressionLineYValue =  slope * xValue + intercept;
        returnXYvaluesForLine.push_back(make_pair(xValue, regressionLineYValue));
    }
    return returnXYvaluesForLine;
}

std::string SimpleLinearRegression::getRegressionAsJSON()
{
    vector<pair<float, float>> regressionLinePoints = getRegression();
    return JSONEncoder::pairsToJson(regressionLinePoints);
}

