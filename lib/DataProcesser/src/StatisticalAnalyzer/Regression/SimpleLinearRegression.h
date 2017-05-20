#ifndef SimpleLinearRegression_H
#define SimpleLinearRegression_H

#include <vector>

#include "IRegression.h"


class SimpleLinearRegression : public IRegression 
{
    private:
        std::pair<GenericVector, GenericVector> xAndYVectors;
        std::pair<GenericVector, GenericVector> convertPairsToGVs(std::vector<std::pair<float, float>> pairs);
        
    protected:
        float getSlope();
        float getIntercept();
        std::vector<std::pair<float, float>> getRegression();
             
    public:
        std::string getRegressionAsJSON();
  
        SimpleLinearRegression(std::vector<std::pair<float, float>> p) { xAndYVectors = convertPairsToGVs(p); }  
};

#endif