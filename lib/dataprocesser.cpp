#include "rice/Class.hpp"
#include <iostream> 
#include <fstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <time.h>  
#include "DataProcesser/src/CsvParser/CsvParser.h"
#include "DataProcesser/src/YamlParser/YamlParser.h"
#include "DataProcesser/src/Utilities/Utilities.h"
#include "DataProcesser/src/Utilities/JSONEncoder.h"
#include "DataProcesser/src/Utilities/UtcTime.h"
#include "DataProcesser/src/Utilities/UtcReader.h"
#include "DataProcesser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "DataProcesser/src/Mapper/Mapper.h"
#include "DataProcesser/src/StatisticalAnalyzer/BasicAnalyzer/BasicAnalyzer.h" 
#include "DataProcesser/src/StatisticalAnalyzer/BasicAnalyzer/AnalysisFilter.h" 
#include "DataProcesser/src/StatisticalAnalyzer/KMeans/KMeansController.h"  
 

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

string getSuccesRate(double upperPercentageOfGradesToBeSelected) 
{
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = upperPercentageOfGradesToBeSelected;
	BasicAnalyzer analyzer (filterer);

	//map<int, pair<int, int>> gradeAndSFRatePerStudent = analyzer.getGradesAndSuccesRates();
	map<int, pair<int, int>> test;
	test.insert(make_pair(0, make_pair(1,2)));
	
	return JSONEncoder::mapToJson(test);
}


string getKMeans(double upperPercentageOfGradesToBeSelected) 
{
	//make optional non-filtered basicanalyzer or provide standard
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
	return clusters;
}

string getAmountOfStartedExcersisesPerStudent()
{
	//make optional non-filtered basicanalyzer or provide standard
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = 100;

	BasicAnalyzer analyzer (filterer);

	map<string, int> amountOfStartedExcersisesPerStudent = analyzer.getAmountOfCompletedExcersisesPerStudent();
	string asJSON = JSONEncoder::mapToJson(amountOfStartedExcersisesPerStudent);
	return asJSON;
} 

string getGradeAvgPerClass()
{
	//make optional non-filtered basicanalyzer or provide standard
	AnalysisFilter filterer;
	filterer.timeBetweenAssignmentsThreshold = TIME_BETWEEN_ASSIGNMENTS_THRESHOLD;
	filterer.upperPercentageOfGradesToBeSelected = 100;

	BasicAnalyzer analyzer (filterer);

	vector<pair<string, int>> gradesAvgsPerClass = analyzer.getGradeAvgPerClass();
	string pairs = JSONEncoder::pairsToJson(gradesAvgsPerClass);   
	return pairs;  
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
	.define_method("parseAndGetGrades", &parseAndGetGrades)  
	.define_method("parseAndGetAssignments", &parseAndGetAssignments)
    .define_method("insertToDB", &insertToDB)  	
    .define_method("getKMeans", &getKMeans)   
    .define_method("getSuccesRate", &getSuccesRate)    		 		 		 		   
    .define_method("getAmountOfStartedExcersisesPerStudent", &getAmountOfStartedExcersisesPerStudent)    		 	
    .define_method("getGradeAvgPerClass", &getGradeAvgPerClass);
}



