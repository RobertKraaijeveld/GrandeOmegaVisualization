#include "../KMeans/KMeansController.h"
#include "../DBSCAN/DBSCAN.h"
#include "../Point/KMeansPoint.h"
#include "../Point/IClusteringPoint.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "../../Utilities/JSONEncoder.h"
#include "../../Utilities/Utilities.h"
#include "ExcersiseCompletionAndGradesClustering.h"
#include "../Filter/IFilter.h"
#include "../Filter/AssignmentIntervalFilter.h"

#include <map>
#include <vector>
#include <iostream>
#include <pqxx/pqxx>

std::string ExcersiseCompletionAndGradesClustering::getVisualizationAsJSON()
{
    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> clusters = getExcersiseCompletionAndGradesClusters();

    return JSONEncoder::clustersToJSON(clusters);
}

std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> ExcersiseCompletionAndGradesClustering::getExcersiseCompletionAndGradesClusters()
{
    std::map<std::string, std::pair<int, int>> excersisesCompletedAndGradePerStudent = getAmountOfExercisesCompletedAndGradesPerStudent();

    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> abstractClusterPointsPtrs;

    int dataDimension = 2;
    int bestClusterAmount = 10; //tested manually
    int iterationAmount = 100;

    //unfortunately needs conversion
    std::vector<std::shared_ptr<KMeansPoint>> excersisesCompletedAndGradePerStudentAsPoints =
        Utilities::convertMapOfPairsToKMeansPoints(excersisesCompletedAndGradePerStudent);
    KMeansController kmController(excersisesCompletedAndGradePerStudentAsPoints, iterationAmount, bestClusterAmount, dataDimension);
    kmController.run();

    abstractClusterPointsPtrs = kmController.getFinalNonEmptyClusters();
    return abstractClusterPointsPtrs;
}

std::map<std::string, std::pair<int, int>> ExcersiseCompletionAndGradesClustering::getAmountOfExercisesCompletedAndGradesPerStudent()
{
    //unclean
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    gradeFilter->setFilterQueryColumnIndexes(queryIndexes);

    std::map<std::string, int> excersiseAmountPerStudent = getAmountOfCompletedExcersisesPerStudent();

    DatabaseInteracter dbInteracter;
    std::string query = "SELECT student_id, grade FROM grades;";

    pqxx::result unfilteredIdsAndGrades = dbInteracter.executeSelectQuery(query);

    std::vector<pqxx::result::tuple> unfilteredIdsAndGradesAsPqxxVector = Utilities::toListOfPqxxTuples(unfilteredIdsAndGrades);
    std::vector<pqxx::result::tuple> idsAndGradesWithinGradePercentage = gradeFilter->filter(unfilteredIdsAndGradesAsPqxxVector);

    return createExcersiseCompletionAmountAndGradesPairs(idsAndGradesWithinGradePercentage, excersiseAmountPerStudent);
}

std::map<std::string, std::pair<int, int>> ExcersiseCompletionAndGradesClustering::createExcersiseCompletionAmountAndGradesPairs(std::vector<pqxx::result::tuple> filteredIdsAndGrades,
                                                                                                                                 std::map<std::string, int> excersiseAmountPerStudent)
{
    std::map<std::string, std::pair<int, int>> amountOfExercisesCompletedAndGradePerStudent;

    for (auto idAndGrade : filteredIdsAndGrades)
    {
        std::string id = string(idAndGrade[0].c_str());
        std::string grade = string(idAndGrade[1].c_str());

        if (excersiseAmountPerStudent.count(id) == 1)
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

    //unclean
    FilterQueryColumnIndexes queryIndexes;
    queryIndexes.studentIdColumnIndex = 0;
    queryIndexes.timestampIndex = 1;
    assignmentIntervalFilter->setFilterQueryColumnIndexes(queryIndexes);
    timeFilter->setFilterQueryColumnIndexes(queryIndexes);

    pqxx::result unfilteredRows = dbInteracter.executeSelectQuery(query);

    std::vector<pqxx::result::tuple> unfilteredRowsAsPqxxVector = Utilities::toListOfPqxxTuples(unfilteredRows);
    std::vector<pqxx::result::tuple> filteredRowsOnGradePercentage = gradeFilter->filter(unfilteredRowsAsPqxxVector);
    std::vector<pqxx::result::tuple> filteredRowsOnDay = timeFilter->filter(filteredRowsOnGradePercentage);
    std::vector<pqxx::result::tuple> filteredRowsOnAssignmentTime = assignmentIntervalFilter->filter(filteredRowsOnDay);

    for (int i = 0; i < filteredRowsOnAssignmentTime.size(); i++)
    {
        std::string occurenceStudentIdStr = std::string(filteredRowsOnAssignmentTime[i][0].c_str());
        returnMap[occurenceStudentIdStr] = returnMap[occurenceStudentIdStr] + 1;
    }
    return returnMap;
}