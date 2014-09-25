#include "test.h"

SensorHandlerTest::SensorHandlerTest()
{
    TEST_ADD(SensorHandlerTest::test);
}

void SensorHandlerTest::tear_down()
{
}

void SensorHandlerTest::setup()
{
    sh = new sensor::SensorHandler(NULL,NULL,new SensorDetector(NULL,0L));
}

void SensorHandlerTest::test()
{
    TEST_FAIL("this will always fail");
    TEST_FAIL("this assert will never be executed");
}

/*!
 * \brief main
 */
int main( int argc, char **argv)
{
    SensorHandlerTest sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return !sts.run(output, true);
}
