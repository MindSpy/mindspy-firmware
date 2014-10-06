#ifndef PROTOCOLSOCKET_H
#define PROTOCOLSOCKET_H

#include "compat.h"
#include "SensorHandler.h"
#include "SensorDetector.h"
#include "Sensor.h"
#include "TestSensor.h"
#include "ProtocolWrapper.h"
#include "FileStream.h"

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
