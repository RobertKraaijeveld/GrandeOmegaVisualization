#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"
#include "../Filter/IFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"
#include "GradeAndExcersiseSuccesses.h"

#include <map>
#include <vector>
#include <iostream>
#include <pqxx/pqxx>

std::string GradeAndExcersiseSuccesses::getVisualizationAsJSON()
{
    std::map<std::string, std::pair<int, int>> gradesAndSuccessesPerStudent = getGradesAndSuccesses();
    return JSONEncoder::mapToJson(gradesAndSuccessesPerStudent);
}

std::map<std::string, std::pair<int, int>> GradeAndExcersiseSuccesses::getGradesAndSuccesses()
{
    std::map<std::string, std::pair<int, int>> returnMapOfPairs;

    pqxx::result unfilteredRows = getUnfilteredStudentSuccessCountsAndGrades();
    std::vector<pqxx::result::tuple> filteredStudentSuccessCountsAndGrades = getFilteredStudentSuccessCountsAndGrades(unfilteredRows);

    for (auto row : filteredStudentSuccessCountsAndGrades)
    {
        //literal for frontend
        std::string studentIdStr = "Student id " + string(row[0].c_str());

        int newSuccesRate = returnMapOfPairs[studentIdStr].first + 1;
        int gradeOfRow = stoi(row[1].c_str());

        returnMapOfPairs[studentIdStr] = make_pair(newSuccesRate, gradeOfRow);
    }
    return returnMapOfPairs;
}

pqxx::result GradeAndExcersiseSuccesses::getUnfilteredStudentSuccessCountsAndGrades()
{
    DatabaseInteracter dbInteracter;

    std::ostringstream queryStream;
    queryStream << "SELECT assignments.student_id, grades.grade, assignments.creation_timestamp"
                << " FROM assignments, grades WHERE assignments.sort != 'Failure'"
                << " AND assignments.student_id = grades.student_id;";

    return dbInteracter.executeSelectQuery(queryStream.str());
}

std::vector<pqxx::result::tuple> GradeAndExcersiseSuccesses::getFilteredStudentSuccessCountsAndGrades(pqxx::result& unfilteredRowsOutOfScope)
{
    //weird position to do this in
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 2;

    gradeFilter->setFilterQueryColumnIndexes(queryIndexes);
    assignmentIntervalFilter->setFilterQueryColumnIndexes(queryIndexes);
    timeFilter->setFilterQueryColumnIndexes(queryIndexes);    

    std::vector<pqxx::result::tuple> unfilteredRowsAsPqxxVector = Utilities::toListOfPqxxTuples(unfilteredRowsOutOfScope);

    std::vector<pqxx::result::tuple> rowsFilteredOnGradePercentage = gradeFilter->filter(unfilteredRowsAsPqxxVector);
    std::vector<pqxx::result::tuple> rowsFilteredOnAssignmentInterval = assignmentIntervalFilter->filter(rowsFilteredOnGradePercentage);
    std::vector<pqxx::result::tuple> rowsFilteredOnDays = timeFilter->filter(rowsFilteredOnAssignmentInterval);

    return rowsFilteredOnDays;
}