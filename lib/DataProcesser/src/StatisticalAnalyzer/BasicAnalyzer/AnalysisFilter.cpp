#include <string>
#include <iostream>
#include <cmath>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "AnalysisFilter.h"

int AnalysisFilter::percentageToValue(int totalAmount) 
{
    double percentAsDecimal = upperPercentageOfGradesToBeSelected * 0.01;
    return ceil(totalAmount * percentAsDecimal);
}

bool AnalysisFilter::isValidAssignmentTime(std::string& previousTime, std::string& currTime)
{
    UtcTime previousUtcTime = UtcReader::toUtcTime(previousTime);
    UtcTime currUtcTime = UtcReader::toUtcTime(currTime);

    return ((currUtcTime.minute * 60) + currUtcTime.second)
           - ((previousUtcTime.minute * 60) + previousUtcTime.second) >= timeBetweenAssignmentsThreshold;
}

vector<int> AnalysisFilter::getGradeIds()
{
    DatabaseInteracter dbInteracter;

    std::ostringstream queryStream;
    queryStream << "SELECT student_id FROM grades "
                << getGradeSortingQuery() << ";";

    pqxx::result allGradeStudentIds = dbInteracter.executeSelectQuery(queryStream.str());

    vector<int> allGradeIdsInts;
    for(auto row: allGradeStudentIds)
    {
        allGradeIdsInts.push_back(stoi(row[0].c_str()));
    }
    return allGradeIdsInts;
}

int AnalysisFilter::getTotalAmountOfGrades()
{
    DatabaseInteracter dbInteracter;
    string amountOfGradesQuery = "SELECT COUNT(grade) FROM grades;";

    pqxx::result amountOfGradesQueryResult = dbInteracter.executeSelectQuery(amountOfGradesQuery);

    return stoi(amountOfGradesQueryResult[0][0].c_str());
}

vector<pqxx::result::tuple> AnalysisFilter::getRowsWithValidGradePercentile(pqxx::result& unfilteredRows)
{
    vector<pqxx::result::tuple> gradeFilteredRows;
    vector<int> gradeStudentIds = getGradeIds();

    for(int i = 0; i < unfilteredRows.size(); i++)
    {
        int unfilteredRowStudentId = stoi(unfilteredRows[i][queryIndexes.studentIdColumnIndex].c_str());   
        for(int j = 0; j < gradeStudentIds.size(); j++)
        {
            if(unfilteredRowStudentId == gradeStudentIds[j])
            {
                gradeFilteredRows.push_back(unfilteredRows[i]);
                break;
            }   
        }
    }
    return gradeFilteredRows;
}

vector<pqxx::result::tuple> AnalysisFilter::getRowsWithValidAssignmentTimes(vector<pqxx::result::tuple>& gradeFilteredRows)
{  
    vector<pqxx::result::tuple> filteredRows;

    map<string, string> studentsAndLatestTimestamps;

    for(pqxx::result::tuple row: gradeFilteredRows)
    {
        string currStudentId = string(row[queryIndexes.studentIdColumnIndex].c_str());
        string currTime = string(row[queryIndexes.timestampIndex].c_str()); 

        if(studentsAndLatestTimestamps.count(currStudentId) == 1)
        {
            string& previousTime = studentsAndLatestTimestamps[currStudentId];

            if(isValidAssignmentTime(previousTime, currTime))
            {
                filteredRows.push_back(row);
            }
        }                 
        studentsAndLatestTimestamps[currStudentId] = currTime;        
    }
    return filteredRows;
}

std::string AnalysisFilter::getGradeSortingQuery()
{
    int totalGradesAmount = getTotalAmountOfGrades();
    return "ORDER BY CAST(grades.grade AS int) DESC LIMIT " + to_string(percentageToValue(totalGradesAmount)) + ";";
}
