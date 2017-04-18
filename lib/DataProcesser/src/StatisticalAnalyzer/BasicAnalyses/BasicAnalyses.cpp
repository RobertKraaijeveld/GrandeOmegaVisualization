#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "../../YamlParser/YamlObject.h"
#include "../../YamlParser/YamlConverter.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/Utilities.h"
#include "BasicAnalyses.h"

using namespace std;

vector<pair<string, int>> BasicAnalyses::getGradeAvgPerClass()
{
    vector<pair<string, int>> returnValues;

    //make multiline
    //ND is a dutch abreviation for meaning 'did not take test'
    string query = "SELECT DISTINCT grades.grade, assignments.class, grades.student_id FROM assignments, grades WHERE assignments.student_id = grades.student_id AND grades.grade <> 'ND' AND assignments.class <> 'tester';";
    DatabaseInteracter dbInteracter;
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
        returnValues.push_back(make_pair(it->first, averageClassGrade));
    }
    return returnValues;
}

map<string, vector<int>> BasicAnalyses::getGradesWithSameClass(multimap<string, int> classesAndGrades) 
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


