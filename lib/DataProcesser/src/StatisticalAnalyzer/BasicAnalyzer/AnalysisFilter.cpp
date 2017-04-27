#include <string>
#include <iostream>
#include <cmath>

#include "../../Utilities/UtcTime.h"
#include "../../Utilities/UtcReader.h"
#include "AnalysisFilter.h"


int AnalysisFilter::calculateQueryLimit(int totalAmount) 
{
    double percentAsDecimal = upperPercentageOfGradesToBeSelected * 0.01;
    cout << "formula for percent to limit " <<  "upperPercentageOfGradesToBeSelected " << upperPercentageOfGradesToBeSelected << "*" << " 0.01 = " << percentAsDecimal << endl; 
    cout << totalAmount << " * " << percentAsDecimal << " = " << (totalAmount * percentAsDecimal) << endl;  
    return ceil(totalAmount * percentAsDecimal);
}


bool AnalysisFilter::isValidAssignmentTime(std::string previousTime, std::string currTime)
{
    UtcTime previousUtcTime = UtcReader::toUtcTime(previousTime);
    UtcTime currUtcTime = UtcReader::toUtcTime(currTime);

    return ((currUtcTime.minute * 60) + currUtcTime.second)
           - ((previousUtcTime.minute * 60) + previousUtcTime.second) >= timeBetweenAssignmentsThreshold;
}

//doesnt take NDs into account
std::string AnalysisFilter::getGradeSortingQuery(int totalGradesAmount)
{
    //use bools to indicate what should and should not be chosen?
    cout << "Query is " << ("ORDER BY CAST(grades.grade AS int) DESC LIMIT " + to_string(calculateQueryLimit(totalGradesAmount))) << endl; 
    return "ORDER BY CAST(grades.grade AS int) DESC LIMIT " + to_string(calculateQueryLimit(totalGradesAmount));
}
