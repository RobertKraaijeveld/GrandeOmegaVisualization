#include "GenericVector.h"
#include <math.h>
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

GenericVector::GenericVector(vector<float> v)
{
    values = v;
}

float GenericVector::getEuclidDistance(GenericVector& other)
{
    if (!this->VectorSizesEqual(other))
    {
        //Todo: Make this an exception instead.
        return -1.0;        
    }

    float sumOfSquaredSubtractions = 0;
    for (int i = 0; i < values.size(); i++)
    {
        sumOfSquaredSubtractions += pow(values[i] - other.values[i], 2.0);
    }
    return sqrt(sumOfSquaredSubtractions);
}

void GenericVector::sumWith(GenericVector& gv2)
{
    for(int i = 0; i < values.size(); i++)
    {
        values[i] += gv2.values[i];
    }
}


GenericVector GenericVector::divide(int divisor)
{
    for (int i = 0; i < values.size(); i++)
    {
        //avoiding division by 0 errors on not-chosen clusters
        if(divisor == 0)
            divisor = 1;

        values[i] = values[i] / divisor;
    }
    return * this;
 }


GenericVector GenericVector::getRandomVector(int dimensionLength)
{
    random_device rd;
 
    //Mersenne Twister pseudo-random number generator
    mt19937 gen(rd());
 
    //distributor in range Todo: Generify this
    uniform_int_distribution<> distributor(0, 1);
 
    vector<float> values;
    for (int i = 0; i < dimensionLength; i++)
    {
        values.push_back(distributor(gen));
    }
    return GenericVector(values);
}

bool GenericVector::isNotEqual(GenericVector& gv)
{
    for(int i = 0; i < values.size(); i++)
    {
        if(values[i] != gv.values[i])
            return true;            
    }
    return false;
}

bool GenericVector::VectorSizesEqual(GenericVector &other)
{
    return other.values.size() == values.size();
}

string GenericVector::ToString()
{
    string returnStr = "(";

    for (int i = 0; i != values.size(); i++)
    {
        stringstream stream;
        stream << (values[i]);
        string strResult = stream.str();

        if (i == (values.size() - 1))
        {
            returnStr = returnStr + strResult + ")";
        }
        else
        {
            returnStr = returnStr + strResult + ",";
        }
    }
    return returnStr;
}

