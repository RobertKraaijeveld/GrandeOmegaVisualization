#include "rice/Class.hpp"
#include "rice/Array.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <ctime>
#include <time.h>
#include <memory>

#include "DataProcesser/src/CsvParser/CsvParser.h"
#include "DataProcesser/src/YamlParser/YamlParser.h"
#include "DataProcesser/src/Utilities/Utilities.h"
#include "DataProcesser/src/Utilities/UtcTime.h"
#include "DataProcesser/src/Utilities/UtcReader.h"
#include "DataProcesser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "DataProcesser/src/Mapper/Mapper.h"
#include "DataProcesser/src/Utilities/JSONEncoder.h"
#include "RubyToCppConverters.h"

#include "DataProcesser/src/StatisticalAnalyzer/GenericVector/GenericVector.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/IVisualization.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/GradeAndExcersiseSuccesses.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/ExcersiseCompletionAndGradesClustering.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/WeekDayExcersiseCompletionAndGradesClassification.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/NightOnlyCompletionsVsGradeClassification.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/DayOnlyCompletionsVsGradeClassification.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/GradeAvgsPerClass.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/AmountOfStudentsPerClass.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/CorrelationMeasures.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/AttemptsVsFailures.h"

#include "DataProcesser/src/StatisticalAnalyzer/Point/IClusteringPoint.h"
#include "DataProcesser/src/StatisticalAnalyzer/Point/DBScanPoint.h"

#include "DataProcesser/src/StatisticalAnalyzer/DBSCAN/DBSCAN.h"

#include "DataProcesser/src/StatisticalAnalyzer/Classifiers/IClassifier.h"
#include "DataProcesser/src/StatisticalAnalyzer/Classifiers/KNearestNeighbours/KNearestNeighbours.h"
#include "DataProcesser/src/StatisticalAnalyzer/Classifiers/NaiveBayesClassification/NaiveBayesClassification.h"

#include "DataProcesser/src/StatisticalAnalyzer/Filter/IFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/ITimeFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/GradePercentageFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/AssignmentIntervalFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/WeekDayFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/WeekendDayFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/NightOnlyFilter.h"

#include "DataProcesser/src/StatisticalAnalyzer/Regression/IRegression.h"
#include "DataProcesser/src/StatisticalAnalyzer/Regression/SimpleLinearRegression.h"
#include "DataProcesser/src/StatisticalAnalyzer/Regression/LogarithmicLinearRegression.h"

using namespace Rice;
using namespace std;


/**
 This file exposes a facade-like structure, consisting of functions that can be directly called by Ruby.
 This is the only entry-point for any Ruby-code, making sure Ruby only ever uses abstractions of the C++ code.

 */



const double TIME_BETWEEN_ASSIGNMENTS_THRESHOLD = 1.5;

string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaVisualization/lib/DataProcesser/docs/classFiles/assignment_activities_with_class";

ifstream explicitStudentsStream = ifstream("DataProcesser/docs/explicitStudents.yaml", ifstream::in);

vector<vector<string>> knownEmailsForClasses;
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;

/*
	PARSING 
*/

vector<YamlObject> parseAndGetGrades()
{
	YamlParser yamlParser(explicitStudentsStream);
	yamlParser.file = ifstream("DataProcesser/docs/Grades/grades.yaml");
	vector<YamlObject> gradeYamlObjects = yamlParser.parseYaml();

	return assignmentYamlObjects;
}

vector<YamlObject> parseAndGetAssignments()
{
	YamlParser yamlParser(explicitStudentsStream);

	vector<YamlObject> assignmentYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);
	return assignmentYamlObjects;
}

void insertToDB()
{
	YamlParser yamlParser(explicitStudentsStream);

	vector<string> assignmentsWithClassesFileNames = Utilities::getListOfNumberedFilesForBaseFile(classFileBaseFileString);
	vector<YamlObject> assignmentsWithClassYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);

	yamlParser.file = ifstream("DataProcesser/docs/Grades/grades.yaml");
	vector<YamlObject> gradeYamlObjects = yamlParser.parseYaml();

	DatabaseInteracter dbInteracter;
	dbInteracter.InsertAssignmentYaml(assignmentsWithClassYamlObjects);
	dbInteracter.InsertGradesYaml(gradeYamlObjects);
}

/*
	DATA FOR VISUALIZATIONS
*/

FilterContext getFilterContext(double upperPercentage)
{
	FilterContext filterContext;
	filterContext.upperPercentageOfGradesToBeSelected = upperPercentage;
	filterContext.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;

	return filterContext;
}

pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> getGradeFilterAndAssignmentIntervalFilter(FilterContext filterContext)
{
	std::shared_ptr<IFilter> gradeFilter(new GradePercentageFilter(filterContext));
	std::shared_ptr<IFilter> assignmentIntervalFilter(new AssignmentIntervalFilter(filterContext));

	return make_pair(gradeFilter, assignmentIntervalFilter);
}


