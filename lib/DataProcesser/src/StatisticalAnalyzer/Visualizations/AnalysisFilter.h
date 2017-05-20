#ifndef ANALYSISFILTER_H
#define ANALYSISFILTER_H

#include <vector>
#include <string>
#include <pqxx/pqxx>

//Not very happy with this since itll need to change a LOT but there's not much of another way. 
struct FilterQueryColumnIndexes
{
    int studentIdColumnIndex;
    int timestampIndex;
    FilterQueryColumnIndexes(){}
};

class AnalysisFilter {
    private:
        int percentageToValue(int totalAmount);         
        bool isValidAssignmentTime(std::string& previousTime, std::string& currTime);    

        std::string getGradeSortingQuery();
        int getTotalAmountOfGrades();
        
        std::vector<int> getGradeIds();        

    public:
        //TODO: Change this: queries should have their own datastructure.
        FilterQueryColumnIndexes queryIndexes;
        double timeBetweenAssignmentsThreshold = 0.0;
        double upperPercentageOfGradesToBeSelected = 100.0; 

        //TODO: Template this for both pqxx::result as well as std::vector of result tuples
        std::vector<pqxx::result::tuple> getRowsWithValidGradePercentage(pqxx::result& unfilteredRows);
        std::vector<pqxx::result::tuple> getRowsWithValidAssignmentTimes(std::vector<pqxx::result::tuple>& gradeFilteredRows); 

        AnalysisFilter(){}
};

#endif