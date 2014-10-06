#include "UnitTests.hpp"

#include "SensorHandler.hpp"
#include "SensorDetector.hpp"
#include "Sensor.h"
#include "TestSensor.hpp"
#include "ProtocolWrapper.hpp"
#include "macros.h"
#include "FileStream.hpp"
#include "MemoryStream.hpp"
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
    delete sensor;
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
    Sample samples[10];
    sensor->begin();
    for (int i = 0; i < 10; i++) {
        sensor->getSamples(10, samples);
        for (int j = 1; j < 10; j++) {
            TEST_ASSERT_EQUALS_MSG(samples[j-1].sequence + 1, samples[j].sequence, "Sequence should grow by 1.");
            for (int k = 1; k < 8; k++) {
                TEST_ASSERT_EQUALS_MSG(samples[j].payload[k-1] + 1,samples[j].payload[k],"Payload ")
            }
        }
    }
    sensor->end();
}

ProtocolWrapperTest::ProtocolWrapperTest()
{
    TEST_ADD(ProtocolWrapperTest::testHandle);
}

void ProtocolWrapperTest::setup()
{
    olen = 1024;// 1KB
    ilen = 1024*1024;// 1MB
    obuffer = (char*)malloc(olen * sizeof(char));
    ibuffer = (char*)malloc(ilen * sizeof(char));
    stream = new MemoryStream(obuffer, ibuffer, olen, ilen);
    ProtocolWrapper::setStream(stream);

    sensors = new sensor::Sensor*[1];
    sensors[0] = new TestSensor("TestSensor", 128L,8L);
    detector = new SensorDetector(sensors, 1);
    wrapper = new ProtocolWrapper(detector);
}

void ProtocolWrapperTest::tear_down()
{
    delete wrapper;
    delete detector;
    delete stream;
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
