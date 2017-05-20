#ifndef IVISUALIZATION_H
#define IVISUALIZATION_H

#include "AnalysisFilter.h"

class IVisualization {
    protected:
        AnalysisFilter filterer;
    public:
        virtual std::string getVisualizationAsJSON(){ return "{ \"data\": \"No derived visualization JSON found.\" }"; };
        void setFilterer(AnalysisFilter f) { filterer = f; };
};

#endif