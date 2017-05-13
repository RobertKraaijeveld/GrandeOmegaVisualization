#ifndef IREGRESSION_H
#define IREGRESSION_H

#include <vector>
#include "../GenericVector/GenericVector.h"

class IRegression {
    protected:
        virtual float getSlope();
        virtual float getIntercept(); 
    public:
        virtual vector<std::pair<float, float>> getRegression();
};

#endif