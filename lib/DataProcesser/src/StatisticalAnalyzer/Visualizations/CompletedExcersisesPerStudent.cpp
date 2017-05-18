#include "CompletedExcersisesPerStudent.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"

#include <map>
#include <vector>
#include <iostream>
#include <pqxx/pqxx>

std::string CompletedExcersisesPerStudent::getVisualizationAsJSON()
{
    std::map<std::string, int> amountOfCompletedExcersisesPerStudent = getAmountOfCompletedExcersisesPerStudent();

    std::map<std::string, int> x;
    string y = "hello"; 
    x.insert(make_pair(y, 1));

    std::string mapAsJson = JSONEncoder::mapToJson(x);
    cout << "mapAsJson = " << mapAsJson << endl;
    return mapAsJson;
}

std::map<std::string, int> CompletedExcersisesPerStudent::getAmountOfCompletedExcersisesPerStudent()
{
    setFilterQueryIndexes();
    
    std::map<std::string, int> returnMap;

    DatabaseInteracter dbInteracter;
    std::string query = "SELECT student_id, creation_timestamp FROM assignments WHERE sort = 'completion';";

    pqxx::result unfilteredRows = dbInteracter.executeSelectQuery(query);
    std::vector<pqxx::result::tuple> filteredRows = getFilteredAssignmentRowsPtr(unfilteredRows);

    for (int i = 0; i < filteredRows.size(); i++)
    {
        std::string occurenceStudentIdStr = std::string(filteredRows[i][0].c_str());
        returnMap[occurenceStudentIdStr] = returnMap[occurenceStudentIdStr] + 1;
    }
    return returnMap;
}

void CompletedExcersisesPerStudent::setFilterQueryIndexes()
{
    //No guarantee that both struct indexes are set... catch exceptions for this and for incorrect indexes!!!
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 1;
    filter.queryIndexes = queryIndexes;
}

std::vector<pqxx::result::tuple> CompletedExcersisesPerStudent::getFilteredAssignmentRowsPtr(pqxx::result& unfilteredRows)
{
    std::vector<pqxx::result::tuple> rowsFilteredOnGradePercentile = filter.getRowsWithValidGradePercentile(unfilteredRows);
    return filter.getRowsWithValidAssignmentTimes(rowsFilteredOnGradePercentile);
}
