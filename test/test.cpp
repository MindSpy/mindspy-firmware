#include <cpptest.h>


/*!
 * \brief The SensorHandlerTest class
 */
class SensorHandlerTest : public Test::Suite
{
public:
    /*!
     * \brief SensorHandlerTest
     */
    SensorHandlerTest()
    {
        TEST_ADD(SensorHandlerTest::test);
    }
protected:
    virtual void setup()     {}
    virtual void tear_down() {}
private:
    /*!
     * \brief test
     */
    void test()
    {
        TEST_FAIL("this will always fail");
        TEST_FAIL("this assert will never be executed");
    }
};

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main( int argc, char **argv)
{
    SensorHandlerTest sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return !sts.run(output, true);
}
