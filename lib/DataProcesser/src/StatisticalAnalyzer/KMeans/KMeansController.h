#ifndef KMEANSCONTROLLER_H
#define KMEANSCONTROLLER_H

#include "CustomTypes/ClusteringPoint.h"
#include "../GenericVector/GenericVector.h"
#include "KMeansIteration.h"
#include "../../YamlParser/YamlObject.h"
#include "../../Utilities/Utilities.h"


class KMeansController {
    private:
        int dimension;
        int algorithmIterationAmount;
        int clusterAmount;

        map<string, pair<int, int>> inputValues;
        vector<ClusteringPoint> points;
        vector<KMeansIteration> iterations;
        vector<vector<ClusteringPoint>> finalClusters;

        void convertGradesAndExcersiseMapToPoints();
        vector<float> convertYamlObjectWantedValues(YamlObject& currentYamlObject);

        KMeansIteration getIterationWithBestSSE();
        vector<vector<ClusteringPoint>> getClustersOfBestIteration();


    public:
        KMeansController(map<string, pair<int, int>> v, int k, int c, int d) 
        { 
            inputValues = v; 
            algorithmIterationAmount = k; 
            clusterAmount = c; 
            dimension = d;

            points = Utilities::convertMapOfPairsToPoints(inputValues);
        };
        
        void run();
        vector<vector<ClusteringPoint>> getFinalNonEmptyClusters();

};


#endif