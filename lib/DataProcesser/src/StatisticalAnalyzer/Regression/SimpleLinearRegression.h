#ifndef SimpleLinearRegression_H
#define SimpleLinearRegression_H

#include <vector>

#include "IRegression.h"


class SimpleLinearRegression : public IRegression 
{
    protected:
        float getSlope();
        float getIntercept();

        std::vector<std::pair<float, float>> getRegression();
             
    public:
        std::string getRegressionAsJSON();
  
        SimpleLinearRegression(std::vector<std::pair<float, float>> p) : IRegression(p){};
};

#endif