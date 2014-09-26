#include "test.h"

#include "SensorHandler.h"
#include "SensorDetector.h"
#include "Sensor.h"
#include "TestSensor.h"
#include "ProtocolWrapper.h"
#include "macros.h"
#include "FileStream.h"
#include "MemoryStream.h"
#include <stdlib.h>
#include <unistd.h>

const char* SENSOR_NAME = "TestSensor";
const unsigned char SAMPLE_RATE = 128;
const unsigned char CHANNEL_COUNT = 8;

SensorTest::SensorTest()
{
    TEST_ADD(SensorTest::testBoolOperator);
    TEST_ADD(SensorTest::testGetModelName);
    TEST_ADD(SensorTest::testGetState);
    TEST_ADD(SensorTest::testSetState);
    TEST_ADD(SensorTest::testGetSamples);
}

void SensorTest::setup()
{
    sensor = new TestSensor(SENSOR_NAME, SAMPLE_RATE, CHANNEL_COUNT);
}

void SensorTest::tear_down()
{
    free(sensor);
}

void SensorTest::testBoolOperator()
{
    sensor->end();
    TEST_ASSERT_MSG(!(*sensor), "Sensor should not collect data.");
    sensor->begin();
    TEST_ASSERT_MSG(!(*sensor), "Sensor should not have data.");
    usleep(100000);
    TEST_ASSERT_MSG(!!(*sensor), "Sensor should collect data.");
    sensor->end();
}

void SensorTest::testGetModelName()
{
    char name[64];
    sensor->getModelName(name);

    TEST_ASSERT_MSG(!strcmp(SENSOR_NAME, name), "Model name mismatch.");
}

void SensorTest::testGetState()
{
    uint32_t addresses[] = {0,1};
    State states[2];
    sensor->getState(addresses, COUNT_OF(addresses), states);
    TEST_ASSERT_EQUALS_MSG(SAMPLE_RATE, states[0].payload, "Sample rate mismatch.");
    TEST_ASSERT_EQUALS_MSG(CHANNEL_COUNT, states[1].payload, "Channel count mismatch.");
}

void SensorTest::testSetState() {
    const unsigned char NEW_RATE = 210;
    const unsigned char NEW_CHAN = 2;
    // set values
    State states[] = {
        { 0, NEW_RATE },
        { 1, NEW_CHAN }
    };
    sensor->setState(states, 2, NULL);

    // check
    uint32_t addresses[] = {0,1};
    sensor->getState(addresses, 2, states);
    TEST_ASSERT_EQUALS_MSG(NEW_RATE, states[0].payload, "Sample rate mismatch.");
    TEST_ASSERT_EQUALS_MSG(NEW_CHAN, states[1].payload, "Channel count mismatch.");

    // revert back
    states[0].payload = SAMPLE_RATE;
    states[1].payload = CHANNEL_COUNT;
    sensor->setState(states, 2, NULL);
}

void SensorTest::testGetSamples()
{
    sensor->begin();
    for (int i = 0; i < 30; i++) {
        Sample samples[10];
        sensor->getSamples(10, samples);
        // chack  10 samples
    }
    sensor->end();
}

ProtocolWrapperTest::ProtocolWrapperTest()
{
    TEST_ADD(ProtocolWrapperTest::testHandle);
}

void ProtocolWrapperTest::setup()
{
    obuffer = (char*)malloc(1024 * sizeof(char)); // 1KB
    ibuffer = (char*)malloc(1024 * 1024 * sizeof(char)); // 1MB
    size_t olen;
    // prepare obuffer set the olen

    sensor::Sensor* sensors[] = { new TestSensor("TestSensor", 128L,8L) };
    wrapper = new ProtocolWrapper(new SensorDetector(sensors,COUNT_OF(sensors)));
    ProtocolWrapper::setStream(new MemoryStream(obuffer, ibuffer, olen));
}

void ProtocolWrapperTest::tear_down()
{
    free(ibuffer);
    free(obuffer);
}

void ProtocolWrapperTest::testHandle()
{
    //TEST_FAIL("this will always fail");
    //TEST_FAIL("this assert will never be executed");
}

int main( int argc, char **argv)
{
    Test::Suite ts;
    ts.add(std::auto_ptr<Test::Suite>(new SensorTest));
    ts.add(std::auto_ptr<Test::Suite>(new ProtocolWrapperTest));

    Test::TextOutput output(Test::TextOutput::Verbose);
    return !ts.run(output);
}
