#include <cpptest.h>


class SensorHandlerTest : public Test::Suite
{
public:
    SensorHandlerTest()
    {
        TEST_ADD(SensorHandlerTest::test);
    }
protected:
    virtual void setup()     {}
    virtual void tear_down() {}
private:
    void test()
    {
        TEST_FAIL("this will always fail");
        TEST_FAIL("this assert will never be executed");
    }
};


int main( int argc, char **argv)
{
    SensorHandlerTest sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return !sts.run(output, true);
}
