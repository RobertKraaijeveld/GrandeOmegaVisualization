#include <vector>
#include "../GenericVector/GenericVector.h"
#include "IRegression.h"

pair<GenericVector, GenericVector> IRegression::convertPairsToGVs(std::vector<std::pair<float, float>> pairs)
{
    pair<GenericVector, GenericVector> returnGVs;

    for(int i = 0; i < pairs.size(); i++)
    {
        returnGVs.first.values.push_back(pairs[i].first);
        returnGVs.second.values.push_back(pairs[i].second);
    }
    return returnGVs;
}
