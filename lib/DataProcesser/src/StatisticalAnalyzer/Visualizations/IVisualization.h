#ifndef IVISUALIZATION_H
#define IVISUALIZATION_H

#include <string>

//Gotta take REAL good care about shared ptrs
class IVisualization {
    public:
        virtual std::string getVisualizationAsJSON() = 0;
};

#endif