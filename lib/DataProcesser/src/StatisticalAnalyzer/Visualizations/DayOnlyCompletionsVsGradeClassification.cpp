#include "ExcersiseCompletionAndGradesClustering.h"
#include "../Filter/IFilter.h"
#include "../Filter/ITimeFilter.h"
#include "../Filter/NightOnlyFilter.h"
#include "../Filter/DayOnlyFilter.h"
#include "../Classifiers/KNearestNeighbours/KNearestNeighbours.h"
#include "../KMeans/KMeansController.h"
#include "../Point/IClusteringPoint.h"
#include "../../Utilities/JSONEncoder.h"
#include "../../Utilities/Utilities.h"
#include "DayOnlyCompletionsVsGradeClassification.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>


std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> DayOnlyCompletionsVsGradeClassification::getTrainingData()
{
    //Opposite kind of filter to make sure training data != input data
    std::shared_ptr<ITimeFilter> nightOnlyFilter(new NightOnlyFilter());

    ExcersiseCompletionAndGradesClustering trainingExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, nightOnlyFilter);

    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClustersOfFilteredData =
                    trainingExcersiseCompletionAndGradesClusteringVisualization.getExcersiseCompletionAndGradesClusters();

    return trainingClustersOfFilteredData;
}

 
std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> DayOnlyCompletionsVsGradeClassification::getClassifiedData()
{
    //Getting basic input data
    std::shared_ptr<DayOnlyFilter> dayOnlyFilter (new DayOnlyFilter());    
    ExcersiseCompletionAndGradesClustering baseExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, dayOnlyFilter);

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
