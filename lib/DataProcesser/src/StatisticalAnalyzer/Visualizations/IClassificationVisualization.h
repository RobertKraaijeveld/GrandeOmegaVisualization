#ifndef ICLASSIFICATIONVISUALIZATION_H
#define ICLASSIFICATIONVISUALIZATION_H

#include "../Point/IClusteringPoint.h"

#include <string>
#include <vector>
#include <memory>

class IClassificationVisualization : public IVisualization
{
private:
  virtual std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getTrainingData() = 0;
  virtual std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> getClassifiedData() = 0;

public:
  //this logic is the same for each IClassificationVisualization so we define it only once, here
  virtual std::string getVisualizationAsJSON()
  {
    std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> classifiedData = getClassifiedData();

    return JSONEncoder::clustersToJSON(classifiedData);
  }
};

#endif