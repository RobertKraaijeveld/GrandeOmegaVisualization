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

map<string, pair<int, int>> BasicAnalyzer::getAmountOfExercisesCompletedAndGradesPerStudent()
{
    //note that not all students have grades
    map<string, pair<int, int>> amountOfExercisesAndGradePerStudent; 

    map<string, int> excersiseAmountPerStudent = getAmountOfCompletedExcersisesPerStudent();
    
    DatabaseInteracter dbInteracter;
    std::ostringstream queryStream;

    queryStream << "SELECT student_id, grade FROM grades WHERE grade != 'ND' "   
                << filter.getGradeSortingQuery(getTotalAmountOfGrades()) << ";";
    string query = queryStream.str();   
    
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

int BasicAnalyzer::getTotalAmountOfGrades()
{
    DatabaseInteracter dbInteracter;
    string amountOfGradesQuery = "SELECT COUNT(grade) FROM grades;";

    pqxx::result amountOfGradesQueryResult = dbInteracter.executeSelectQuery(amountOfGradesQuery);

    return stoi(amountOfGradesQueryResult[0][0].c_str());
}



map<string, int> BasicAnalyzer::getAmountOfCompletedExcersisesPerStudent()
{
    map<string, int> returnMap;
    DatabaseInteracter dbInteracter;
    
    auto allStudentOccurences = dbInteracter.executeSelectQuery("SELECT student_id, creation_timestamp FROM assignments WHERE sort = 'completion';");

    //DIRTY CONSTANT
    string previousTime = "0000-12-12 00:00:0.0";
    string currTime;

    for(auto occurenceRow: allStudentOccurences) 
    {
        currTime = string(occurenceRow[1].c_str());
        
        if(filter.isValidAssignmentTime(previousTime, currTime))
        {
            string occurenceStudentIdStr = string(occurenceRow[0].c_str());        
            returnMap[occurenceStudentIdStr] = returnMap[occurenceStudentIdStr] + 1; 
        }
        previousTime = currTime;        
    }    
    return returnMap;
}

vector<pair<string, int>> BasicAnalyzer::getGradeAvgPerClass()
{
    vector<pair<string, int>> returnValues;

    DatabaseInteracter dbInteracter;
    
    std::ostringstream queryStream;
    //note: ND is a dutch abreviation for 'Niet Deelgenomen', meaning 'did not attend'
     queryStream << "SELECT DISTINCT CAST(grades.grade AS int), assignments.class, grades.student_id"
                 << " FROM assignments, grades WHERE assignments.student_id = grades.student_id"
                 << " AND grades.grade != 'ND' AND assignments.class != 'tester' "
                 << filter.getGradeSortingQuery(getTotalAmountOfGrades()) << ";";

    string query = queryStream.str();
    pqxx::result queryResult = dbInteracter.executeSelectQuery(query);

    multimap<string, int> classesAndGrades;    
    for(auto row: queryResult)
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

