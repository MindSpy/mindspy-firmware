#ifndef PROTOCOLTEST_HPP
#define PROTOCOLTEST_HPP

#include <cpptest.h>
#include <stdint.h>
#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/message.h>
#include "Proto.hpp"

using namespace google::protobuf;

class ProtocolTest : public Test::Suite
{
public:
    ProtocolTest();
protected:
    // inherit
    virtual void setup();
    virtual void tear_down();
private:
    std::iostream* in;
    std::iostream* out;

    void testLocal();
    void testRemote();

    uint64_t timestamp();
    uint64_t reqid();
    void communicate(void*, void*);
    bool writeDelimitedTo(const MessageLite*, io::ZeroCopyOutputStream*);
    bool readDelimitedFrom(io::ZeroCopyInputStream*, MessageLite*);
};


#endif // PROTOCOLTEST_HPP
