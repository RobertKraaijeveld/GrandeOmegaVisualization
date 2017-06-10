#include <vector>
#include <map>
#include <cmath>
#include <numeric>
#include <iostream>

#include "IRegression.h"
#include "LogarithmicLinearRegression.h"
#include "../../Utilities/JSONEncoder.h"

using namespace std;


float LogarithmicLinearRegression::sumValuesUsingLog(GenericVector vector)
{
    float sumUsingLog = 0.0;

    for(int i = 0; i < vector.values.size(); i++)
    {
        sumUsingLog += (log(vector.values[i]));
    }
    return sumUsingLog;
}


float LogarithmicLinearRegression::getIntercept()
{
    float slope = getSlope();
    float amountOfValues = xAndYVectors.second.values.size(); 

    float xValuesSumUsingLog = sumValuesUsingLog(xAndYVectors.first);
    float yValuesSum = accumulate(xAndYVectors.second.values.begin(), xAndYVectors.second.values.end(), 0);
    
    float yValueMinusSlopeTimesXValuesSum = yValuesSum - (slope * xValuesSumUsingLog); 

    return yValueMinusSlopeTimesXValuesSum / amountOfValues;
}

float LogarithmicLinearRegression::getSlope()
{
    float amountOfValues = xAndYVectors.first.values.size(); 
    float finalSlope = 0.0;

    float xValuesSumUsingLog = sumValuesUsingLog(xAndYVectors.first);
    float yValuesSum = accumulate(xAndYVectors.second.values.begin(), xAndYVectors.second.values.end(), 0);


    float divisorFirstHalf = 0.0;
    float divisorSecondHalf =  yValuesSum * xValuesSumUsingLog;
    
    float dividendFirstHalf = 0.0;
    float dividendSecondHalf = pow(xValuesSumUsingLog, 2);
    
    for(int i = 0; i < amountOfValues; i++)
    {
        float& currX = xAndYVectors.first.values[i];
        float& currY = xAndYVectors.second.values[i];

        divisorFirstHalf += currY * log(currX); 
        dividendFirstHalf += pow(log(currX), 2);
    }
    float fullDivisor = (amountOfValues * divisorFirstHalf) - divisorSecondHalf;
    float fullDividend = (amountOfValues * dividendFirstHalf) - dividendSecondHalf;  

    return fullDivisor/fullDividend;
}


vector<pair<float, float>> LogarithmicLinearRegression::getRegression()
{
    vector<pair<float, float>> returnXYvaluesForLine;

    vector<float>& xValues = xAndYVectors.first.values;
    float xValuesAmount = xAndYVectors.first.values.size();

    float slope = getSlope();
    float intercept = getIntercept();    

    for (float &xValue : xValues)
    {
        float regressionLineYValue = slope * log(xValue) + intercept;
        returnXYvaluesForLine.push_back(make_pair(xValue, regressionLineYValue));
    }
    return returnXYvaluesForLine;
}

std::string LogarithmicLinearRegression::getRegressionAsJSON()
{
    vector<pair<float, float>> regressionLinePoints = getRegression();
    return JSONEncoder::pairsToJson(regressionLinePoints);
}

