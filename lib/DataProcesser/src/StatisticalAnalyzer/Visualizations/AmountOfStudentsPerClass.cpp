#include "IVisualization.h"
#include "AmountOfStudentsPerClass.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"

pqxx::result AmountOfStudentsPerClass::getStudentAmountPerClassQueryResult()
{
    DatabaseInteracter dbInteracter;

    std::ostringstream queryStream;
    queryStream << "SELECT class, COUNT(DISTINCT assignments.student_id)"
                << " FROM assignments, grades WHERE assignments.class != 'tester'"
                << " AND assignments.student_id = grades.student_id AND grades.grade > 0"
                << " GROUP BY assignments.class ORDER BY avg(grades.grade);";
    std::string query = queryStream.str();

    //NOTE: no filtering necessary or helpfull here
    return dbInteracter.executeSelectQuery(queryStream.str());
}

std::string AmountOfStudentsPerClass::getVisualizationAsJSON()
{
    vector<std::pair<string, int>> studentCountPerClass;
    pqxx::result studentAmountPerClassQueryResult = getStudentAmountPerClassQueryResult();

    for (auto const &row : studentAmountPerClassQueryResult)
    {
        std::string classNo = string(row[0].c_str());        
        auto amountOfStudents = row[1].c_str();

        studentCountPerClass.push_back(make_pair((classNo), atoi(amountOfStudents)));
    }
    
    std::string pairsJSON = JSONEncoder::pairsToJson(studentCountPerClass);
    return pairsJSON;
}