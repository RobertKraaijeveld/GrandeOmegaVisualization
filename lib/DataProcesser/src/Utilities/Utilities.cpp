#include "Utilities.h"
#include "../StatisticalAnalyzer/Point/IClusteringPoint.h"
#include "../StatisticalAnalyzer/Point/KMeansPoint.h"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cctype>
#include <locale>
#include <memory>
#include <pqxx/pqxx>

using namespace std;

vector<string> Utilities::toArrayByDelim(string &s, char delim) 
{
    vector<string> result;
    stringstream sstr(s);

    string token;
    while (getline(sstr, token, delim))
    {
        result.push_back(token);
    }
    return result;
}

vector<string> Utilities::getListOfNumberedFilesForBaseFile(string baseFileString)
{
    vector<string> returnVector;

    for (int i = 1; i < 66; i++)
    {
        returnVector.push_back(baseFileString + to_string(i) + ".yaml");
    }
    return returnVector;
}

void Utilities::removeYamlReferences(string &str)
{
    //TODO: Make constants out of these magic numbers. Also, decide whether to remove the first value whitespaces or not
    str.erase(0, str.find(" 2"));
}

string Utilities::createStrWithWhiteSpaces(int amount)
{
    size_t size = amount;
    return string(size, ' ');
}

void Utilities::removeStrWhiteSpace(string &str)
{
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}

bool Utilities::isNCharsWhiteSpace(int N, string &str)
{
    for (int i = 0; i < N - 1; i++)
    {
        if (!isspace(str[i]))
            return false;
    }
    return true;
}

bool Utilities::AreFloatsEqual(float f1, float f2)
{
    //ulp is 4, might be wrong
    return fabs(f1 - f2) < 1;
}

//DUPLICATION WITH STATISTICALTOOLS
int Utilities::computeAverage(vector<int> &values)
{
    int totalValues = 0;
    for (auto v : values)
    {
        totalValues += v;
    }

    if (values.size() > 0)
        return totalValues / values.size();
    else
        return totalValues / 1;
}

bool Utilities::isInVector(string &value, vector<string> &vector)
{
    return find(vector.begin(), vector.end(), value) != vector.end();
}

//Always bind the return of this to a new var first before using it!
vector<pqxx::result::tuple> Utilities::toListOfPqxxTuples(pqxx::result& r)
{
    vector<pqxx::result::tuple> resultVector;
    for(pqxx::result::tuple row : r)
    {
        resultVector.push_back(row);
    }
    return resultVector;
}

//Kinda sucks the dupeness between these two. 
//Need some kind of type covariance

vector<std::shared_ptr<IClusteringPoint>> Utilities::convertMapOfPairsToPoints(map<string, pair<int, int>> inputValues)
{    
    vector<std::shared_ptr<IClusteringPoint>> points;

    map<string, pair<int, int>>::iterator it;
    size_t counter = 0;

    for(it=inputValues.begin(); it!=inputValues.end(); ++it)
    {
        vector<float> valuesForGV;
        valuesForGV.push_back((float) it->second.first);
        valuesForGV.push_back((float) it->second.second);
        GenericVector newGv (valuesForGV);
        
        //dirty. How can we get to this derived obj using the std::shared_ptr<IClusteringPoint> vector???
        //yup. a visitor...
        std::shared_ptr<IClusteringPoint> pointPtr (new KMeansPoint(counter, newGv));
        points.push_back(pointPtr);

        counter++;
    }
    return points;
}


vector<std::shared_ptr<KMeansPoint>> Utilities::convertMapOfPairsToKMeansPoints(map<string, pair<int, int>> inputValues)
{
    vector<std::shared_ptr<KMeansPoint>> points;

    map<string, pair<int, int>>::iterator it;
    size_t counter = 0;

    for(it=inputValues.begin(); it!=inputValues.end(); ++it)
    {
        vector<float> valuesForGV;
        valuesForGV.push_back((float) it->second.first);
        valuesForGV.push_back((float) it->second.second);
        GenericVector newGv (valuesForGV);

        std::shared_ptr<KMeansPoint> kmeansPointPtr (new KMeansPoint(counter, newGv));
        points.push_back(kmeansPointPtr);

        counter++;
    }
    return points;
}

vector<std::shared_ptr<DBScanPoint>> Utilities::convertMapOfPairsToDBScanPoints(map<string, pair<int, int>> inputValues)
{
    vector<std::shared_ptr<DBScanPoint>> points;

    map<string, pair<int, int>>::iterator it;
    size_t counter = 0;

    for(it=inputValues.begin(); it!=inputValues.end(); ++it)
    {
        vector<float> valuesForGV;
        valuesForGV.push_back((float) it->second.first);
        valuesForGV.push_back((float) it->second.second);
        GenericVector newGv (valuesForGV);
        
        std::shared_ptr<DBScanPoint> dbScanPointPtr (new DBScanPoint(counter, newGv));
        points.push_back(dbScanPointPtr);

        counter++;
    }
    return points;
}
