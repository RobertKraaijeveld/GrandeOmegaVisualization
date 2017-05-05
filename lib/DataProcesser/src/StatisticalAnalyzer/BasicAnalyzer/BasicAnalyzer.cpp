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
    string query = "SELECT student_id, grade FROM grades WHERE grade != 'ND';";   
    auto allStudentIdsAndGrades = dbInteracter.executeSelectQuery(query); 

    for(auto idAndGrade: allStudentIdsAndGrades) 
    {
        string id = string(idAndGrade[0].c_str());
        string grade = string(idAndGrade[1].c_str());

        //Do something with NDs elsewhere, or cast it to a int value
        if(excersiseAmountPerStudent.count(id) != 0)
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

    //Duplication
    int studentIdColumnIndex = 0;
    int timestampColumnIndex = 1;        
    FilterQueryColumnIndexes filterIndexes (studentIdColumnIndex, timestampColumnIndex);
    filter.queryIndexes = filterIndexes;
    
    /*
    vector<pqxx::result::tuple> filteredRows = filter.getFilteredQueryRows(query);

    for(auto row: filteredRows)
    {
        string occurenceStudentIdStr = string(row[0].c_str());
        returnMap[occurenceStudentIdStr] = returnMap[occurenceStudentIdStr] + 1; 
    }
    */
    return returnMap;
}










//Still coupling because each new filter kind needs to be added as a method.
vector<pqxx::result::tuple> BasicAnalyzer::getFilteredQueryRows(std::string& query, vector<pqxx::result::tuple> gradeFilteredRows)
{
    vector<pqxx::result::tuple> assignmentTimeFilteredRows = filter.getRowsWithValidAssignmentTimes(gradeFilteredRows);
    return gradeFilteredRows;
} //the action of returning itself fucks it up; dont know why

map<string, pair<int, int>> BasicAnalyzer::getGradesAndSuccessRates()
{
    map<string, pair<int, int>> returnMapOfPairs;
    DatabaseInteracter dbInteracter;
    
    //Select count does not work with filtering ><
    std::ostringstream queryStream;
    queryStream << "SELECT assignments.student_id, grades.grade, assignments.creation_timestamp"
                << " FROM assignments, grades WHERE assignments.sort != 'Failure'"
                << " AND assignments.student_id = grades.student_id;";

    int studentIdColumnIndex = 0;
    int timestampColumnIndex = 2;    
    filter.queryIndexes = FilterQueryColumnIndexes (studentIdColumnIndex, timestampColumnIndex);

    pqxx::result testquery = dbInteracter.executeSelectQuery(queryStream.str());
    vector<pqxx::result::tuple> gradetest = filter.getRowsWithValidGradePercentile(testquery); 
    vector<pqxx::result::tuple> assignmentstest = filter.getRowsWithValidAssignmentTimes(gradetest); 

    string query = queryStream.str();
    vector<pqxx::result::tuple> filteredStudentSuccessCountsAndGrades = getFilteredQueryRows(query, gradetest);        

    //forloop is fault?
    for(int i = 0; i < filteredStudentSuccessCountsAndGrades.size(); i++)
    {
        auto y = filteredStudentSuccessCountsAndGrades.at(i);
        cout << "filteredStudentSuccessCountsAndGrades = " << y[0].c_str() << endl; 
    }

    /*
    for(auto row: filteredStudentSuccessCountsAndGrades)
    {
        cout << "row[0].c_str() = " << row[0].c_str() << endl;
        string studentIdStr = string(row[0].c_str());
        cout << "studentIdStr " << studentIdStr << endl;

        int newSuccesRate = returnMapOfPairs[studentIdStr].second + 1; 
        cout << "newSuccesRate " << newSuccesRate << endl;

        int gradeOfRow = stoi(row[1].c_str());
        cout << "gradeOfRow " << gradeOfRow << endl;

        returnMapOfPairs[studentIdStr] = make_pair(gradeOfRow, newSuccesRate); 
    }*/
    return returnMapOfPairs;
}















vector<pair<string, int>> BasicAnalyzer::getGradeAvgPerClass()
{
    vector<pair<string, int>> returnValues;

    DatabaseInteracter dbInteracter;
    
    std::ostringstream queryStream;
    //note: ND is a dutch abreviation for 'Niet Deelgenomen', meaning 'did not attend'
     queryStream << "SELECT DISTINCT CAST(grades.grade AS int), assignments.class, grades.student_id"
                 << " FROM assignments, grades WHERE assignments.student_id = grades.student_id"
                 << " AND assignments.class != 'tester';";

    string query = queryStream.str();

    //Duplication. ALSO INCORRECT; MAKE FILTER HAVE OPTIONALS
    int studentIdColumnIndex = 0;
    int timestampColumnIndex = 2;        
    FilterQueryColumnIndexes filterIndexes (studentIdColumnIndex, timestampColumnIndex);
    
    /*
    vector<pqxx::result::tuple> filteredGradeCountsPerClass = filter.getFilteredQueryRows(queryStream.str());

    multimap<string, int> classesAndGrades;    
    for(auto row: filteredGradeCountsPerClass)
    {
        auto rowGrade = row[0].c_str();        
        string rowClass = string(row[1].c_str());
        classesAndGrades.insert(make_pair(rowClass, atoi(rowGrade)));
    }  

    map<string, vector<int>> gradesWithSameClass = getGradesWithSameClass(classesAndGrades);
    map<string, vector<int>>::iterator it;
    for(it = gradesWithSameClass.begin(); it != gradesWithSameClass.end(); ++it)
    {
        int averageClassGrade = Utilities::computeAverage(it->second);
        
        //ugly literal for frontend
        returnValues.push_back(make_pair("class no. " + it->first, averageClassGrade));
    }
    */
    return returnValues;
}

map<string, vector<int>> BasicAnalyzer::getGradesWithSameClass(multimap<string, int> classesAndGrades) 
{
    map<string, vector<int>> gradesWithSameClass;

    multimap<string, int>::iterator it;
    for(it = classesAndGrades.begin(); it != classesAndGrades.end(); ++it)
    {
        string currentClass = it->first;
        vector<int> gradesForCurrentClass;
        
        auto rangeForCurrentClass = classesAndGrades.equal_range(currentClass);
        for(multimap<string, int>::iterator currClassIt = rangeForCurrentClass.first; currClassIt != rangeForCurrentClass.second; ++currClassIt)
        {
            gradesForCurrentClass.push_back(currClassIt->second);
        }
        gradesWithSameClass.insert(make_pair(currentClass, gradesForCurrentClass));
    }
    return gradesWithSameClass;
}


