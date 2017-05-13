#ifndef SimpleLinearRegression_H
#define SimpleLinearRegression_H

#include <vector>

#include "IRegression.h"


class SimpleLinearRegression : public IRegression 
{
    private:
        pair<GenericVector, GenericVector> xAndY;
        pair<GenericVector, GenericVector> convertPairsToGVs(map<string, pair<float, float>> pairsMap);
    
    protected:
        float getSlope();
        float getIntercept();
         
    public:
        vector<std::pair<float, float>> getRegression();  
        SimpleLinearRegression(map<string, pair<float, float>> p) { xAndY = convertPairsToGVs(p); }  
};

#endif