#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

#include "../../Utilities/Utilities.h"
#include "../../Utilities/JSONEncoder.h"
#include "CorrelationMeasures.h"
#include "../GenericVector/GenericVector.h"

float CorrelationMeasures::getStandardDeviation(vector<float> vals, float mean)
{
    float sumOfSquaredMeanDistances = 0.0f;
    for (auto &val : vals)
    {
        sumOfSquaredMeanDistances += pow(fabs(val - mean), 2);
    }
    return sqrt(sumOfSquaredMeanDistances / vals.size());
}

float CorrelationMeasures::getMean(vector<float> vals)
{
    float total = 0.0f;
    for (auto &val : vals)
    {
        total += val;
    }
    return total / vals.size();
}

float CorrelationMeasures::raiseVectorValues(std::vector<float> values, int exponent)
{
    float returnVal = 0.0f;
    for (float &val : values)
    {
        returnVal += pow(val, exponent);
    }
    return returnVal;
}


/*
SPEARMANS
*/

std::map<float, float> CorrelationMeasures::getRanksForUniqueValues(vector<float> sortedByLargestValues)
{
    std::map<float, float> valuesAndRanks;

    //1 to avoid division by zero
    float duplicateRankAmount = 1;
    float previousValue = 0; 

    for (size_t i = 0; i < sortedByLargestValues.size(); i++)
    {
        float currValue = sortedByLargestValues[i];

        if(valuesAndRanks.count(currValue) >= 1)
        {
            valuesAndRanks[currValue] += (i + 1);
            duplicateRankAmount++;
        }
        else
        {
            //using and resetting duplication counter so duped values are divided by the amount of times they are duped
            if(i != 0 ) //cant have dupes at first
                valuesAndRanks[previousValue] /= duplicateRankAmount;

            valuesAndRanks.insert(make_pair(currValue, i + 1)); 
            duplicateRankAmount = 1;
        }
        //in case the last value is a dupe, make sure averaging still takes place        
        if(i == (sortedByLargestValues.size() - 1))
        {
            valuesAndRanks[previousValue] /= duplicateRankAmount;
        }

        previousValue = currValue;
    }
    return valuesAndRanks;
}


//christ thats a long type signature
pair<map<float, float>, map<float, float>> CorrelationMeasures::getUniqueRanksAsPairOfMaps()
{
    //temporary ordering since we later need the xyVectors in their original ordering
    vector<float> orderedXValues = xyVectors.first.values;
    vector<float> orderedYValues = xyVectors.second.values;

    std::sort(orderedXValues.begin(), orderedXValues.end());
    std::sort(orderedYValues.begin(), orderedYValues.end());

    map<float, float> xValuesAndRanks = getRanksForUniqueValues(orderedXValues);
    map<float, float> yValuesAndRanks = getRanksForUniqueValues(orderedYValues);

    return make_pair(xValuesAndRanks, yValuesAndRanks);
}

pair<vector<float>, vector<float>> CorrelationMeasures::getAllXYRanksIncludingRepeatedRanks(pair<map<float, float>, map<float, float>> xyRanksForUniqueValues)
{
    vector<float> allXRanks;
    vector<float> allYRanks;    

    map<float, float> uniqueRanksPerXValue = xyRanksForUniqueValues.first;
    map<float, float> uniqueRanksPerYValue = xyRanksForUniqueValues.second;

    for(int i = 0; i < xyVectors.first.values.size(); i++)
    {
        float& currXValue = xyVectors.first.values[i];
        float& currYValue = xyVectors.second.values[i];

        if(uniqueRanksPerXValue.count(currXValue))
            allXRanks.push_back(uniqueRanksPerXValue[currXValue]);

        if(uniqueRanksPerYValue.count(currYValue))
            allYRanks.push_back(uniqueRanksPerYValue[currYValue]);
    }
    return make_pair(allXRanks, allYRanks);
}