//AVERAGES
string getGradeAvgPerClass()
{
	std::shared_ptr<IVisualization> gradeAvgsPerClassVisualization(new GradeAvgsPerClass());
	return gradeAvgsPerClassVisualization->getVisualizationAsJSON();
}

string getAmountOfStudentsPerClass()
{
	std::shared_ptr<IVisualization> amountOfStudentsPerClassVisualization(new AmountOfStudentsPerClass());
	return amountOfStudentsPerClassVisualization->getVisualizationAsJSON();
}



//RENAME
string getSuccesRate(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);

	//These shared ptrs need to be more polymorphic.
	pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> gradeFilterAndAssignmentIntervalFilters = getGradeFilterAndAssignmentIntervalFilter(filterContext);
	std::shared_ptr<ITimeFilter> emptyDayFilter(new ITimeFilter());

	std::shared_ptr<IVisualization> gradeAndExcersiseSuccessesVisualization
									(new GradeAndExcersiseSuccesses(gradeFilterAndAssignmentIntervalFilters.first, 
																	gradeFilterAndAssignmentIntervalFilters.second, 
																	emptyDayFilter));

	return gradeAndExcersiseSuccessesVisualization->getVisualizationAsJSON();
}

//CLUSTERINGS
string getKMeans(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);

	pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> gradeFilterAndAssignmentIntervalFilters = getGradeFilterAndAssignmentIntervalFilter(filterContext);		
	std::shared_ptr<ITimeFilter> emptyFilter(new ITimeFilter());

	std::shared_ptr<IVisualization> completionAndGradesClustering 
											 (new ExcersiseCompletionAndGradesClustering(
											  gradeFilterAndAssignmentIntervalFilters.first, 
											  gradeFilterAndAssignmentIntervalFilters.second, 
											  emptyFilter));
	
	return completionAndGradesClustering->getVisualizationAsJSON();
}



//CLASSIFICATIONS
string getWeekdayCompletionsVsGradesClassification(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);

	pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> gradeFilterAndAssignmentIntervalFilters = getGradeFilterAndAssignmentIntervalFilter(filterContext);		
	std::shared_ptr<ITimeFilter> weekDayFilter(new WeekDayFilter());

	std::shared_ptr<IClassifier> knearestClassifier (new KNearestNeighbours);

	//ugly bool switch to make WeekDayExcersiseCompletionAndGradesClassification behave for both week and weekend
	bool filterOnWeekend = false;
	std::shared_ptr<IClassificationVisualization> excersiseCompletionAndGradesClassificationVisualization
									(new WeekDayExcersiseCompletionAndGradesClassification
											(knearestClassifier,
											gradeFilterAndAssignmentIntervalFilters.first, 
											gradeFilterAndAssignmentIntervalFilters.second, 
											weekDayFilter, filterOnWeekend));

	return excersiseCompletionAndGradesClassificationVisualization->getVisualizationAsJSON();
}

//duplication with above
string getWeekendCompletionsVsGradesClassification(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);

	pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> gradeFilterAndAssignmentIntervalFilters = getGradeFilterAndAssignmentIntervalFilter(filterContext);		
	std::shared_ptr<ITimeFilter> weekendFilter(new WeekendDayFilter());

	std::shared_ptr<IClassifier> knearestClassifier (new KNearestNeighbours);	

	//ugly bool switch to make WeekDayExcersiseCompletionAndGradesClassification behave for both week and weekend	
	bool filterOnWeekend = true;
	std::shared_ptr<IClassificationVisualization> excersiseCompletionAndGradesClassificationVisualization
									(new WeekDayExcersiseCompletionAndGradesClassification
											(knearestClassifier,
											gradeFilterAndAssignmentIntervalFilters.first, 
											gradeFilterAndAssignmentIntervalFilters.second, 
											weekendFilter, filterOnWeekend));

	return excersiseCompletionAndGradesClassificationVisualization->getVisualizationAsJSON();
}

string getNightCompletionsVsGradesClassification(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);
	pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> gradeFilterAndAssignmentIntervalFilters = getGradeFilterAndAssignmentIntervalFilter(filterContext);		

	std::shared_ptr<IClassifier> naiveBayesClassifier (new NaiveBayesClassification());
	
	std::shared_ptr<IClassificationVisualization> nightCompletionsVsGradesClassification
												  (new NightOnlyCompletionsVsGradeClassification
												  		(
															naiveBayesClassifier,
															gradeFilterAndAssignmentIntervalFilters.first,
															gradeFilterAndAssignmentIntervalFilters.second
												  		)
												  );

	return nightCompletionsVsGradesClassification->getVisualizationAsJSON(); 
}

