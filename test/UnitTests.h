#ifndef _TEST_H
#define _TEST_H

#include <cpptest.h>
#include "ProtocolWrapper.h"
#include "SensorHandler.h"
#include "Sensor.h"

class SensorTest : public Test::Suite
{
public:
    SensorTest();
protected:
    // inherit
    virtual void setup();
    virtual void tear_down();
private:
    sensor::Sensor* sensor;

    void testBoolOperator();
    void testGetModelName();
    void testGetSamples();
    void testGetState();
    void testSetState();
};

class ProtocolWrapperTest : public Test::Suite
{
public:
    ProtocolWrapperTest();
protected:
    // inherit
    virtual void setup();
    virtual void tear_down();
private:
    ProtocolWrapper* wrapper;
    char* ibuffer;
    char* obuffer;
    size_t olen;
    size_t ilen;
    Stream* stream;
    sensor::Sensor** sensors;
    SensorDetector* detector;

    void testHandle();
};




#endif // TEST_H
