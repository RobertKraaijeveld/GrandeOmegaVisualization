#ifndef IREGRESSION_H
#define IREGRESSION_H

#include <vector>
#include "../GenericVector/GenericVector.h"


/**
  Each IRegression has to define its' own ways of calculating slope, intercept and final regression.
  'getRegressionAsJSON' is specific per IRegression derived class as well.
 */ 


class IRegression {
    protected:
        std::pair<GenericVector, GenericVector> xAndYVectors;
        std::pair<GenericVector, GenericVector> convertPairsToGVs(std::vector<std::pair<float, float>> pairs);
        
        virtual float getSlope() = 0;
        virtual float getIntercept() = 0; 
        virtual std::vector<std::pair<float, float>> getRegression() = 0;
    public:
        virtual std::string getRegressionAsJSON() = 0;

        IRegression(std::vector<std::pair<float, float>> p){ xAndYVectors = convertPairsToGVs(p); };
};

#endif   