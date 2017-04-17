#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "../Mapper/Mapper.h"
#include "../YamlParser/YamlObject.h"
#include "../YamlParser/YamlParser.h"
#include "../YamlParser/YamlConverter.h"
#include "../CsvParser/CsvParser.h"
#include "../Utilities/Utilities.h"


using namespace std;


/*
	FILEWRITING
*/

void Mapper::createGradesYamlFile(ofstream& outfile)
{
	YamlConverter converter;
	vector<YamlObject> allStudentIdAndGradeYamlObjs; 	
	map<string, string> studentIdsAndEmails = mapStudentIdsToEmailsAdresses();

	converter.studentIdsAndClasses = studentIdsAndClasses;	
	converter.generateClassHashes = false;
	converter.preferredIdentifier = "student_id";

	for(size_t i = 0; i < 11; i++)  
	{
		//MAKE A CONSTANT
		ifstream currentGradeCsvFile ("/home/robert/Documents/Projects/GrandeOmegaProject/GrandeOmegaRailsPrj/ext/Parser/docs/Grades/" + to_string(i+1) + ".csv");
		
		vector<pair<string, string>> numbersAndGradesOfFile = CsvParser::parseGradeFile(currentGradeCsvFile);

		vector<map<string, string>> studentIdsAndGradesForFile = mapStudentIdsToGradesAsKVs(numbersAndGradesOfFile, studentIdsAndEmails);

		for(int j = 0; j < studentIdsAndGradesForFile.size(); j++)
		{
			YamlObject objectForMap = converter.convertToBasicYamlObject(studentIdsAndGradesForFile[j]);
			allStudentIdAndGradeYamlObjs.push_back(objectForMap);				
		}		
	}
	converter.writeOutYamlFile(outfile, allStudentIdAndGradeYamlObjs);
	//fix student id deficiency in converter
} 

void Mapper::createYamlFilesWithClasses()
{
	studentIdsAndClasses = mapStudentIdsToClasses();

	//MAKE A CONSTANT
	string baseAssignmentFileString = "/home/robert/Documents/Projects/GrandeOmegaProject/GrandeOmegaRailsPrj/ext/Parser/docs/assignments/assignment_activities";
    vector<string> assignmentFileNames = Utilities::getListOfNumberedFilesForBaseFile(baseAssignmentFileString);
	ifstream initialStream = ifstream(assignmentFileNames[0]);
	
	YamlParser yamlParser (initialStream);

	//This setting of vars needs to be removed.
	YamlConverter converter;
	converter.studentIdsAndClasses = studentIdsAndClasses;
	converter.generateClassHashes = true;
	
	for(int i = 0; i < assignmentFileNames.size(); i++)  
	{
		//MAKE A CONSTANT
		ofstream outfile ("docs/classFiles/assignment_activities_with_class" + to_string(i+1) + ".yaml");  		
		yamlParser.file = ifstream(assignmentFileNames[i]);
		
		vector<YamlObject> yamlObjectsForFile = yamlParser.parseYaml();
		converter.writeOutYamlFile(outfile, yamlObjectsForFile);
	}	
}


/*
	MAPPING
*/

vector<map<string, string>> Mapper::mapStudentIdsToGradesAsKVs(vector<pair<string,string>>& numsAndGrades, map<string, string>& studentIdsAndEmails)
{
	vector<map<string, string>> studentIdsAndGrades; 
	map<string, string>::iterator studentIdsAndEmailsIterator;

	for (size_t i = 0; i < numsAndGrades.size(); i++)
	{
		normalizeGrade(numsAndGrades[i]);

		for (studentIdsAndEmailsIterator = studentIdsAndEmails.begin(); studentIdsAndEmailsIterator != studentIdsAndEmails.end(); 
			 studentIdsAndEmailsIterator++)		
		{
			string emailStudentNumber = studentIdsAndEmailsIterator->second.substr(0, studentIdsAndEmailsIterator->second.find("@"));

			if(emailStudentNumber == numsAndGrades[i].first)
			{
				map<string, string> studentIdKVPair;
				studentIdKVPair.insert(make_pair("student_id", studentIdsAndEmailsIterator->first));
				
				map<string, string> gradeKVPair;	
				gradeKVPair.insert(make_pair("grade", numsAndGrades[i].second));

				studentIdsAndGrades.push_back(studentIdKVPair);
				studentIdsAndGrades.push_back(gradeKVPair);;				
			}
		}		
	}
	return studentIdsAndGrades;
}



