#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"
#include "../Filter/IFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"
#include "GradeAndExcersiseSuccesses.h"

#include <map>
#include <vector>
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

    //No guarantee that both struct indexes are set... catch exceptions for this and for incorrect indexes!!!
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 2;

    filter->queryColumnIndexes = queryIndexes;

    return dbInteracter.executeSelectQuery(queryStream.str());
}

//needs the reference param or else we get segfaults on getRowsWithValidGradePercentage which also takes a ref
std::vector<pqxx::result::tuple> GradeAndExcersiseSuccesses::getFilteredStudentSuccessCountsAndGrades(pqxx::result& unfilteredRowsOutOfScope)
{
    std::vector<pqxx::result::tuple> rowsFilteredOnGradePercentage = filter->filter(Utilities::toListOfPqxxTuples(unfilteredRowsOutOfScope));
    
    std::shared_ptr<IFilter> newFilter(new AssignmentIntervalFilter(filter->queryColumnIndexes, filter->filterContext));
    filter = newFilter;
    
    std::vector<pqxx::result::tuple> rowsFilteredOnAssignmentInterval = filter->filter(rowsFilteredOnGradePercentage);  

    return rowsFilteredOnAssignmentInterval;
}