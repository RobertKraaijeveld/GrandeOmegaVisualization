#ifndef IVISUALIZATION_H
#define IVISUALIZATION_H

#include <string>

class IVisualization {
    public:
        virtual std::string getVisualizationAsJSON() = 0;
};

#endif