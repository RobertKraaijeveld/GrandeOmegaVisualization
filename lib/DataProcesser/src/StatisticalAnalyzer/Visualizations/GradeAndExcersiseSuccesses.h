#ifndef GRADEANDEXCERSISESUCCESSES_H
#define GRADEANDEXCERSISESUCCESSES_H

#include "IVisualization.h"
#include "AnalysisFilter.h"

#include <pqxx/pqxx>
#include <map>
#include <vector>


class GradeAndExcersiseSuccesses : public IVisualization 
{
    private:
        AnalysisFilter filter;

        std::map<std::string, std::pair<int, int>> getGradesAndSuccesses();
        pqxx::result getUnfilteredStudentSuccessCountsAndGrades();
        std::vector<pqxx::result::tuple> getFilteredStudentSuccessCountsAndGrades(pqxx::result& unfilteredRows);

    public:
        std::string getVisualizationAsJSON();
        GradeAndExcersiseSuccesses(AnalysisFilter f) { filter = f; };
};

#endif

