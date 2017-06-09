#ifndef DBSCAN_H
#define DBSCAN_H

#include "../Point/DBScanPoint.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include "../../YamlParser/YamlObject.h"
#include "../../Utilities/Utilities.h"

#include <memory>

class DBSCAN
{
private:
  std::vector<DBScanPoint> inputPoints;
  std::vector<std::vector<DBScanPoint>> returnClusters;
  int minNeighboursAmount;
  float maxNeighbourRadius;

  void clusterData();

  void expandCluster(DBScanPoint &point, std::vector<DBScanPoint> &neighbours, int currentClusterIndex);
  std::vector<DBScanPoint> getPointsWithinMaxRadius(DBScanPoint &point);

  bool isInAnyCluster(DBScanPoint point);
  void setPointClusterIdsToClusterIndexes(int clusterIndex, std::vector<DBScanPoint> &pointsOfCluster);


public:
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClusteredData();

  //conversion sucks
  DBSCAN(std::vector<std::shared_ptr<DBScanPoint>> p, int minpts, float maxradius)
  {
    //dereferencing KMeansPoints*
    for (auto point : p)
      inputPoints.push_back(*point);

    minNeighboursAmount = minpts;
    maxNeighbourRadius = maxradius;
  }
};
#endif