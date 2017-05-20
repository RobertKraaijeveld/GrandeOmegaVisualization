#ifndef IREGRESSION_H
#define IREGRESSION_H

#include <vector>
#include "../GenericVector/GenericVector.h"

class IRegression {
    protected:
        virtual float getSlope(){};
        virtual float getIntercept(){}; 
        virtual std::vector<std::pair<float, float>> getRegression(){};
    public:
        virtual std::string getRegressionAsJSON(){};
};

#endif   