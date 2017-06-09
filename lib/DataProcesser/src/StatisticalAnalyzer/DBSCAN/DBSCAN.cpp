#include "../Point/DBScanPoint.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include "../../YamlParser/YamlObject.h"
#include "../../Utilities/Utilities.h"
#include "DBSCAN.h"

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <memory>


std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> DBSCAN::getClusteredData()
{
    clusterData();

    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> abstractClusterPointsPtrs;

    int nonEmptyClusterCount = 0;    
    for (std::vector<DBScanPoint> dbscanCluster : returnClusters)
    {
        //ensuring empty clusters arent added too
        if(dbscanCluster.size() > 1)
        {
            setPointClusterIdsToClusterIndexes(nonEmptyClusterCount, dbscanCluster);

            //bit of duplication with kmeans
            std::vector<std::shared_ptr<IClusteringPoint>> currClusterPtrs;
            for (size_t i = 0; i < dbscanCluster.size(); i++)
            {
                if(dbscanCluster[i].isNoise == false)
                {
                    std::shared_ptr<IClusteringPoint> currDBScanPointPtr (new DBScanPoint(dbscanCluster[i].getClusterId(), dbscanCluster[i].getVector()));
                    currClusterPtrs.push_back(currDBScanPointPtr);
                }
            }

            abstractClusterPointsPtrs.push_back(currClusterPtrs);
            nonEmptyClusterCount++;
        }
    }
    return abstractClusterPointsPtrs;
}



void DBSCAN::clusterData()
{
    int currentClusterIndex = 0;

    for (DBScanPoint currPoint : inputPoints)
    {
        if (currPoint.visited == true)
            continue;
        currPoint.visited = true;
        
        std::vector<DBScanPoint> neighboursOfCurrPoint = getPointsWithinMaxRadius(currPoint);

        if (neighboursOfCurrPoint.size() < minNeighboursAmount)
            currPoint.isNoise = true;
        else
        {
            returnClusters.push_back(vector<DBScanPoint>{});

            expandCluster(currPoint, neighboursOfCurrPoint, currentClusterIndex);
            currentClusterIndex++;            
        }
    }
}


void DBSCAN::expandCluster(DBScanPoint& point, std::vector<DBScanPoint>& neighbours, int currentClusterIndex)
{
    returnClusters[currentClusterIndex].push_back(point);

    for (DBScanPoint neighbour : neighbours)
    {
        if (point.visited == false)
        {
            point.visited = true;
            std::vector<DBScanPoint> newNeighbours = getPointsWithinMaxRadius(point);

            if (newNeighbours.size() >= minNeighboursAmount)
                neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());
        }

        if (isInAnyCluster(neighbour) == false)
            returnClusters[currentClusterIndex].push_back(neighbour);
    }
}

std::vector<DBScanPoint> DBSCAN::getPointsWithinMaxRadius(DBScanPoint &point)
{
    std::vector<DBScanPoint> pointsInRange;

    for (DBScanPoint currPoint : inputPoints)
    {
        if (currPoint.vector.getEuclidDistance(point.vector) < maxNeighbourRadius)
            pointsInRange.push_back(currPoint);
    }
    return pointsInRange;
}



bool DBSCAN::isInAnyCluster(DBScanPoint point)
{
    for (std::vector<DBScanPoint> &cluster : returnClusters)
    {
        if (std::find(cluster.begin(), cluster.end(), point) != cluster.end())
            return true;
    }
    return false;
}

void DBSCAN::setPointClusterIdsToClusterIndexes(int clusterIndex, std::vector<DBScanPoint> &pointsOfCluster)
{
    for (size_t i = 0; i < pointsOfCluster.size(); i++)
    {
        pointsOfCluster[i].setClusterId(clusterIndex);
    }
}