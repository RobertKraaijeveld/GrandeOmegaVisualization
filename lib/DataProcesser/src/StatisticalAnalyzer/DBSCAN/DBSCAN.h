#ifndef KMEANSCONTROLLER_H
#define KMEANSCONTROLLER_H

#include "../Point/DBScanPoint.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include "../../YamlParser/YamlObject.h"
#include "../../Utilities/Utilities.h"

class DBSCAN
{
  private:
    std::vector<DBScanPoint*> inputPoints;
    std::vector<std::vector<DBScanPoint>> returnClusters;
    int minNeighboursAmount;
    float maxNeighbourRadius;


    void clusterData();

    void expandCluster(DBScanPoint& point, std::vector<DBScanPoint*>& neighbours, int currentClusterIndex);
    std::vector<DBScanPoint*>& getPointsWithinMaxRadius(DBScanPoint& point);

    bool isInAnyCluster(DBScanPoint point);

  public:
    std::vector<std::vector<IClusteringPoint*>> getClusteredData();

    //conversion sucks
    DBSCAN(std::vector<DBScanPoint*> input, int minpts, float maxradius)
    {
        inputPoints = input;
        minNeighboursAmount = minpts;
        maxNeighbourRadius = maxradius;
    }
};
#endif