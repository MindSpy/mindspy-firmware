#ifndef PROTOCOLTEST_HPP
#define PROTOCOLTEST_HPP

#include <cpptest.h>
#include <stdint.h>
#include <iostream>


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

    void testHandle();
    void testLocal();
    void testRemote();

    uint64_t timestamp();
    uint64_t reqid();
    void communicate(void*, void*);
};


#endif // PROTOCOLTEST_HPP
