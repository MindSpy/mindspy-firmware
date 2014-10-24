#ifndef SENSORTEST_H
#define SENSORTEST_H

#include <cpptest.h>
#include "Sensor.hpp"

#define COUNT_OF_ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


namespace mindspy {
namespace test {

class SensorTest : public Test::Suite
{
public:
    SensorTest();
protected:
    // inherit
    virtual void setup();
    virtual void tear_down();
private:
    const char* SENSOR_NAME = "TestSensor";
    const unsigned char SAMPLE_RATE = 128;
    const unsigned char CHANNEL_COUNT = 8;

    sensor::Sensor* sensor;

    void testBoolOperator();
    void testGetModelName();
    void testGetSamples();
    void testGetState();
    void testSetState();
};

}
} // namspace

#endif // SENSORTEST_H
