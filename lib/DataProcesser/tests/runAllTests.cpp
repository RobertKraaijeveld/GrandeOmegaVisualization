#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>
#include "UtilitiesTest.h"
#include "StatisticalToolsTest.h"

using namespace CppUnit;
using namespace std;
       
int main(int argc, char* argv[])
{
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(UtilitiesTest::suite());
    runner.addTest(StatisticalToolsTest::suite());
    
    runner.run();
    return 0;
}    