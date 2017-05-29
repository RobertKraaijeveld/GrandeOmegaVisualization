#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "GradePercentageFilter.h"



int GradePercentageFilter::percentageToValue(int totalAmount) 
{
    double percentAsDecimal = filterContext.upperPercentageOfGradesToBeSelected * 0.01;
    return ceil(totalAmount * percentAsDecimal);
}

std::string GradePercentageFilter::getGradeSortingQuery()
{
    int totalGradesAmount = getTotalAmountOfGrades();
    return "ORDER BY CAST(grades.grade AS int) DESC LIMIT " + to_string(percentageToValue(totalGradesAmount)) + ";";
}

std::vector<int> GradePercentageFilter::getGradeIds()
{
    DatabaseInteracter dbInteracter;

    std::ostringstream queryStream;
    queryStream << "SELECT student_id FROM grades "
                << getGradeSortingQuery() << ";";

    pqxx::result allGradeStudentIds = dbInteracter.executeSelectQuery(queryStream.str());

    std::vector<int> allGradeIdsInts;
    for(auto row: allGradeStudentIds)
    {
        allGradeIdsInts.push_back(stoi(row[0].c_str()));
    }
    return allGradeIdsInts;
}

int GradePercentageFilter::getTotalAmountOfGrades()
{
    DatabaseInteracter dbInteracter;
    string amountOfGradesQuery = "SELECT COUNT(grade) FROM grades;";

    pqxx::result amountOfGradesQueryResult = dbInteracter.executeSelectQuery(amountOfGradesQuery);

    return stoi(amountOfGradesQueryResult[0][0].c_str());
}



std::vector<pqxx::result::tuple> GradePercentageFilter::filter(vector<pqxx::result::tuple> unfilteredRows)
{
    std::vector<pqxx::result::tuple> gradeFilteredRows;
    std::vector<int> gradeStudentIds = getGradeIds();

    for(pqxx::result::tuple row: unfilteredRows)
    {
        int unfilteredRowStudentId = stoi(row[queryColumnIndexes.studentIdColumnIndex].c_str());   
        
        if(std::find(gradeStudentIds.begin(), gradeStudentIds.end(), unfilteredRowStudentId) != gradeStudentIds.end())
        {
            gradeFilteredRows.push_back(row);
        }   
    }
    return gradeFilteredRows;
}

