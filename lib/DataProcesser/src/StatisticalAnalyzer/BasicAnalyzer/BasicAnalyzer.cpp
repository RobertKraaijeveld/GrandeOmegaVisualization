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

//check if still correct; nein
map<string, pair<int, int>> BasicAnalyzer::getAmountOfExercisesCompletedAndGradesPerStudent()
{
    //note that not all students have grades
    map<string, pair<int, int>> amountOfExercisesAndGradePerStudent; 

    map<string, int> excersiseAmountPerStudent = getAmountOfCompletedExcersisesPerStudent();
    
    DatabaseInteracter dbInteracter;
    string query = "SELECT student_id, grade FROM grades;"; 

    //No guarantee that both struct indexes are set... catch exceptions for this and for incorrect indexes!!!
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    filter.queryIndexes = queryIndexes;

    pqxx::result allStudentIdsAndGradesUnFiltered = dbInteracter.executeSelectQuery(query); 
    vector<pqxx::result::tuple> filteredIdsAndGrades = filter.getRowsWithValidGradePercentile(allStudentIdsAndGradesUnFiltered);


    for(auto idAndGrade: filteredIdsAndGrades) 
    {
        string id = string(idAndGrade[0].c_str());
        string grade = string(idAndGrade[1].c_str());

        if(excersiseAmountPerStudent.count(id) == 1)
        {
            pair<int, int> excersiseAmountAndGrade = make_pair(excersiseAmountPerStudent[id], atoi(grade.c_str()));
            amountOfExercisesAndGradePerStudent.insert(make_pair(id, excersiseAmountAndGrade));
        }
    }       
    return amountOfExercisesAndGradePerStudent;
}

map<string, int> BasicAnalyzer::getAmountOfCompletedExcersisesPerStudent()
{
    map<string, int> returnMap;
    DatabaseInteracter dbInteracter;

    string query = "SELECT student_id, creation_timestamp FROM assignments WHERE sort = 'completion';";

    //No guarantee that both struct indexes are set... catch exceptions for this and for incorrect indexes!!!
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 1;    
    filter.queryIndexes = queryIndexes;

    //extract to a method
    pqxx::result unfilteredRows = dbInteracter.executeSelectQuery(query);
    vector<pqxx::result::tuple> rowsFilteredOnGradePercentile = filter.getRowsWithValidGradePercentile(unfilteredRows);
    vector<pqxx::result::tuple> fullyFilteredRows = filter.getRowsWithValidAssignmentTimes(rowsFilteredOnGradePercentile);        
    
    for(auto row: fullyFilteredRows)
    {
        string occurenceStudentIdStr = string(row[0].c_str());
        returnMap[occurenceStudentIdStr] = returnMap[occurenceStudentIdStr] + 1; 
    }
    return returnMap;
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

vector<pair<string, int>> BasicAnalyzer::getGradeAvgPerClass()
{
    vector<pair<string, int>> returnValues;

    DatabaseInteracter dbInteracter;
    
    std::ostringstream queryStream;
    //note: ND is a dutch abreviation for 'Niet Deelgenomen', meaning 'did not attend'
     queryStream << "SELECT avg(grades.grade), assignments.class"
                 << " FROM assignments, grades WHERE assignments.student_id = grades.student_id"
                 << " AND assignments.class != 'tester' GROUP BY assignments.class";
    string query = queryStream.str();

    //no filtering necessary or helpfull here    
    pqxx::result averageAndClassRows = dbInteracter.executeSelectQuery(queryStream.str());

    for(auto row: averageAndClassRows)
    {
        auto rowGradeAverage = row[0].c_str();        
        string rowClass = string(row[1].c_str());
        returnValues.push_back(make_pair(("class no. " + rowClass), atoi(rowGradeAverage)));
    }  
    return returnValues;
}