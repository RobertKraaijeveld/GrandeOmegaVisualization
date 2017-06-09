#ifndef JSONENCODER_H
#define JSONENCODER_H

#include "Utilities.h"
#include "../StatisticalAnalyzer/GenericVector/GenericVector.h"
#include "../StatisticalAnalyzer/Point/IClusteringPoint.h"
#include <vector>
#include <string>
#include <memory>

class JSONEncoder
{
  private:
    template <typename T, typename J>
    static std::string kvToJson(T &k, J &v);

  public:
    template <typename T, typename J>
    static std::string pairToJson(pair<T, J> &pair);

    template <typename T, typename J>
    static std::string pairsToJson(vector<pair<T, J>> &pairList);

    template <typename T, typename J>
    static std::string mapToJson(map<T, J> &map, bool separateIntoXY);
    template <typename T, typename J, typename L>
    static std::string mapToJson(map<T, pair<J, L>> &mapOfPairs);

    static std::string clustersToJSON(vector<vector<std::shared_ptr<IClusteringPoint>>> clusters);
};

//Contains implementation, since template implementations have to be in the header of the template declarations.
#include "JSONEncoder.inl"

#endif