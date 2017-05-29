#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <pqxx/pqxx>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "../../DatabaseInteracter/DatabaseInteracter.h"
#include "AssignmentIntervalFilter.h"

//Todo: Catch missing filterQueryColumnIndexes segfaults

bool AssignmentIntervalFilter::isValidAssignmentTime(std::string& previousTime, std::string& currTime)
{
    UtcTime previousUtcTime = UtcReader::toUtcTime(previousTime);
    UtcTime currUtcTime = UtcReader::toUtcTime(currTime);

    return ((currUtcTime.minute * 60) + currUtcTime.second)
           - ((previousUtcTime.minute * 60) + previousUtcTime.second) >= filterContext.timeBetweenAssignmentsThreshold;
}


std::vector<pqxx::result::tuple> AssignmentIntervalFilter::filter(std::vector<pqxx::result::tuple> unfilteredRows)
{  
    std::vector<pqxx::result::tuple> filteredRows;
    map<string, string> studentsAndLatestTimestamps;

    for(pqxx::result::tuple row: unfilteredRows)
    {
        string currStudentId = string(row[queryColumnIndexes.studentIdColumnIndex].c_str());
        string currTime = string(row[queryColumnIndexes.timestampIndex].c_str()); 

        if(studentsAndLatestTimestamps.count(currStudentId) == 1)
        {
            string& previousTime = studentsAndLatestTimestamps[currStudentId];

            if(isValidAssignmentTime(previousTime, currTime))
            {
              filteredRows.push_back(row);
            }
        }
        studentsAndLatestTimestamps[currStudentId] = currTime;        
    }
    return filteredRows;
}
