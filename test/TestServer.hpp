#ifndef PROTOCOLSOCKET_H
#define PROTOCOLSOCKET_H

#include "compat.hpp"
#include "ProtocolWrapper.hpp"
#include "Sensor.hpp"
#include "TestSensors.hpp"
#include <iostream>

namespace mindspy {
namespace test {

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


}
} // namespace

#endif // PROTOCOLSOCKET_H