map<string, string> Mapper::mapStudentIdsToClasses()
{
	cout << "Mapping studentIds to classes..." << endl;
	
	map<string, string> studentIdsAndClasses;

	map<string, string>::iterator studentIdAndEmailElement;
	map<string, string>::iterator emailAndClassElement;
	
	emailsAndClasses = mapEmailsToClasses();
	cout << "Amount of emailsAndClasses = " << emailsAndClasses.size() << endl;


	for (studentIdAndEmailElement = studentIdsAndEmails.begin(); studentIdAndEmailElement != studentIdsAndEmails.end(); studentIdAndEmailElement++)
	{
		for (emailAndClassElement = emailsAndClasses.begin(); emailAndClassElement != emailsAndClasses.end(); emailAndClassElement++)		
		{
			cout << "studentIdAndEmailElement->second = " << studentIdAndEmailElement->second << " emailAndClassElement->first = " << emailAndClassElement->first << endl; 
			if(studentIdAndEmailElement->second == emailAndClassElement->first)
				studentIdsAndClasses.insert(make_pair(studentIdAndEmailElement->first, emailAndClassElement->second));		
		}
	}
	return studentIdsAndClasses;
}


map<string, string> Mapper::mapEmailsToClasses()
{
	cout << "Mapping emails to classes..." << endl;
    map<string, string> emailsAndClasses;

	studentIdsAndEmails = mapStudentIdsToEmailsAdresses();
    vector<string> allEmailAdresses = getListOfMapValues(studentIdsAndEmails);
	
    for(int i = 0; i < allEmailAdresses.size(); i++)
    {
		pair<string, string> emailAndClass = getClassAndEmailPair(allEmailAdresses[i]);
		emailsAndClasses.insert(emailAndClass);
    }

	int counter = 0;
	map<string, string>::iterator emailsIt; 
	for (emailsIt = emailsAndClasses.begin(); emailsIt != emailsAndClasses.end(); emailsIt++)
	{
		if(emailsIt->second == "tester")
			counter++;
	}	
	//is 24
	cout << "AMOUNT OF TESTER emails" << counter << endl;
    return emailsAndClasses;
}

map<string, string> Mapper::mapStudentIdsToEmailsAdresses()
{
	cout << "Mapping studentIds to emails..." << endl;

	map<string, string> studentIdsAndAdresses;
	for (int fileCounter = 0; fileCounter < anonymousYamlObjects.size(); fileCounter++)
	{
		for (int emailCounter= 0; emailCounter < emailYamlObjects.size(); emailCounter++)
		{
			string assignmentFileId = anonymousYamlObjects[fileCounter].values["student_id"];						
			string emailFileId = emailYamlObjects[emailCounter].values["- id"];	
			string email = emailYamlObjects[emailCounter].values["email"];


			if(assignmentFileId == emailFileId)
			{
				Utilities::removeStrWhiteSpace(assignmentFileId);
				Utilities::removeStrWhiteSpace(email);
				studentIdsAndAdresses.insert(make_pair(assignmentFileId, email));
			}
		}
	}
	return studentIdsAndAdresses;
}

/*
	UTILS
*/

pair<string, string> Mapper::getClassAndEmailPair(string& emailAdress)
{
	bool emailIsInAnyClass = false;

    for(int j= 0; j < emailAdressesPerClass.size(); j++)
    {
	    if(Utilities::isInVector(emailAdress, emailAdressesPerClass[j]))
        {
     	   string anonymizedClassNumber = to_string(hash<string>()(to_string(j)));
		   //making hash shorter
		   anonymizedClassNumber.erase(0,5);

	       emailIsInAnyClass = true;
		   return make_pair(emailAdress,anonymizedClassNumber); 
        }
	}

	if(emailIsInAnyClass == false)
    	return make_pair(emailAdress, "tester");
}

vector<string> Mapper::getListOfMapValues(map<string, string>& map)
{
    vector<string> valueList;
    for (const auto &p : map) {
        valueList.push_back( p.second );
    }   
    return valueList;
}

void Mapper::normalizeGrade(pair<string, string>& gradePair)
{
	gradePair.first = "0" + gradePair.first;
}