#ifndef ANALYSISFILTER_H
#define ANALYSISFILTER_H

#include <string>
#include <pqxx/pqxx>

//Not very happy with this since itll need to change a LOT but there's not much of another way. 
struct FilterQueryColumnIndexes
{
    int assignmentIdIndex;
    int timestampIndex;
    FilterQueryColumnIndexes(){}
    FilterQueryColumnIndexes(int a, int t) { assignmentIdIndex = a; timestampIndex = t; }
};

class AnalysisFilter {
    private:
        int percentageToValue(int totalAmount);         
        bool isValidAssignmentTime(std::string& previousTime, std::string& currTime);    

        std::string getGradeSortingQuery();
        vector<int> getGradeIds();
        int getTotalAmountOfGrades();

        vector<pqxx::tuple> getRowsWithValidGradePercentile(pqxx::result& unfilteredRows);
        vector<pqxx::tuple> getRowsWithValidAssignmentTimes(vector<pqxx::tuple>& gradeFilteredRows);                        
        
    public:
        //TODO: Change this: queries should have their own datastructure.
        FilterQueryColumnIndexes queryIndexes;

        double timeBetweenAssignmentsThreshold = 0.0;
        double upperPercentageOfGradesToBeSelected = 100.0; 

        vector<pqxx::tuple> getFilteredQueryRows(std::string query);
        AnalysisFilter(){}
};

#endif