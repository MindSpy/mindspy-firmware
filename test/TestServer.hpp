#ifndef PROTOCOLSOCKET_H
#define PROTOCOLSOCKET_H

#include "compat.hpp"
#include "ProtocolWrapper.hpp"
#include "SensorDetector.hpp"
#include "Sensor.h"
#include <iostream>

class TestServer
{
public:
    TestServer(std::istream* , std::ostream* );
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
