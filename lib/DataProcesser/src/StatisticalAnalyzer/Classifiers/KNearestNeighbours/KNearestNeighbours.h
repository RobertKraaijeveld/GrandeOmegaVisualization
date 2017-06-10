#ifndef KNEARESTNEIGHBOURS_H
#define KNEARESTNEIGHBOURS_H

#include "../IClassifier.h"
#include "../../../Utilities/Utilities.h"
#include "../../Point/IClusteringPoint.h"
#include "../../GenericVector/GenericVector.h"

#include <vector>
#include <map>
#include <memory>

class KNearestNeighbours : public IClassifier
{
private:
  int maxAmountOfNeighbours;
  std::vector<std::shared_ptr<IClusteringPoint>> inputPoints;
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> trainingClusters;

  
  int getNewClusterIdByVote(std::shared_ptr<IClusteringPoint>point, std::vector<std::shared_ptr<IClusteringPoint>> nearestNeighbours);

public:
  //also used in naive bayes, thusly static
  static std::vector<std::shared_ptr<IClusteringPoint>> getNearestNeighbours(std::shared_ptr<IClusteringPoint>& point, int maxAmountOfNeighbours,
                                                                             std::vector<std::vector<std::shared_ptr<IClusteringPoint>>>& clusters);
  
  std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedPoints();

  //necessary to overload initialize()
  //initialize method instead of constructor in order to be able to pass pointers to this around without having to declare input and training at the same time
  void initialize(std::vector<std::shared_ptr<IClusteringPoint>> input, vector<vector<std::shared_ptr<IClusteringPoint>>> tc)
  {
    inputPoints = input;
    trainingClusters = tc;

    //cheeky hard-code for nicer polymorphism
    std::cout << "KNN maxAmountOfNeighbours = 7" << endl; 
    maxAmountOfNeighbours = 7;
  }
};

#endif