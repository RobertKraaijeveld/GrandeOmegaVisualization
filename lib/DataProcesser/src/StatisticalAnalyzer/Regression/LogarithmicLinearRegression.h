#ifndef LogarithmicLinearRegression_H
#define LogarithmicLinearRegression_H

#include <vector>

#include "IRegression.h"


class LogarithmicLinearRegression : public IRegression
{
    private:
        float sumValuesUsingLog(GenericVector vector);

    protected:
        float getSlope();
        float getIntercept();

        std::vector<std::pair<float, float>> getRegression();
             
    public:
        std::string getRegressionAsJSON();
  
        LogarithmicLinearRegression(std::vector<std::pair<float, float>> p) : IRegression(p){};
};

#endif