#include "../Point/DBScanPoint.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include "../../YamlParser/YamlObject.h"
#include "../../Utilities/Utilities.h"
#include "DBSCAN.h"
#include <vector>
#include <map>
#include <algorithm>


void DBSCAN::clusterData()
{
    int currentClusterIndex = 0;

    for(DBScanPoint* currPoint : inputPoints)
    {
        if((*currPoint).visited)
            continue;

        (*currPoint).visited = true;
        std::vector<DBScanPoint*> neighboursOfCurrPoint = getPointsWithinMaxRadius((*currPoint));

        if(neighboursOfCurrPoint.size() < minNeighboursAmount)
            (*currPoint).isNoise = true;
        else
        {
            currentClusterIndex++;
            expandCluster((*currPoint), neighboursOfCurrPoint, currentClusterIndex);
        }
    }
}

void DBSCAN::expandCluster(DBScanPoint& point, std::vector<DBScanPoint*>& neighbours, int currentClusterIndex)
{
    returnClusters[currentClusterIndex].push_back(point);

    for(DBScanPoint* neighbour : neighbours)
    {
        if(point.visited == false)
        {
            point.visited = true;
            std::vector<DBScanPoint*> newNeighbours = getPointsWithinMaxRadius(point);

            if(newNeighbours.size() >= minNeighboursAmount)
                neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());
        }
        
        if(isInAnyCluster((*neighbour)))
            returnClusters[currentClusterIndex].push_back((*neighbour));
    }
}

std::vector<DBScanPoint*>& DBSCAN::getPointsWithinMaxRadius(DBScanPoint& point)
{
    std::vector<DBScanPoint*> pointsInRange;

    for(DBScanPoint* currPoint : inputPoints)
    {
        if((*currPoint).vector.getEuclidDistance(point.vector))
            pointsInRange.push_back(currPoint);   
    }
    return pointsInRange;
}


bool DBSCAN::isInAnyCluster(DBScanPoint point)
{
    for(std::vector<DBScanPoint>& cluster : returnClusters)
    {
        if(std::find(cluster.begin(), cluster.end(), point) != cluster.end())
            return true;
    }
    return false;
}
