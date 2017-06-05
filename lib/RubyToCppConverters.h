#ifndef RUBYTOCPPCONVERTERS_H
#define RUBYTOCPPCONVERTERS_H

#include "rice/Array.hpp"

#include <vector>
#include <string>

#include "DataProcesser/src/StatisticalAnalyzer/GenericVector/GenericVector.h"

using namespace Rice;

//Custom from_ruby() to convert ruby float arrays to cpp float vectors
template <>
std::vector<float> from_ruby<std::vector<float>>(Object o)
{
    Array a(o);
    std::vector<float> v;
    for (Array::iterator aI = a.begin(); aI != a.end(); ++aI)
        v.push_back(from_ruby<float>(*aI));
    return v;
}

std::vector<std::pair<float, float>> floatVectorToPairVector(std::vector<float> v)
{
    std::vector<std::pair<float, float>> returnPairs;

    std::pair<float, float> currPair;
    for(int i = 0; i < v.size(); i++)
    {
        if (i % 2 == 0)
        {
            currPair.first = v[i];
        }
        else
        {
            currPair.second = v[i];
            returnPairs.push_back(currPair);

            //resetting currPair
            currPair = make_pair(-1.0f, -1.0f);
        }
    }
    return returnPairs;
}

std::pair<GenericVector, GenericVector> convertPairsToGVs(std::vector<std::pair<float, float>> pairs)
{
    std::pair<GenericVector, GenericVector> returnGVs;

    for(int i = 0; i < pairs.size(); i++)
    {
        returnGVs.first.values.push_back(pairs[i].first);
        returnGVs.second.values.push_back(pairs[i].second);
    }
    return returnGVs;
}

#endif