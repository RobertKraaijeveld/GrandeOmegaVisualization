#include "ExcersiseCompletionAndGradesClustering.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../Filter/WeekendDayFilter.h"
#include "../Filter/WeekDayFilter.h"
#include "../KNearestNeighbours/KNearestNeighbours.h"
#include "../KMeans/KMeansController.h"
#include "../KMeans/CustomTypes/KMeansPoint.h"
#include "../../Utilities/JSONEncoder.h"
#include "../../Utilities/Utilities.h"
#include "WeekDayExcersiseCompletionAndGradesClassification.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>


std::vector<std::vector<KMeansPoint*>> WeekDayExcersiseCompletionAndGradesClassification::getTrainingData()
{
    //The filter for the training clusters is the opposite of the training data for the actual input vals,
    //so that the training set is actually differently categorized than the result.
    std::shared_ptr<ITimeFilter> oppositeKindOfFilter;

    if (filterOnWeekendOnly)
        oppositeKindOfFilter.reset(new WeekDayFilter());
    else
    {
        //if filterOnWeekendOnly is false the actual data to be classified is weekendOnly, so we use the opposite for training data.
        oppositeKindOfFilter.reset(new WeekendDayFilter());
    }

    ExcersiseCompletionAndGradesClustering trainingExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, oppositeKindOfFilter);

    //std::vector<std::vector<IClusteringPoint*>> trainingClustersOfFilteredData =
    //    trainingExcersiseCompletionAndGradesClusteringVisualization.getExcersiseCompletionAndGradesClusters();

   
    //return trainingClustersOfFilteredData;
    auto trainingClusters = trainingExcersiseCompletionAndGradesClusteringVisualization.getExcersiseCompletionAndGradesClusters();
    return trainingClusters;
}



std::vector<std::vector<IClusteringPoint*>> WeekDayExcersiseCompletionAndGradesClassification::getClassifiedData()
{
    //Getting basic input data
    ExcersiseCompletionAndGradesClustering baseExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, timeFilter);

    std::map<std::string, std::pair<int, int>> unclassifiedInputData =
        baseExcersiseCompletionAndGradesClusteringVisualization.getAmountOfExercisesCompletedAndGradesPerStudent();

    //using abstract IClusteringPoint* instead of concrete KMeansPoint because KNN does not need to know KMeansPoint specifics :)
    std::vector<IClusteringPoint*> unclassifiedInputDataAsPoints = Utilities::convertMapOfPairsToPoints(unclassifiedInputData);

    //Creating new clusterer with different filter in order to get training clusters/categories
    //std::vector<std::vector<IClusteringPoint*>> trainingClusters = getTrainingData();
    std::vector<std::vector<KMeansPoint*>> trainingClusters = getTrainingData();


    //Finally, running KNN
    KNearestNeighbours kNearestAlgo(unclassifiedInputDataAsPoints, trainingClusters, 7);

    return kNearestAlgo.getClassifiedPoints();
}

std::string WeekDayExcersiseCompletionAndGradesClassification::getVisualizationAsJSON()
{
    std::vector<std::vector<IClusteringPoint*>> classifiedData = getClassifiedData();

    return JSONEncoder::clustersToJSON(classifiedData);
}