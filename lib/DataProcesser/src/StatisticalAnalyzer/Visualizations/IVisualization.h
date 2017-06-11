#ifndef IVISUALIZATION_H
#define IVISUALIZATION_H

#include <string>

/**
    Each IVisualization has to be encodable to JSON.
*/

class IVisualization {
    public:
        virtual std::string getVisualizationAsJSON() = 0;
};

#endif