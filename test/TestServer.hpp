#ifndef PROTOCOLSOCKET_H
#define PROTOCOLSOCKET_H

#include "compat.hpp"
#include "SensorHandler.hpp"
#include "SensorDetector.hpp"
#include "Sensor.h"
#include "TestSensor.hpp"
#include "ProtocolWrapper.hpp"
#include "FileStream.hpp"

class TestServer
{
public:
    TestServer(FILE* in, FILE* out);
    ~TestServer();
    void handle();

private:
    Stream * stream;
    ProtocolWrapper* wrapper;
    sensor::Sensor** sensors;
    SensorDetector* detector;
};

extern "C" {

int main(int, char **);

}

#endif // PROTOCOLSOCKET_H
