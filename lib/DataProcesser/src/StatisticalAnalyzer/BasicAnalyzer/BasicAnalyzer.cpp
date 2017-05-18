#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include "../../YamlParser/YamlObject.h"
#include "../../YamlParser/YamlConverter.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/Utilities.h"
#include "../../Utilities/UtcReader.h" 
#include "../../Utilities/UtcTime.h"
#include "BasicAnalyzer.h"
#include "AnalysisFilter.h"

using namespace std;

map<string, int> BasicAnalyzer::getExceriseDateTimeMeasurements()
{

}

map<string, pair<int, int>> BasicAnalyzer::getGradesAndSuccessRates()
{
    map<string, pair<int, int>> returnMapOfPairs;
    DatabaseInteracter dbInteracter;
    
    std::ostringstream queryStream;
    queryStream << "SELECT assignments.student_id, grades.grade, assignments.creation_timestamp"
                << " FROM assignments, grades WHERE assignments.sort != 'Failure'"
                << " AND assignments.student_id = grades.student_id;";

    //No guarantee that both struct indexes are set... catch exceptions for this and for incorrect indexes!!!
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 2;    
    filter.queryIndexes = queryIndexes;

    //extract to a method    
    pqxx::result unfilteredRows = dbInteracter.executeSelectQuery(queryStream.str());
    vector<pqxx::result::tuple> rowsFilteredOnGradePercentile = filter.getRowsWithValidGradePercentile(unfilteredRows);
    vector<pqxx::result::tuple> filteredStudentSuccessCountsAndGrades = filter.getRowsWithValidAssignmentTimes(rowsFilteredOnGradePercentile);        


    for(auto row: filteredStudentSuccessCountsAndGrades)
    {
        //literal for frontend
        string studentIdStr = "Student id " + string(row[0].c_str());

        int newSuccesRate = returnMapOfPairs[studentIdStr].first + 1; 
        int gradeOfRow = stoi(row[1].c_str());

        returnMapOfPairs[studentIdStr] = make_pair(newSuccesRate, gradeOfRow); 
    }
    return returnMapOfPairs;
}