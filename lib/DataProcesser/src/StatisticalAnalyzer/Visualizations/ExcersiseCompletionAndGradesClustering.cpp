#include "../KMeans/KMeansController.h"
#include "../KMeans/CustomTypes/KMeansPoint.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"
#include "ExcersiseCompletionAndGradesClustering.h"

#include <map>
#include <vector>
#include <pqxx/pqxx>

std::string ExcersiseCompletionAndGradesClustering::getVisualizationAsJSON()
{
    std::vector<std::vector<KMeansPoint>> clusters = getExcersiseCompletionAndGradesClusters();

    return JSONEncoder::clustersToJSON(clusters);
}

std::vector<std::vector<KMeansPoint>> ExcersiseCompletionAndGradesClustering::getExcersiseCompletionAndGradesClusters()
{
	int dataDimension = 2;  
	int bestClusterAmount = 19; //tested manually with elbow method
	int iterationAmount = 100;

    std::map<std::string, std::pair<int, int>> excersisesCompletedAndGradePerStudent = getAmountOfExercisesCompletedAndGradesPerStudent(); 

	KMeansController kmController (excersisesCompletedAndGradePerStudent, iterationAmount, bestClusterAmount, dataDimension);
	kmController.run();

	return kmController.getFinalNonEmptyClusters(); 
}

std::map<std::string, std::pair<int, int>> ExcersiseCompletionAndGradesClustering::getAmountOfExercisesCompletedAndGradesPerStudent()
{
    //note that not all students have grades
    std::map<std::string, int> excersiseAmountPerStudent = getAmountOfCompletedExcersisesPerStudent();

    DatabaseInteracter dbInteracter;
    std::string query = "SELECT student_id, grade FROM grades;"; 

    //this shit sucks
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    filter.queryIndexes = queryIndexes;

    pqxx::result unfilteredIdsAndGrades = dbInteracter.executeSelectQuery(query); 
    std::vector<pqxx::result::tuple> idsAndGradesWithinGradePercentage = filter.getRowsWithValidGradePercentage(unfilteredIdsAndGrades);

    return createExcersiseCompletionAmountAndGradesPairs(idsAndGradesWithinGradePercentage, excersiseAmountPerStudent);
}

std::map<std::string, std::pair<int, int>> ExcersiseCompletionAndGradesClustering::createExcersiseCompletionAmountAndGradesPairs
                                                                         (std::vector<pqxx::result::tuple> filteredIdsAndGrades,
                                                                          std::map<std::string, int> excersiseAmountPerStudent)
{
    std::map<std::string, std::pair<int, int>> amountOfExercisesCompletedAndGradePerStudent;

    for(auto idAndGrade: filteredIdsAndGrades) 
    {
        std::string id = string(idAndGrade[0].c_str());
        std::string grade = string(idAndGrade[1].c_str());

        if(excersiseAmountPerStudent.count(id) == 1)
        {
            std::pair<int, int> excersiseAmountAndGradePair = make_pair(excersiseAmountPerStudent[id], atoi(grade.c_str()));
            amountOfExercisesCompletedAndGradePerStudent.insert(make_pair(id, excersiseAmountAndGradePair));
        }
    }
    return amountOfExercisesCompletedAndGradePerStudent;
}

std::map<std::string, int> ExcersiseCompletionAndGradesClustering::getAmountOfCompletedExcersisesPerStudent()
{
    std::map<std::string, int> returnMap;

    DatabaseInteracter dbInteracter;
    std::string query = "SELECT student_id, creation_timestamp FROM assignments WHERE sort = 'completion';";

    //this shit sucks
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 1;
    filter.queryIndexes = queryIndexes;

    pqxx::result unfilteredRows = dbInteracter.executeSelectQuery(query);
    std::vector<pqxx::result::tuple> filteredRowsOnGradePercentage = filter.getRowsWithValidGradePercentage(unfilteredRows);
    std::vector<pqxx::result::tuple> filteredRowsOnAssignmentTime = filter.getRowsWithValidAssignmentTimes(filteredRowsOnGradePercentage);    

    for (int i = 0; i < filteredRowsOnAssignmentTime.size(); i++)
    {
        std::string occurenceStudentIdStr = std::string(filteredRowsOnAssignmentTime[i][0].c_str());
        returnMap[occurenceStudentIdStr] = returnMap[occurenceStudentIdStr] + 1;
    }
    return returnMap;
}