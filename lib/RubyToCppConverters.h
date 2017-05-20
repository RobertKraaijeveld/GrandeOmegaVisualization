#ifndef RUBYTOCPPCONVERTERS_H
#define RUBYTOCPPCONVERTERS_H

#include "rice/Array.hpp"

#include <vector>
#include <string>

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

std::vector<std::pair<float, float>> floatVectorToPairVector(vector<float> v)
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

#endif