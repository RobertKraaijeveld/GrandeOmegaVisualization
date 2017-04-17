#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include "UtilitiesTest.h"

#include "../src/Utilities/Utilities.h"

using namespace CppUnit;
using namespace std;

void UtilitiesTest::testIsNCharsWhiteSpace()
{
    string testString = "1";

    bool testResultOne = Utilities::isNCharsWhiteSpace(0, testString);
    bool testResultTwo = Utilities::isNCharsWhiteSpace(2, testString);

    CPPUNIT_ASSERT(testResultOne == true);
}