#include "ExcersiseCompletionAndGradesClustering.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../Filter/WeekendDayFilter.h"
#include "../Filter/WeekDayFilter.h"
#include "../Classifiers/KNearestNeighbours/KNearestNeighbours.h"
#include "../KMeans/KMeansController.h"
#include "../Point/IClusteringPoint.h"
#include "../../Utilities/JSONEncoder.h"
#include "../../Utilities/Utilities.h"
#include "WeekDayExcersiseCompletionAndGradesClassification.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>


std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> WeekDayExcersiseCompletionAndGradesClassification::getTrainingData()
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

    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClustersOfFilteredData =
                    trainingExcersiseCompletionAndGradesClusteringVisualization.getExcersiseCompletionAndGradesClusters();

    return trainingClustersOfFilteredData;
}


 
std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> WeekDayExcersiseCompletionAndGradesClassification::getClassifiedData()
{
    //Getting basic input data
    ExcersiseCompletionAndGradesClustering baseExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, timeFilter);

    std::map<std::string, std::pair<int, int>> unclassifiedInputData =
        baseExcersiseCompletionAndGradesClusteringVisualization.getAmountOfExercisesCompletedAndGradesPerStudent();

    //using abstract std::shared_ptr<IClusteringPoint> instead of concrete IClusteringPoint because the classifier does not need to know IClusteringPoint specifics :)
    std::vector<std::shared_ptr<IClusteringPoint>> unclassifiedInputDataAsPoints = Utilities::convertMapOfPairsToPoints(unclassifiedInputData);

    //Creating new clusterer with different filter in order to get training clusters/categories
    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClusters = getTrainingData();

    //Finally, running the classifier
    (*classifier).initialize(unclassifiedInputDataAsPoints, trainingClusters);

    return (*classifier).getClassifiedPoints();
}
