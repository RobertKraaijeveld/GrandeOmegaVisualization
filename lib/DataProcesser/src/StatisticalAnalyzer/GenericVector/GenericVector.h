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
    GenericVector(){ values = vector<float>(2, 0.0);  };
    GenericVector(pair<float, float> p){ values = {p.first, p.second}; };    
    GenericVector(vector<float> v) { values = v; };

    float dotProduct(GenericVector& other);
    float getEuclidDistance(GenericVector& other);
    void sumWith(GenericVector& gv2);
    GenericVector divide(int divisor);

    static GenericVector getRandomVector(int dimensionLength);
    bool isNotEqual(GenericVector& other);
    bool VectorSizesEqual(GenericVector& other);
    string ToString();
};

#endif