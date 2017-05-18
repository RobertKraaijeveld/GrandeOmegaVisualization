#include "IVisualization.h"
#include "GradeAvgsPerClass.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"

#include <vector>

//SPlit it up
std::vector<std::pair<std::string, int>> GradeAvgsPerClass::getGradeAvgsPairs()
{
    std::vector<std::pair<std::string, int>> returnValues;

    pqxx::result averagesPerClassRows = getClassAveragesRows();

    for (auto row : averagesPerClassRows)
    {
        auto rowGradeAverage = row[0].c_str();
        std::string rowClass = string(row[1].c_str());
        returnValues.push_back(make_pair((rowClass), atoi(rowGradeAverage)));
    }
    return returnValues;
}

pqxx::result GradeAvgsPerClass::getClassAveragesRows()
{
    DatabaseInteracter dbInteracter;

    std::ostringstream queryStream;
    queryStream << "SELECT avg(grades.grade), assignments.class"
                << " FROM assignments, grades WHERE assignments.student_id = grades.student_id"
                << " AND assignments.class != 'tester' GROUP BY assignments.class;";
    std::string query = queryStream.str();

    //NOTE: no filtering necessary or helpfull here
    return dbInteracter.executeSelectQuery(queryStream.str());
}

std::string GradeAvgsPerClass::getVisualizationAsJSON()
{
    std::vector<std::pair<std::string, int>> gradesAvgsPerClass = getGradeAvgsPairs();
    std::string pairs = JSONEncoder::pairsToJson(gradesAvgsPerClass);
    return pairs;
}