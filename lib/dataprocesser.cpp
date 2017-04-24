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
#include "DataProcesser/src/StatisticalAnalyzer/BasicAnalyses/BasicAnalyses.h" 
#include "DataProcesser/src/StatisticalAnalyzer/KMeans/KMeansController.h"  
 

using namespace Rice;
using namespace std;

string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaVisualization/lib/DataProcesser/docs/classFiles/assignment_activities_with_class";

ifstream explicitStudentsStream = ifstream("DataProcesser/docs/explicitStudents.yaml", ifstream::in); 

vector<vector<string>> knownEmailsForClasses;	
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;
                   

string utcTimeTest()
{
	//catch exception here instead to control wht is sent to ruby. also, unittest the utc stuff
	string test = "HAHA-12-16 19:58:29.777448";
	UtcTime utcTime = UtcReader::toUtcTime(test);
	return utcTime.ToString();
}

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

string getKMeansAsJSON()
{
	int dataDimension = 2;
	int bestClusterAmount = 19; //tested manually with elbow method
	int iterationAmount = 100;

	auto gradesAndExcersisePerStudent = BasicAnalyses::getGradesAndAmountOfExercisesStartedPerStudent();
	KMeansController kmController (gradesAndExcersisePerStudent, iterationAmount, bestClusterAmount, dataDimension);
	kmController.run();
	string clustersAsJSON = JSONEncoder::clustersToJSON(kmController.finalClusters); 
	return clustersAsJSON;
}

string getAmountOfStartedExcersisesPerStudentAsJSON()
{
	map<string, int> amountOfStartedExcersisesPerStudent = BasicAnalyses::getAmountOfStartedExcersisesPerStudent();
	string asJSON = JSONEncoder::mapToJson(amountOfStartedExcersisesPerStudent);
	return asJSON;
} 

string getGradeAvgPerClassAsJSON()
{
	vector<pair<string, int>> gradesAvgsPerClass = BasicAnalyses::getGradeAvgPerClass();
	string pairsAsJSON = JSONEncoder::pairsToJson(gradesAvgsPerClass);   
	return pairsAsJSON;  
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
	.define_method("utcTimeTest", &utcTimeTest)  
	.define_method("parseAndGetAssignments", &parseAndGetAssignments)
    .define_method("getKMeansAsJSON", &getKMeansAsJSON)    		 		   
    .define_method("getAmountOfStartedExcersisesPerStudentAsJSON", &getAmountOfStartedExcersisesPerStudentAsJSON)    		 	
    .define_method("getGradeAvgPerClassAsJSON", &getGradeAvgPerClassAsJSON)  
    .define_method("insertToDB", &insertToDB);    
}



