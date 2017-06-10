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
#include "NightOnlyCompletionsVsGradeClassification.h"

#include <pqxx/pqxx>
#include <memory>
#include <map>
#include <vector>


std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> NightOnlyCompletionsVsGradeClassification::getTrainingData()
{
    //Opposite kind of filter to make sure training data != input data
    std::shared_ptr<ITimeFilter> dayOnlyFilter(new DayOnlyFilter());

    ExcersiseCompletionAndGradesClustering trainingExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, dayOnlyFilter);

    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClustersOfFilteredData =
                    trainingExcersiseCompletionAndGradesClusteringVisualization.getExcersiseCompletionAndGradesClusters();

    return trainingClustersOfFilteredData;
}

 
std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> NightOnlyCompletionsVsGradeClassification::getClassifiedData()
{
    //Getting basic input data
    std::shared_ptr<NightOnlyFilter> nightOnlyFilter (new NightOnlyFilter());    
    ExcersiseCompletionAndGradesClustering baseExcersiseCompletionAndGradesClusteringVisualization(gradeFilter, assignmentIntervalFilter, nightOnlyFilter);

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
