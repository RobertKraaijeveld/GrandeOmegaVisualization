#ifndef DATABASEINTERACTER_H
#define DATABASEINTERACTER_H

#define PATH_TO_CONFIG_FILE "/home/robert/Documents/Projects/GrandeOmegaVisualization/ext/Parser/docs/credentials.pgconf"
#define HOST "hostip"
#define PORT "port"
#define DATABASE "database"
#define USER "user"
#define PASSWORD "password"

#include <pqxx/pqxx>
#include <map>
#include <vector>
#include "../YamlParser/YamlObject.h"

using namespace std;

class DatabaseInteracter {
    private:
        string connectionString;
        void setDbConnectionString();
        
    public:
        void InsertAssignmentYaml(vector<YamlObject>& assignmentsObjects);
        void InsertGradesYaml(vector<YamlObject>& gradesObjects);     
        DatabaseInteracter(){ setDbConnectionString(); }   
};



#endif