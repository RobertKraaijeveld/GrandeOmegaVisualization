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


#include "DataProcesser/src/CsvParser/CsvParser.h"
#include "DataProcesser/src/YamlParser/YamlParser.h"
#include "DataProcesser/src/Utilities/Utilities.h"
#include "DataProcesser/src/Utilities/UtcTime.h"
#include "DataProcesser/src/Utilities/UtcReader.h"
#include "DataProcesser/src/DatabaseInteracter/DatabaseInteracter.h"
#include "DataProcesser/src/Mapper/Mapper.h"
#include "RubyToCppConverters.h"

#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/IVisualization.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/GradeAndExcersiseSuccesses.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/ExcersiseCompletionAndGradesClustering.h"
#include "DataProcesser/src/StatisticalAnalyzer/Visualizations/GradeAvgsPerClass.h"

#include "DataProcesser/src/StatisticalAnalyzer/Filter/IFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/GradePercentageFilter.h"
#include "DataProcesser/src/StatisticalAnalyzer/Filter/AssignmentIntervalFilter.h"

#include "DataProcesser/src/StatisticalAnalyzer/Regression/SimpleLinearRegression.h"
#include "DataProcesser/src/StatisticalAnalyzer/Regression/IRegression.h"
#include "DataProcesser/src/StatisticalAnalyzer/GenericVector/GenericVector.h"

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

string getSuccesRate(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);

	//These shared ptrs need to be more polymorphic.
	std::shared_ptr<GradePercentageFilter> gradeFilter (new GradePercentageFilter(filterContext));
	std::shared_ptr<AssignmentIntervalFilter> assignmentIntervalFilter (new AssignmentIntervalFilter(filterContext));
	
	std::unique_ptr<IVisualization> visualization(new GradeAndExcersiseSuccesses(gradeFilter, assignmentIntervalFilter));
	return visualization->getVisualizationAsJSON();
}

string getKMeans(double upperPercentageOfGradesToBeSelected)
{
	FilterContext filterContext = getFilterContext(upperPercentageOfGradesToBeSelected);

	//These shared ptrs need to be more polymorphic.
	std::shared_ptr<GradePercentageFilter> gradeFilter (new GradePercentageFilter(filterContext));
	std::shared_ptr<AssignmentIntervalFilter> assignmentIntervalFilter (new AssignmentIntervalFilter(filterContext));
	
	std::unique_ptr<IVisualization> visualization(new ExcersiseCompletionAndGradesClustering(gradeFilter, assignmentIntervalFilter));
	return visualization->getVisualizationAsJSON();
}

string getGradeAvgPerClass()
{
	std::unique_ptr<IVisualization> visualization(new GradeAvgsPerClass());
	return visualization->getVisualizationAsJSON();
}

string getLinearRegression(vector<float> xValues)
{
	vector<pair<float, float>> pairs = floatVectorToPairVector(xValues);

	std::unique_ptr<IRegression> linearRegression(new SimpleLinearRegression(pairs));
	return linearRegression->getRegressionAsJSON();
}


//Using C compiler and removing name-mangling for Ruby
extern "C"
void Init_dataprocesser()
{
	Class rb_c = define_class("Dataprocesser")
					 .define_method("parseAndGetGrades", &parseAndGetGrades)
					 .define_method("parseAndGetAssignments", &parseAndGetAssignments)
					 .define_method("insertToDB", &insertToDB)
					 .define_method("getKMeans", &getKMeans)
					 .define_method("getSuccesRate", &getSuccesRate)
					 .define_method("getGradeAvgPerClass", &getGradeAvgPerClass)
					 .define_method("getLinearRegression", &getLinearRegression);
}
