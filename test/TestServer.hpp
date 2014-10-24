#ifndef PROTOCOLSOCKET_H
#define PROTOCOLSOCKET_H

#include "compat.hpp"
#include "ProtocolWrapper.hpp"
#include "Sensor.hpp"
#include "TestSensors.hpp"
#include <iostream>

class TestServer
{
public:
    TestServer(const char* , const char* );
    ~TestServer();
    void handle();

private:
    Stream * stream;
    ProtocolWrapper wrapper;
    TestSensors sensors;
};


#endif // PROTOCOLSOCKET_H
