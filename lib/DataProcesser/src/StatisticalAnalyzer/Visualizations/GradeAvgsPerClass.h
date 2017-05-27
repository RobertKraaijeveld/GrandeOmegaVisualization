#ifndef GRADEAVGSPERCLASS_H
#define GRADEAVGSPERCLASS_H

#include "IVisualization.h"
#include <vector>
#include <pqxx/pqxx>

class GradeAvgsPerClass : public IVisualization 
{
    private:
        std::vector<std::pair<std::string, int>> getGradeAvgsPairs();
        pqxx::result getClassAveragesRows();

    public:
        std::string getVisualizationAsJSON();
};

#endif