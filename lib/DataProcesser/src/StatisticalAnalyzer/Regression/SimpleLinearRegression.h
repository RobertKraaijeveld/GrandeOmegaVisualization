#ifndef SimpleLinearRegression_H
#define SimpleLinearRegression_H

#include <vector>

#include "IRegression.h"


class SimpleLinearRegression : public IRegression 
{
    private:
        pair<GenericVector, GenericVector> xAndYVectors;
        pair<GenericVector, GenericVector> convertPairsToGVs(map<string, pair<float, float>> pairsMap);

        pair<float, float> getXYMeans();
        
    protected:
        float getSlope();
        float getIntercept();
         
    public:
        vector<std::pair<float, float>> getRegression(vector<float> xValues);  
        SimpleLinearRegression(map<string, pair<float, float>> p) { xAndYVectors = convertPairsToGVs(p); }  
};

#endif