float CorrelationMeasures::getJointRankedSpearmanCoefficient(pair<map<float, float>, map<float, float>> xyRanksForUniqueValues,
                                                             vector<float> allXRanks, vector<float> allYRanks)
{
    map<float, float>& xRanksForUniqueValues = xyRanksForUniqueValues.first;
    map<float, float>& yRanksForUniqueValues = xyRanksForUniqueValues.second;

    float xMean = getMean(allXRanks);
    float yMean = getMean(allYRanks);


    float dividend = 0;
    float divisorFirstHalf = 0;
    float divisorSecondHalf = 0;

    //rank usage needs to be done ordered on the same ORIGINAL order as the values
    for (int i = 0; i < xyVectors.first.values.size(); i++)
    {
        float currXValue = xyVectors.first.values[i];
        float currYValue = xyVectors.second.values[i];        

        float currXRank = xRanksForUniqueValues[currXValue];
        float currYRank = yRanksForUniqueValues[currYValue];


        float xValMinusxMean = currXRank - xMean;
        float yValMinusyMean = currYRank - yMean;

        dividend += (xValMinusxMean * yValMinusyMean);

        divisorFirstHalf += pow(xValMinusxMean, 2);
        divisorSecondHalf += pow(yValMinusyMean, 2);
    }
    float divisor = sqrt(divisorFirstHalf * divisorSecondHalf);

    return dividend / divisor;
}

float CorrelationMeasures::getSpearmanCoefficient()
{
    pair<map<float, float>, map<float, float>> xyRanksForUniqueValues = getUniqueRanksAsPairOfMaps();

    vector<float> uniqueXRanks = Utilities::getValuesOfMap(xyRanksForUniqueValues.first); 
    vector<float> uniqueYRanks = Utilities::getValuesOfMap(xyRanksForUniqueValues.second);

    pair<vector<float>, vector<float>> allXYRanksIncludingRepeatedRanks = getAllXYRanksIncludingRepeatedRanks(xyRanksForUniqueValues);
    
    vector<float> allXRanks = allXYRanksIncludingRepeatedRanks.first;
    vector<float> allYRanks = allXYRanksIncludingRepeatedRanks.second;

    return getJointRankedSpearmanCoefficient(xyRanksForUniqueValues, allXRanks, allYRanks);
}

/*
END SPEARMAN
*/



/*
PEARSONS
*/

float CorrelationMeasures::getPearsonCoefficient()
{
    //Saves us a few size() calls :)
    int xValuesSize = xyVectors.first.values.size();
    int yValuesSize = xyVectors.second.values.size();

    float xyMultiplication = xyVectors.first.dotProduct(xyVectors.second);

    //Note that the type of accumulate's last literal arg determines its returntype.
    int xSum = std::accumulate(xyVectors.first.values.begin(), xyVectors.first.values.end(), 0);
    int ySum = std::accumulate(xyVectors.second.values.begin(), xyVectors.second.values.end(), 0);
    int sumsDividedByDimension = (xSum * ySum) / xValuesSize;

    float xValsSquared = raiseVectorValues(xyVectors.first.values, 2);
    float yValsSquared = raiseVectorValues(xyVectors.second.values, 2);

    float xValsSumDividedByDimension = pow(xSum, 2) / xValuesSize;
    float yValsSumDividedByDimension = pow(ySum, 2) / yValuesSize;

    float dividend = xyMultiplication - sumsDividedByDimension;
    float divisorFirstHalf = sqrt(xValsSquared - xValsSumDividedByDimension);
    float divisorSecondHalf = sqrt(yValsSquared - yValsSumDividedByDimension);

    return (dividend / (divisorFirstHalf * divisorSecondHalf));
}

/*
END PEARSONS
*/

bool CorrelationMeasures::inputValuesAreValid()
{
    vector<float> &xValues = xyVectors.first.values;
    vector<float> &yValues = xyVectors.second.values;

    //less than 5 values, non-equal size or non-unique vector values is not okay
    return xValues.size() >= 5 && yValues.size() >= 5 
           && xValues.size() == yValues.size()
           && std::equal(xValues.begin() + 1, xValues.end(), xValues.begin()) == false 
           && std::equal(yValues.begin() + 1, yValues.end(), yValues.begin()) == false;
}

//pairsToJson
std::string CorrelationMeasures::getVisualizationAsJSON()
{
    bool separateJSONIntoXYPoints = false;

    if (inputValuesAreValid())
    {
        std::map<string, float> measures;

        float pearsonCoefficient = getPearsonCoefficient();
        float spearmanCoefficient = getSpearmanCoefficient();

        measures["pearsonCoefficient"] = pearsonCoefficient;
        measures["spearmanCoefficient"] = spearmanCoefficient;

        return JSONEncoder::mapToJson(measures, separateJSONIntoXYPoints);
    }
    else
    {
        std::map<string, string> invalidValuesReturnMap;
        invalidValuesReturnMap["pearsonCoefficient"] = "Invalid input";
        invalidValuesReturnMap["spearmanCoefficient"] = "Invalid input";

        return JSONEncoder::mapToJson(invalidValuesReturnMap, separateJSONIntoXYPoints);
    }
}