string getDayCompletionsVsGradesClassification(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);
	pair<std::shared_ptr<IFilter>, std::shared_ptr<IFilter>> gradeFilterAndAssignmentIntervalFilters = getGradeFilterAndAssignmentIntervalFilter(filterContext);		

	std::shared_ptr<IClassifier> naiveBayesClassifier (new NaiveBayesClassification());
	
	std::shared_ptr<IClassificationVisualization> nightCompletionsVsGradesClassification
												  (new DayOnlyCompletionsVsGradeClassification
												  		(
															naiveBayesClassifier,
															gradeFilterAndAssignmentIntervalFilters.first,
															gradeFilterAndAssignmentIntervalFilters.second
												  		)
												  );

	return nightCompletionsVsGradesClassification->getVisualizationAsJSON(); 
}

/*
Just for fun :)
*/

string getAttemptsVsFailures()
{
	std::shared_ptr<IVisualization> attemptsVsFailuresVisualization(new AttemptsVsFailures());
	return attemptsVsFailuresVisualization->getVisualizationAsJSON();
}


/*
Measures instead of graphs
*/

string getCorrelationMeasures(vector<float> xyValues)
{
	vector<pair<float, float>> pairs = floatVectorToPairVector(xyValues);
	pair<GenericVector, GenericVector> xyVectors = convertPairsToGVs(pairs);

	std::shared_ptr<IVisualization> correlationMeasures(new CorrelationMeasures(xyVectors));
	return correlationMeasures->getVisualizationAsJSON();
}

string getLinearRegression(vector<float> xyValues)
{
	vector<pair<float, float>> pairs = floatVectorToPairVector(xyValues);

	std::shared_ptr<IRegression> linearRegression(new SimpleLinearRegression(pairs));
	return linearRegression->getRegressionAsJSON();
}

string getLogarithmicLinearRegression(vector<float> xyValues)
{
	//LogarithmicLinearRegression
	vector<pair<float, float>> pairs = floatVectorToPairVector(xyValues);

	std::shared_ptr<IRegression> logarithmicLinearRegression(new LogarithmicLinearRegression(pairs));
	return logarithmicLinearRegression->getRegressionAsJSON();
}

string filterOutliers(vector<float> xyValues)
{
	vector<pair<float, float>> pairs = floatVectorToPairVector(xyValues);	
	vector<GenericVector> pairsAsGenericVectors = pairsTo2DGenericVectors(pairs);

	//converting to dbscanpoints
	vector<shared_ptr<DBScanPoint>> inputPointsPtr;
	for(size_t i = 0; i < pairsAsGenericVectors.size(); i++)
	{
		DBScanPoint pointForGVPtr(-1, pairsAsGenericVectors[i]);
		shared_ptr<DBScanPoint> asPtr = make_shared<DBScanPoint>(pointForGVPtr);
		inputPointsPtr.push_back(asPtr);
	}

	//tested manually for this dataset 
	int minNeighbourAmounts = 5;
    float maxRadius = 16.5;

    DBSCAN scanner (inputPointsPtr, minNeighbourAmounts, maxRadius);
    auto abstractClusterPointsPtrs = scanner.getClusteredData();  

	return JSONEncoder::clustersToJSON(abstractClusterPointsPtrs);
}

//Using C compiler and removing name-mangling for Ruby
extern "C" void Init_dataprocesser()
{
	Class rb_c = define_class("Dataprocesser")
					 .define_method("parseAndGetGrades", &parseAndGetGrades)
					 .define_method("parseAndGetAssignments", &parseAndGetAssignments)
					 .define_method("insertToDB", &insertToDB)
					 .define_method("getKMeans", &getKMeans)
					 .define_method("getSuccesRate", &getSuccesRate)
					 .define_method("getWeekdayCompletionsVsGradesClassification", &getWeekdayCompletionsVsGradesClassification)
					 .define_method("getWeekendCompletionsVsGradesClassification", &getWeekendCompletionsVsGradesClassification)
					 .define_method("getGradeAvgPerClass", &getGradeAvgPerClass)
					 .define_method("getAmountOfStudentsPerClass", &getAmountOfStudentsPerClass)
					 .define_method("getLinearRegression", &getLinearRegression)
					 .define_method("getLogarithmicLinearRegression", &getLogarithmicLinearRegression)
					 .define_method("filterOutliers", &filterOutliers)
					 .define_method("getNightCompletionsVsGradesClassification", &getNightCompletionsVsGradesClassification)
					 .define_method("getDayCompletionsVsGradesClassification", &getDayCompletionsVsGradesClassification)		
					 .define_method("getAttemptsVsFailures", &getAttemptsVsFailures)					 					 					 
					 .define_method("getCorrelationMeasures", &getCorrelationMeasures);
					 
}
