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



string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaVisualization/ext/Parser/docs/classFiles/assignment_activities_with_class";

ifstream explicitStudentsStream = ifstream("Parser/docs/explicitStudents.yaml", ifstream::in); 

vector<vector<string>> knownEmailsForClasses;	
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;
                   

 //IMPORTANT: Add checks if files actually exist
 //ALSO: give ruby only the results of algorithms etc in words IT can understand; no yamlobjects.
void parseAssignments()
{
	cout << "Parsing assignments with classes..." << endl;
  YamlParser yamlParser(explicitStudentsStream);
	vector<YamlObject> assignmentYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);
}

void insertToDB()
{
  cout << "Inserting grades and assignments to DB..." << endl;

	YamlParser yamlParser(explicitStudentsStream);	

  vector<string> assignmentsWithClassesFileNames = Utilities::getListOfNumberedFilesForBaseFile(classFileBaseFileString);
	vector<YamlObject> assignmentsWithClassYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);

	//extract to constant
	yamlParser.file = ifstream("Parser/docs/Grades/grades.yaml");
	vector<YamlObject> gradeYamlObjects = yamlParser.parseYaml();

	DatabaseInteracter dbInteracter;
	dbInteracter.InsertAssignmentYaml(assignmentsWithClassYamlObjects);
	dbInteracter.InsertGradesYaml(gradeYamlObjects);

	cout << "Done inserting grades and assignments to DB"	<< endl;
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
    .define_method("parseAssignments", &parseAssignments)    
    .define_method("insertToDB", &insertToDB);    
}



