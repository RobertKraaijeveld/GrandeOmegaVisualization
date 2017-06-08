#ifndef KMEANSCONTROLLER_H
#define KMEANSCONTROLLER_H

#include "CustomTypes/KMeansPoint.h"
#include "../Point/IClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include "KMeansIteration.h"
#include "../../YamlParser/YamlObject.h"
#include "../../Utilities/Utilities.h"


class KMeansController {
    private:
        int dimension;
        int algorithmIterationAmount;
        int clusterAmount;

        vector<KMeansPoint> points;
        vector<KMeansIteration> iterations;
        vector<vector<KMeansPoint>> finalClusters;

        void convertGradesAndExcersiseMapToPoints();
        vector<float> convertYamlObjectWantedValues(YamlObject& currentYamlObject);

        KMeansIteration getIterationWithBestSSE();
        vector<vector<KMeansPoint>> getClustersOfBestIteration();

        void setPointCentroidIdsToClusterIndexes(int clusterIndex, vector<KMeansPoint>& pointsOfCluster);


    public:
        KMeansController(vector<KMeansPoint*> p, int k, int c, int d) 
        { 
            //dereferencing KMeansPoints* 
            for(auto point : p)
                 points.push_back(*point);

            algorithmIterationAmount = k; 
            clusterAmount = c; 
            dimension = d;
        };
        
        void run();
        vector<vector<KMeansPoint*>> getFinalNonEmptyClusters();

};


#endif