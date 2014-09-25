#include "test.h"

#include "SensorHandler.h"
#include "SensorDetector.h"
#include "Sensor.h"
#include "TestSensor.h"
#include "ProtocolWrapper.h"
#include "macros.h"
#include "FileStream.h"

SensorHandlerTest::SensorHandlerTest()
{
    TEST_ADD(SensorHandlerTest::test);
}

void SensorHandlerTest::tear_down()
{
}

void SensorHandlerTest::setup()
{
    sensor::Sensor* sensors[] = { new TestSensor("TestSensor", 128L,8L) };
    wrapper = new ProtocolWrapper(new SensorDetector(sensors,COUNT_OF(sensors)));
    //ProtocolWrapper::setStream(new FileStream());
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
