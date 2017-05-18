#ifndef SimpleLinearRegressionTest_H
#define SimpleLinearRegressionTest_H

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

using namespace CppUnit;
using namespace std;

class SimpleLinearRegressionTest : public CppUnit::TestFixture
{
  public:
    SimpleLinearRegressionTest(){};
    static CppUnit::Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("SimpleLinearRegressionTest");

        suiteOfTests->addTest(new CppUnit::TestCaller<SimpleLinearRegressionTest>("testSlope", &SimpleLinearRegressionTest::testSlope));
        suiteOfTests->addTest(new CppUnit::TestCaller<SimpleLinearRegressionTest>("testIntercept", &SimpleLinearRegressionTest::testSlope));        
        return suiteOfTests;
    }
    void testIntercept();
    void testSlope();    
};

#endif