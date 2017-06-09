#include "Utilities.h"
#include "../StatisticalAnalyzer/GenericVector/GenericVector.h"
#include "../StatisticalAnalyzer/Point/IClusteringPoint.h"
#include "JSONEncoder.h"

#include <vector>
#include <string>


/*
This .cpp file is separate from the .inl file exists because
clustersToJSON does not use any template parameters; 

Putting its implementation in JSONEncoder.inl and then
including JSONEncoder.inl in  JSONEncoder.h would therefore produce linker errors.

SO, we define it in a normal cpp file instead. 
*/ 


std::string JSONEncoder::clustersToJSON(std::vector<std::vector<std::shared_ptr<IClusteringPoint>>> clusters) 
{
    stringstream returnJSONStr;

    //obj opening brace
    returnJSONStr << "[" << "\n";

    int clusterCounter = 0;

    //all clusters
    for(int i = 0; i < clusters.size(); i++)
    {
        size_t lastClusterIndex = clusters.size();

        if(clusters[i].size() > 0)
        {
            clusterCounter++;

            //opening brace for cluster
            returnJSONStr << "{ \"name\": \"" << clusterCounter << "\", \"data\": [" << endl;

            //vectors in a single cluster
            for(int j = 0; j < clusters[i].size(); j++)
            {
                //opening brace for vector pair            
                returnJSONStr << "{";
                
                //values in a single vector
                for(int z = 0; z < clusters[i][j]->getVector().values.size(); z++)
                {
                    size_t currVectorSize = clusters[i][j]->getVector().values.size();

                    //todo: generify for > 2 dimensional KMeans 
                    if(z == 0)
                        returnJSONStr << "\"x\": " << '"' << clusters[i][j]->getVector().values[z] << '"' << ',';
                    else
                        returnJSONStr << "\"y\": " << '"' << clusters[i][j]->getVector().values[z] << '"';
                }
                size_t lastClusterVectorIndex = clusters[i].size() - 1; 

                //closing brace for vector pair
                returnJSONStr << "}";

                //comma if theres clusters left to go                
                if(i != lastClusterIndex && j != lastClusterVectorIndex)
                    returnJSONStr << ", ";
            }
            //closing brace of entire cluster
            if(i != (lastClusterIndex- 1))
                returnJSONStr << "] }," << endl;
            else
                returnJSONStr << "] }" << endl;          
        }  
    }
    //final json obj closing brace 
    returnJSONStr << " ]";
    return returnJSONStr.str();
}