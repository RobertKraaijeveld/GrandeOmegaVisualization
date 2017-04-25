#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include "DatabaseInteracter.h"
#include "../Utilities/Utilities.h"

using namespace std;

void DatabaseInteracter::setDbConnectionString()
{
    string ConnectionString;
    string line;
    ifstream config_file (PATH_TO_CONFIG_FILE);
    
    if(config_file.good())
    {
        string key;
        string value;

        while(getline(config_file, line)) 
        {
            key = line.substr(0, line.find(':'));
            value = line.substr(line.find(':') + 1, line.size());
            Utilities::removeStrWhiteSpace(value);
            
            if(key == HOST) 
                ConnectionString += (" hostaddr=" + value);
            else if(key == PORT) 
                ConnectionString += (" port=" + value);            
            else if(key == DATABASE) 
                ConnectionString += ("dbname=" + value);            
            else if(key == USER)
                ConnectionString += (" user=" + value);            
            else if(key == PASSWORD)
                ConnectionString += (" password=" + value);            
        }
    }
    else
    {
        cout << "DatabaseInteracter: config file not found" << endl; 
    }
    config_file.close();
    this->connectionString = ConnectionString;
}


pqxx::result DatabaseInteracter::executeSelectQuery(string query)
{
    //unsafe; might not return anything
    try
    {
        pqxx::connection conn(this->connectionString);
        pqxx::work worker(conn);

        pqxx::result queryResult = worker.exec(query);

        worker.commit();

        return queryResult;        
    }
    catch (const exception &e)
    {
        cout << e.what() << endl;
    }
}

void DatabaseInteracter::InsertAssignmentYaml(vector<YamlObject>& assignmentsObjects)
{
    try
    {
        pqxx::connection conn(this->connectionString);
        pqxx::work worker(conn);

        for(int i = 0; i < assignmentsObjects.size(); i++)
        {
            string creation_timestamp = assignmentsObjects[i].children["created_at"].values["utc"];
            string update_timestamp = assignmentsObjects[i].children["updated_at"].values["utc"];            

            Utilities::removeYamlReferences(creation_timestamp);
            Utilities::removeYamlReferences(update_timestamp);

            std::ostringstream insertionStream;

            insertionStream << "INSERT INTO assignments(id, creation_timestamp, sort, student_id, class, teaching_unit_id) VALUES(\'"
            << assignmentsObjects[i].values["- id"] << "\', \'" << creation_timestamp << "\', \'" << assignmentsObjects[i].values["sort"] 
            << "\', \'" << assignmentsObjects[i].values["student_id"] << "\', \'" << assignmentsObjects[i].values["class"] 
            << "\', \'" << assignmentsObjects[i].values["teaching_unit_id"] << "\');";

            string insertionString = insertionStream.str();
            worker.exec(insertionString);
        }
        worker.commit();        
    }
    catch (const exception &e)
    {
        cout << e.what() << endl;
    }
}

void DatabaseInteracter::InsertGradesYaml(vector<YamlObject>& gradesObjects)
{
    try
    {
        pqxx::connection conn(this->connectionString);
        pqxx::work worker(conn);

        for(int i = 0; i < gradesObjects.size(); i++)
        {
            std::ostringstream insertionStream;

            insertionStream << "INSERT INTO grades(student_id, grade) VALUES(\'"
            << gradesObjects[i].values["- student_id"] << "\', \'" << gradesObjects[i].values["grade"] << "\');";

            string insertionString = insertionStream.str();

            worker.exec(insertionString);
        }
        worker.commit();
    }
    catch (const exception &e)
    {
        cout << e.what() << endl;
    }
}