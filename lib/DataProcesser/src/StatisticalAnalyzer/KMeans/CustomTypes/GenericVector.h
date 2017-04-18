#ifndef GENERICVECTOR_H
#define GENERICVECTOR_H

#include <string>
#include <vector>

using namespace std;


class GenericVector
{ 
   public:
    vector<float> values;   

    //standard data-structure for accompanied data
    GenericVector(){ values = vector<float>(32, 0.0);  };
    GenericVector(vector<float> v);

    float getEuclidDistance(GenericVector& gv);
    void sumWith(GenericVector& gv2);
    GenericVector divide(int divisor);

    static GenericVector getRandomVector(int dimensionLength);
    bool isNotEqual(GenericVector& gv);
    bool VectorSizesEqual(GenericVector& gv);
    string ToString();
};

#endif