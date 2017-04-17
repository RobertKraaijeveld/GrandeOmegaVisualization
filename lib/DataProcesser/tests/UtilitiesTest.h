#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

using namespace CppUnit;
using namespace std;

class UtilitiesTest : public CppUnit::TestFixture
{
  public: 
    UtilitiesTest(){};
    static CppUnit::Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("UtilitiesTest");

        suiteOfTests->addTest(new CppUnit::TestCaller<UtilitiesTest>("testIsNCharsWhiteSpace", &UtilitiesTest::testIsNCharsWhiteSpace));
        return suiteOfTests;
    }
   //void setUp();
   //void tearDown();

   void testIsNCharsWhiteSpace();
};