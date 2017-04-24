#include "Point.h"
#include "GenericVector.h"
#include "../KMeansIteration.h"
#include <map>
#include <iostream>
#include <vector>

using namespace std;

Point::Point(int cId, GenericVector gv)
{
    centroidId = cId;
    vector = gv;
}

Centroid::Centroid(int i, GenericVector gv)
{
    id = i;
    vector = gv;
} 


