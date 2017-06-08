#include "KMeansPoint.h"
#include "../../GenericVector/GenericVector.h"
#include "../KMeansIteration.h"
#include <map>
#include <iostream>
#include <vector>

using namespace std;

KMeansPoint::KMeansPoint(int cId, GenericVector gv)
{
    centroidId = cId;
    vector = gv;
}

Centroid::Centroid(int i, GenericVector gv)
{
    id = i;
    vector = gv;
} 


