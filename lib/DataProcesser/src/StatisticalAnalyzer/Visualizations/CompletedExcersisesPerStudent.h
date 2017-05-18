#ifndef COMPLETEDEXCERSISESPERSTUDENT_H
#define COMPLETEDEXCERSISESPERSTUDENT_H

#include <map>
#include <vector>
#include <pqxx/pqxx>

#include "IVisualization.h"
#include "AnalysisFilter.h"

class CompletedExcersisesPerStudent : public IVisualization
{
    private:
        AnalysisFilter filter;

        std::map<std::string, int> getAmountOfCompletedExcersisesPerStudent();
        void setFilterQueryIndexes();
        std::vector<pqxx::result::tuple> getFilteredAssignmentRowsPtr(pqxx::result& unfilteredRows);

    public:
        std::string getVisualizationAsJSON();
        CompletedExcersisesPerStudent(AnalysisFilter f) { filter = f; };

};


#endif