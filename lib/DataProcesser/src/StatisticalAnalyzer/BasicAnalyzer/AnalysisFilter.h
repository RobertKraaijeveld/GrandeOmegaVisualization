#ifndef ANALYSISFILTER_H
#define ANALYSISFILTER_H

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
        
        vector<int> getGradeIds();        

    public:
        //TODO: Change this: queries should have their own datastructure.
        FilterQueryColumnIndexes queryIndexes;
        double timeBetweenAssignmentsThreshold = 0.0;
        double upperPercentageOfGradesToBeSelected = 100.0; 

        //TODO: Template this for both pqxx::result as well as vector of result tuples
        vector<pqxx::result::tuple> getRowsWithValidGradePercentile(pqxx::result& unfilteredRows);
        vector<pqxx::result::tuple> getRowsWithValidAssignmentTimes(vector<pqxx::result::tuple>& gradeFilteredRows); 

        AnalysisFilter(){}
};

#endif