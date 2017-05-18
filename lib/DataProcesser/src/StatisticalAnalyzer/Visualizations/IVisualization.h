#ifndef IVISUALIZATION_H
#define IVISUALIZATION_H

#include "../BasicAnalyzer/AnalysisFilter.h"

class IVisualization {
    protected:
        AnalysisFilter filterer;
    public:
        virtual std::string getVisualizationAsJSON(){};
        void setFilterer(AnalysisFilter f) { filterer = f; };
};

#endif