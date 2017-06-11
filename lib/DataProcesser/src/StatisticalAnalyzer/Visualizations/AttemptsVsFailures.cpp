#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"
#include "../Filter/IFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"
#include "AttemptsVsFailures.h"

#include <map>
#include <vector>
#include <iostream>
#include <pqxx/pqxx>


std::string AttemptsVsFailures::getVisualizationAsJSON()
{
    std::map<std::string, std::pair<int, int>> gradesAndSuccessesPerStudent = getFailsAndTotalAttemptsPerStudent();
    return JSONEncoder::mapToJson(gradesAndSuccessesPerStudent);
}

std::map<std::string, std::pair<int, int>> AttemptsVsFailures::getFailsAndTotalAttemptsPerStudent()
{
    DatabaseInteracter dbInteracter;

    std::ostringstream failuresQueryStream;
    failuresQueryStream << "SELECT assignments.student_id"
                << " FROM assignments WHERE assignments.sort = 'mistake';";

    std::ostringstream allAttemptsQueryStream;
    allAttemptsQueryStream << "SELECT assignments.student_id FROM assignments WHERE assignments.sort = 'mistake' OR assignments.sort = 'success'"
                            << " OR assignments.sort = 'completion';";

    pqxx::result failuresPerStudent = dbInteracter.executeSelectQuery(failuresQueryStream.str());
    pqxx::result allAttempts = dbInteracter.executeSelectQuery(allAttemptsQueryStream.str());


    std::map<std::string, std::pair<int, int>> AttemptsVsFailures;

    //2 loops is less expensive than doing 1 loop with a lookup to see if the given row is a failure or not
    cout << "allAttempts.size() = " << allAttempts.size() << endl;
    for (auto row : allAttempts)
    {
        string studentId = string(row[0].c_str());
        AttemptsVsFailures[studentId].first += 1;
    }
    
    cout << "failuresPerStudent.size() = " << failuresPerStudent.size() << endl;
    for(auto failureRow : failuresPerStudent)
    {
        string studentId = string(failureRow[0].c_str());
        AttemptsVsFailures[studentId].second += 1;
    }
    return AttemptsVsFailures;
}
