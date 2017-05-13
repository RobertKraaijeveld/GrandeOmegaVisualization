#include <vector>
#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include "StatisticalToolsTest.h"

#include "../src/StatisticalAnalyzer/StatisticalTools/StatisticalTools.h"
#include "../src/Utilities/Utilities.h"

using namespace CppUnit;
using namespace std;

void StatisticalToolsTest::testMean()
{
    vector<float> testVals {1,5,8};
    float testMeanResult = StatisticalTools::getMean(testVals);

    CPPUNIT_ASSERT(Utilities::AreFloatsEqual(testMeanResult, 4.66667f) == true);
}

void StatisticalToolsTest::testStandardDeviation()
{
    vector<float> testVals {1,5,8};
    float mean = 4.66667f;

    float testSDResult = StatisticalTools::getStandardDeviation(testVals, mean);

    CPPUNIT_ASSERT(Utilities::AreFloatsEqual(testSDResult, 2.867f) == true);
}