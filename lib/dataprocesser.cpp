#include "rice/Class.hpp"
//#include "rice/Array.hpp"

#include <iostream> 
#include <fstream>
#include <sstream>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <ctime>
#include <time.h>  
#include "DataProcesser/src/CsvParser/CsvParser.h"
#include "DataProcesser/src/YamlParser/YamlParser.h"
#include "DataProcesser/src/Utilities/Utilities.h"
#include "DataProcesser/src/Utilities/UtcTime.h"
#include "DataProcesser/src/Utilities/UtcReader.h"
#include "DataProcesser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "DataProcesser/src/Mapper/Mapper.h"

#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/IVisualization.h" 
#include "DataProcesser/src/StatisticalAnalyzer/Regression/IRegression.h" 
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/GradeAvgsPerClass.h" 
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/CompletedExcersisesPerStudent.h" 

#include "DataProcesser/src/StatisticalAnalyzer/BasicAnalyzer/AnalysisFilter.h" 
#include "DataProcesser/src/StatisticalAnalyzer/KMeans/KMeansController.h"
#include "DataProcesser/src/StatisticalAnalyzer/StatisticalTools/StatisticalTools.h"  
#include "DataProcesser/src/StatisticalAnalyzer/GenericVector/GenericVector.h"    
 
//CLEAN UP HERE

using namespace Rice;
using namespace std;

const double TIME_BETWEEN_ASSIGNMENTS_THRESHOLD = 1.5;

string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaVisualization/lib/DataProcesser/docs/classFiles/assignment_activities_with_class";

ifstream explicitStudentsStream = ifstream("DataProcesser/docs/explicitStudents.yaml", ifstream::in); 

vector<vector<string>> knownEmailsForClasses;	
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;

                   
/*
	PARSING 
*/

 //IMPORTANT: Add checks if files actually exist
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

	//extract to constant
	yamlParser.file = ifstream("DataProcesser/docs/Grades/grades.yaml");
	vector<YamlObject> gradeYamlObjects = yamlParser.parseYaml();

	DatabaseInteracter dbInteracter;
	dbInteracter.InsertAssignmentYaml(assignmentsWithClassYamlObjects);
	dbInteracter.InsertGradesYaml(gradeYamlObjects);
}

/*
	DATA FOR VISUALIZATIONS
*/

string getExcersiseDateTimeMetrics(double upperPercentageOfGradesToBeSelected) 
{
	/*
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = upperPercentageOfGradesToBeSelected;
	BasicAnalyzer analyzer (filterer);

	map<string, int> excersiseDateTimeMeasurements = analyzer.getExceriseDateTimeMeasurements();	
	return JSONEncoder::mapToJson(excersiseDateTimeMeasurements);*/
	return "";
}

string getSuccesRate(double upperPercentageOfGradesToBeSelected) 
{
	/*
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = upperPercentageOfGradesToBeSelected;
	BasicAnalyzer analyzer (filterer);

	map<string, pair<int, int>> gradeAndSuccessRatePerStudent = analyzer.getGradesAndSuccessRates();
	
	return JSONEncoder::mapToJson(gradeAndSuccessRatePerStudent);*/
	return "";	
}

string getKMeans(double upperPercentageOfGradesToBeSelected)
{
	/*
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = upperPercentageOfGradesToBeSelected;
	BasicAnalyzer analyzer (filterer);

	auto gradesAndExcersisePerStudent = analyzer.getAmountOfExercisesCompletedAndGradesPerStudent();

	int dataDimension = 2;  
	int bestClusterAmount = 19; //tested manually with elbow method
	int iterationAmount = 100;

	KMeansController kmController (gradesAndExcersisePerStudent, iterationAmount, bestClusterAmount, dataDimension);
	kmController.run();

	string clusters = JSONEncoder::clustersToJSON(kmController.getFinalNonEmptyClusters()); 
	return clusters;*/
	return "";
} 

string getAmountOfCompletedExcersisesPerStudent(double upperPercentageOfGradesToBeSelected)
{
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = upperPercentageOfGradesToBeSelected;

	std::unique_ptr<IVisualization> visualization(new CompletedExcersisesPerStudent(filterer));  
	return visualization->getVisualizationAsJSON();  
} 

string getGradeAvgPerClass()
{
	std::unique_ptr<IVisualization> visualization(new GradeAvgsPerClass());  
	return visualization->getVisualizationAsJSON();  
}

string getLinearRegression()
{
	//Array xvalues
	//convert Array to vector of float
	//check conversion to float is possible

	/*
	note that the endpoint in the controller does need access to the original points in order to send xValues.
	do we want this linear regression to be generic or not?
	
	Making it generic would mean ENCODING javascript values of a series to JSON (theres methods for that),
	posting it to an controller (can easily be done using params_fetch),
	decoding the JSON to an array (json.parse),
	calling this method with said array
	returning the resulting JSON to the caller on the view
	*/

	return "x";
}

  /*
	Backward assignments have only 1 succes.
	Other ones either succeed or fail step-by-step.
	CI and testing will award you extra points.
	Only cluster on measurements, not on ids.
	Get GrandeOmega to runn
	*/
    
extern "C"

void Init_dataprocesser()
{
  Class rb_c = define_class("Dataprocesser")
	.define_method("getLinearRegression", &getLinearRegression)    
	.define_method("parseAndGetGrades", &parseAndGetGrades)  
	.define_method("parseAndGetAssignments", &parseAndGetAssignments)
    .define_method("insertToDB", &insertToDB)  	
    .define_method("getKMeans", &getKMeans)   
    .define_method("getSuccesRate", &getSuccesRate)    		 		 		 		   
    .define_method("getAmountOfCompletedExcersisesPerStudent", &getAmountOfCompletedExcersisesPerStudent)    		 	
    .define_method("getGradeAvgPerClass", &getGradeAvgPerClass);
}



