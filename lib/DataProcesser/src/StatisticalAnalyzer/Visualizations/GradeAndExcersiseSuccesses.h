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
    std::shared_ptr<IFilter> gradeFilter;
    std::shared_ptr<IFilter> assignmentIntervalFilter;
    std::shared_ptr<IFilter> weekDayOnlyFilter;    

    std::map<std::string, std::pair<int, int>> getGradesAndSuccesses();
    pqxx::result getUnfilteredStudentSuccessCountsAndGrades();
    std::vector<pqxx::result::tuple> getFilteredStudentSuccessCountsAndGrades(pqxx::result &unfilteredRows);

  public:
    std::string getVisualizationAsJSON();
    GradeAndExcersiseSuccesses(std::shared_ptr<IFilter> gf,
                               std::shared_ptr<IFilter> af,
                               std::shared_ptr<IFilter> wd)
    {
        gradeFilter = gf;
        assignmentIntervalFilter = af;
        weekDayOnlyFilter = wd;
    };
};

#endif
