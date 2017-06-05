#ifndef AMOUNTOFSTUDENTSPERCLASS_H
#define AMOUNTOFSTUDENTSPERCLASS_H

#include "IVisualization.h"
#include <vector>
#include <pqxx/pqxx>

class AmountOfStudentsPerClass : public IVisualization 
{
    private:
        pqxx::result getStudentAmountPerClassQueryResult();
    public:
        std::string getVisualizationAsJSON();
};

#endif