#include "rice/Class.hpp"
#include <iostream> 
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <time.h>  
#include "Parser/src/CsvParser/CsvParser.h"
#include "Parser/src/YamlParser/YamlParser.h"
#include "Parser/src/Utilities/Utilities.h"
#include "Parser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "Parser/src/Mapper/Mapper.h"

using namespace Rice;
using namespace std;

//make uppercase :D
//make paths generic/relative for testing purposes
string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaProject/Parser/Parser/docs/classFiles/assignment_activities_with_class";
string assignmentsBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaProject/GrandeOmegaRailsPrj/ext/Parser/docs/assignments/assignment_activities";

ifstream classesFile ("/home/robert/Documents/Projects/GrandeOmegaProject/GrandeOmegaRailsPrj/ext/Parser/emailsAndClasses.csv", ifstream::in);
ifstream explicitStudentsStream = ifstream("explicitStudents.yaml", ifstream::in); 

ofstream yamlFileForGradesOut = ofstream("docs/Grades/grades.yaml");
ifstream yamlFileForGradesIn = ifstream("docs/Grades/grades.yaml");

vector<vector<string>> knownEmailsForClasses;	
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;
                   

//add check if is already set?
void parseYamlAndCsvs()
{
  YamlParser yamlParser(explicitStudentsStream);
	
  vector<vector<string>> knownEmailsForClasses = CsvParser::parseClassMatrix(classesFile);	
	vector<YamlObject> emailYamlObjects = yamlParser.parseYaml();
	vector<YamlObject> assignmentYamlObjects = yamlParser.parseListOfFiles(assignmentsBaseFileString);
}


//TODO: ADD FILE EXCERPTION CHECKING
//ALSO: make things like emailYamlObjects static vars here
//ALSO: MAKE PATHS EITHER RELATIVE OR USE STATIC VARS
void callAssignmentParser()
{
  cout << "Starting to parse assingment files and adding classes" << endl;
  parseYamlAndCsvs();

  Mapper mapper (emailYamlObjects, assignmentYamlObjects, knownEmailsForClasses);	
	mapper.createYamlFilesWithClasses();
}

void callGradesParser()
{
  cout << "Mapping grades (includes parsing assignments to get student_ids)" << endl;
  parseYamlAndCsvs();

  Mapper mapper (emailYamlObjects, assignmentYamlObjects, knownEmailsForClasses); 

  mapper.createGradesYamlFile(yamlFileForGradesOut);
	cout << "done" << endl;   
}

void insertGradesToDB()
{
  cout << "Inserting grades to DB" << endl;
/*
	YamlParser yamlParser(explicitStudentsStream);	

  vector<string> assignmentsWithClassesFileNames = Utilities::getListOfNumberedFilesForBaseFile(classFileBaseFileString);
  //extract
	vector<YamlObject> assignmentsWithClassYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);

	yamlParser.file = yamlFileForGradesIn;
	vector<YamlObject> gradeYamlObjects = yamlParser.parseYaml();

	DatabaseInteracter dbInteracter;
	//dbInteracter.InsertAssignmentYaml(assignmentsWithClassYamlObjects);
	dbInteracter.InsertGradesYaml(gradeYamlObjects);
*/
	cout << "done"	<< endl;
}


  /*
	Backward assignments have only 1 succes.
	Other ones either succeed or fail step-by-step.
	The pointers in the yaml dont mean much.
	CI and testing will award you extra points.
	Only cluster on measurements, not on ids.
	Check not filled ins manually with mohammed.
	Include libs
	Get GrandeOmega to run
	*/

extern "C"

void Init_parsercoupling()
{
  //void callAssignmentParser()
  Class rb_c = define_class("Parsercoupling")
    .define_method("callGradesParser", &callGradesParser)
    .define_method("callAssignmentParser", &callAssignmentParser)    
    .define_method("insertGradesToDB", &insertGradesToDB);    
}



