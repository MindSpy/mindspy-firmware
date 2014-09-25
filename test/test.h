#ifndef _TEST_H
#define _TEST_H

#include <cpptest.h>
#include "ProtocolWrapper.h"

/*!
 * \brief The SensorHandlerTest class
 */
class SensorHandlerTest : public Test::Suite
{
public:
    /*!
     * \brief SensorHandlerTest
     */
    SensorHandlerTest();
protected:
    /*!
     * \brief setup
     */
    virtual void setup();
    /*!
     * \brief tear_down
     */
    virtual void tear_down();
private:
    /*!
     * \brief test
     */
    void test();
    ProtocolWrapper* wrapper;
};


#endif // TEST_H
