#include "KMeansController.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm> 

using namespace std;



//	KMeansController kmeansController(parsedPoints,amountOfIterationsAndClusters.first, amountOfIterationsAndClusters.second);
//	kmeansController.run();

/*

vector<Point> CsvParser::parseToPoints(ifstream& fileStr)
{
    vector<Point> finalResult;
    string line;
    int rowCounter;

    while( getline(fileStr, line) )
    {
        rowCounter++;
        vector<float> valuesForVector;
        
        stringstream linestr (line);
        string cell;

        while(getline(linestr, cell, ','))
        {
            float vectorFactor = (float) atoi(cell.c_str());;
            valuesForVector.push_back(vectorFactor);
        }
        GenericVector newVector (valuesForVector);
        Point newPoint (rowCounter, newVector);

        finalResult.push_back(newPoint);
    }
    return finalResult;  
}


int main()    
{
	ifstream ifs;
	ifs.open("/home/robert/Documents/Projects/DataMiningExcersises/Excersise 1 - KMeans Clustering/docs/wine.csv");

	if(ifs.good())
	{
		executeKMeansRoutine(ifs);
	}
	else
	{
		cout << "CSV File not found. Please check if the path in main.cpp is correct." << endl;				
		cin.get();	
	}
	return 0;
}


class Point {
    public:
        Point(int cId, GenericVector vector);
        int centroidId;
        GenericVector vector;
        double distanceToCentroid;
};


class GenericVector
{ 
   public:
    vector<float> values;   

    //standard data-structure for accompanied data
    GenericVector(){ values = vector<float>(32, 0.0);  };
    GenericVector(vector<float> v);


*/

//MUST BE ABLE TO BE CONVERTED TO FLOATS. RAISE EXCEPTION IF NOT?
void KMeansController::convertGradesAndExcersiseMapToPoints()
{    
    //maybe also add something so we can see studentnumbers without using them in the clustering
    map<string, pair<int, int>>::iterator it;
    size_t counter = 0;

    for(it=inputValues.begin(); it!=inputValues.end(); ++it)
    {
        vector<float> valuesForGV;
        valuesForGV.push_back((float) it->second.first);
        valuesForGV.push_back((float) it->second.second);


        GenericVector newGv (valuesForGV);
        cout << "Adding new gv " << newGv.ToString() << endl;
        
        Point newPoint (counter, newGv);
        points.push_back(newPoint);

        counter++;
    }
    cout << "Points.size = " << points.size() << endl;
}

KMeansIteration KMeansController::getIterationWithBestSSE()
{
    pair<int&, float> bestIterationIdAndSSE (iterations[0].iterationId, iterations[0].sumOfSquaredErrors);

    //maybe tell iteration to calc sse here?
    for (int i = 0; i < iterations.size(); i++)
    {
        if(iterations[i].sumOfSquaredErrors <  bestIterationIdAndSSE.second)
        {
            bestIterationIdAndSSE.first = iterations[i].iterationId;
            bestIterationIdAndSSE.second =  iterations[i].sumOfSquaredErrors;
        }
    }

    for(int j =0; j < iterations.size(); j++)
    {
        if(iterations[j].iterationId == bestIterationIdAndSSE.first)
            return iterations[j];            
    }
}


void KMeansController::run()
{
    int iterationsCount = 0;

    while(iterationsCount < algorithmIterationAmount)
    {
        KMeansIteration kmeansIteration(points, clusterAmount, iterationsCount, dimension);
        kmeansIteration.runIteration();

        iterations.push_back(kmeansIteration);
        iterationsCount++;
    }
    finalClusters = getClustersOfBestIteration();
}

vector<vector<Point>> KMeansController::getClustersOfBestIteration()
{
    vector<vector<Point>> returnClusters;

    //TEST TEMP
    for(int i = 0; i <= clusterAmount; i++)
    {
        vector<Point> fillerVector;
        returnClusters.push_back(fillerVector);
    }

    KMeansIteration bestIteration = getIterationWithBestSSE();
    
    for(auto p : bestIteration.points)
    {
        int clusterIndex = p.centroidId;

        returnClusters[clusterIndex].push_back(p);
    }
    return returnClusters;
}


