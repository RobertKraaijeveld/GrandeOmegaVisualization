#ifndef IREGRESSION_H
#define IREGRESSION_H

#include <vector>
#include "../GenericVector/GenericVector.h"

class IRegression {
    protected:
        virtual float getSlope() = 0;
        virtual float getIntercept() = 0; 
        virtual std::vector<std::pair<float, float>> getRegression() = 0;
    public:
        virtual std::string getRegressionAsJSON() = 0;
};

#endif   