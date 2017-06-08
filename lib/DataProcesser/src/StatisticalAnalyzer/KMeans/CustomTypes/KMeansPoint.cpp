#include "KMeansPoint.h"
#include "../../GenericVector/GenericVector.h"
#include "../KMeansIteration.h"
#include <map>
#include <iostream>
#include <vector>

using namespace std;


Centroid::Centroid(int i, GenericVector gv)
{
    id = i;
    vector = gv;
} 


