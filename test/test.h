#ifndef _TEST_H
#define _TEST_H

#include <cpptest.h>
#include "SensorHandler.h"
#include "SensorDetector.h"

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
    /*!
     * \brief sh
     */
    sensor::SensorHandler* sh;
};


#endif // TEST_H
