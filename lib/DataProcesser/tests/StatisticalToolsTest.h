#ifndef STATISTICALTOOLSTEST_H
#define STATISTICALTOOLSTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

using namespace CppUnit;
using namespace std;

class StatisticalToolsTest : public CppUnit::TestFixture
{
  public:
    StatisticalToolsTest(){};
    static CppUnit::Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("StatisticalToolsTest");

        suiteOfTests->addTest(new CppUnit::TestCaller<StatisticalToolsTest>("testMean", &StatisticalToolsTest::testMean));
        suiteOfTests->addTest(new CppUnit::TestCaller<StatisticalToolsTest>("testStandardDeviation", &StatisticalToolsTest::testStandardDeviation));        
        return suiteOfTests;
    }
    void testMean();
    void testStandardDeviation();    
};

#endif