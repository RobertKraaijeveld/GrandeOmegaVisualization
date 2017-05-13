#ifndef KMEANSCONTROLLER_H
#define KMEANSCONTROLLER_H

#include "CustomTypes/KMeansPoint.h"
#include "../GenericVector/GenericVector.h"
#include "KMeansIteration.h"
#include "../../YamlParser/YamlObject.h"

class KMeansController {
    private:
        int dimension;
        int algorithmIterationAmount;
        int clusterAmount;

        //MAKE GENERIC
        map<string, pair<int, int>> inputValues;
        vector<KMeansPoint> points;
        vector<KMeansIteration> iterations;
        vector<vector<KMeansPoint>> finalClusters;

        void convertGradesAndExcersiseMapToPoints();
        vector<float> convertYamlObjectWantedValues(YamlObject& currentYamlObject);

        KMeansIteration getIterationWithBestSSE();
        vector<vector<KMeansPoint>> getClustersOfBestIteration();


    public:
        //IMPORTANT TODO: make input value more generic
        KMeansController(map<string, pair<int, int>> v, int k, int c, int d) 
        { 
            inputValues = v; 
            algorithmIterationAmount = k; 
            clusterAmount = c; 
            dimension = d;

            convertGradesAndExcersiseMapToPoints();
        };
        
        void run();
        vector<vector<KMeansPoint>> getFinalNonEmptyClusters();

};


#endif