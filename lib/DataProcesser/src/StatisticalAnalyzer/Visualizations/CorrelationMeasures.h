#ifndef CorrelationMeasures_H
#define CorrelationMeasures_H

#include <vector>
#include <map>

#include "IVisualization.h"
#include "../Filter/GradePercentageFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"
#include "../GenericVector/GenericVector.h"

class CorrelationMeasures : public IVisualization
{
private:
  std::pair<GenericVector, GenericVector> xyVectors;
  bool inputValuesAreValid();

  float raiseVectorValues(std::vector<float> values, int exponent);

  float getStandardDeviation(std::vector<float> vals, float mean);
  float getMean(std::vector<float> vals);


  std::map<float, float> getRanksForUniqueValues(vector<float> sortedByLargestValues);
  std::pair<std::map<float, float>, std::map<float, float>> getUniqueRanksAsPairOfMaps();
  std::pair<std::vector<float>, std::vector<float>> getAllXYRanksIncludingRepeatedRanks(std::pair<std::map<float, float>, std::map<float, float>> xyRanksForUniqueValues);

  float getJointRankedSpearmanCoefficient(pair<map<float, float>, map<float, float>> xyRanksForUniqueValues,
                                          vector<float> allXRanks, vector<float> allYRanks);


  float getSpearmanCoefficient();
  float getPearsonCoefficient();

public:
  std::string getVisualizationAsJSON();
  CorrelationMeasures(std::pair<GenericVector, GenericVector> xy)
  {
    xyVectors = xy;
  };
};

#endif