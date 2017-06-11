#ifndef AttemptsVsFailures_H
#define AttemptsVsFailures_H

#include "IVisualization.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../Filter/GradePercentageFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>

class AttemptsVsFailures : public IVisualization
{
private:
  std::map<std::string, std::pair<int, int>> getFailsAndTotalAttemptsPerStudent();

public:
  std::string getVisualizationAsJSON();
};

#endif
