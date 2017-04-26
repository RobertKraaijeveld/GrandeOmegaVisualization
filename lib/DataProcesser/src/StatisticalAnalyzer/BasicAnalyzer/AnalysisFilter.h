#ifndef ANALYSISFILTER_H
#define ANALYSISFILTER_H

#include <string>

class AnalysisFilter {
    private:
        int calculateQueryLimit(int totalAmount); 

    public:
        double timeBetweenAssignmentsThreshold = 0.0;
        double upperPercentageOfGradesToBeSelected = 100.0; 

        bool isValidAssignmentTime(std::string previousTime, std::string currTime);
        std::string getGradeSortingQuery(int totalGradesAmount);
};

#endif