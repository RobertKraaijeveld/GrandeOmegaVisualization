/*
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
#include "DataProcesser/src/CsvParser/CsvParser.h"
#include "DataProcesser/src/YamlParser/YamlParser.h"
#include "DataProcesser/src/Utilities/Utilities.h"
#include "DataProcesser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "DataProcesser/src/Mapper/Mapper.h"
  
using namespace Rice;
using namespace std;



string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaVisualization/lib/DataProcesser/docs/classFiles/assignment_activities_with_class";

ifstream explicitStudentsStream = ifstream("DataProcesser/docs/explicitStudents.yaml", ifstream::in); 

vector<vector<string>> knownEmailsForClasses;	
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;
                   

 //IMPORTANT: Add checks if files actually exist
 //ALSO: give ruby only the results of algorithms etc in words IT can understand; no yamlobjects.

vector<YamlObject> parseAndGetAssignments()
{
	cout << "Parsing assignments with classes..." << endl;
  YamlParser yamlParser(explicitStudentsStream);
	
	vector<YamlObject> assignmentYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);
	return assignmentYamlObjects; 
}

void insertToDB()
{
  cout << "Inserting grades and assignments to DB..." << endl;

	YamlParser yamlParser(explicitStudentsStream);	

  vector<string> assignmentsWithClassesFileNames = Utilities::getListOfNumberedFilesForBaseFile(classFileBaseFileString);
	vector<YamlObject> assignmentsWithClassYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);

	//extract to constant
	yamlParser.file = ifstream("DataProcesser/docs/Grades/grades.yaml");
	vector<YamlObject> gradeYamlObjects = yamlParser.parseYaml();

	DatabaseInteracter dbInteracter;
	dbInteracter.InsertAssignmentYaml(assignmentsWithClassYamlObjects);
	dbInteracter.InsertGradesYaml(gradeYamlObjects);

	cout << "Done inserting grades and assignments to DB"	<< endl;
}

string rubyTest()
{
	return parseAndGetAssignments()[0].values["class"];
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
	

extern "C"

void Init_parsercoupling()
{
  //void callAssignmentParser()
Class rb_c = define_class("Parsercoupling")
    .define_method("parseAndGetAssignments", &parseAndGetAssignments)   
    .define_method("rubyTest", &rubyTest)    		 
    .define_method("insertToDB", &insertToDB);    
}
*/

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
#include "DataProcesser/src/CsvParser/CsvParser.h"
#include "DataProcesser/src/YamlParser/YamlParser.h"
#include "DataProcesser/src/Utilities/Utilities.h"
#include "DataProcesser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "DataProcesser/src/Mapper/Mapper.h"
  
using namespace Rice;
using namespace std;

string classFileBaseFileString = "/home/robert/Documents/Projects/GrandeOmegaVisualization/lib/DataProcesser/docs/classFiles/assignment_activities_with_class";

ifstream explicitStudentsStream = ifstream("DataProcesser/docs/explicitStudents.yaml", ifstream::in); 

vector<vector<string>> knownEmailsForClasses;	
vector<YamlObject> emailYamlObjects;
vector<YamlObject> assignmentYamlObjects;
                   

 //IMPORTANT: Add checks if files actually exist
 //ALSO: give ruby only the results of algorithms etc in words IT can understand; no yamlobjects.
vector<YamlObject> parseAndGetAssignments()
{
	cout << "Parsing assignments with classes..." << endl;
  YamlParser yamlParser(explicitStudentsStream);

	vector<YamlObject> assignmentYamlObjects = yamlParser.parseListOfFiles(classFileBaseFileString);
	return assignmentYamlObjects;
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

string testRuby()
{
	return parseAndGetAssignments()[0].values["class"];
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

void Init_dataprocesser()
{
  Class rb_c = define_class("Dataprocesser")
	  .define_method("parseAndGetAssignments", &parseAndGetAssignments)   
    .define_method("testRuby", & testRuby)    		 
    .define_method("insertToDB", &insertToDB);    
}



