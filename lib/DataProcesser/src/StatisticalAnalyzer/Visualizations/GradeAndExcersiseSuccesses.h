#ifndef GRADEANDEXCERSISESUCCESSES_H
#define GRADEANDEXCERSISESUCCESSES_H

#include "IVisualization.h"
#include "../Filter/GradePercentageFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>

class GradeAndExcersiseSuccesses : public IVisualization
{
  private:
    std::shared_ptr<GradePercentageFilter> gradeFilter;
    std::shared_ptr<AssignmentIntervalFilter> assignmentIntervalFilter;

    std::map<std::string, std::pair<int, int>> getGradesAndSuccesses();
    pqxx::result getUnfilteredStudentSuccessCountsAndGrades();
    std::vector<pqxx::result::tuple> getFilteredStudentSuccessCountsAndGrades(pqxx::result &unfilteredRows);

  public:
    std::string getVisualizationAsJSON();
    GradeAndExcersiseSuccesses(std::shared_ptr<GradePercentageFilter> gf,
                               std::shared_ptr<AssignmentIntervalFilter> af)
    {
        gradeFilter = gf;
        assignmentIntervalFilter = af;
    };
};

#endif
