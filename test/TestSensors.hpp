#ifndef TESTSENSORS_HPP
#define TESTSENSORS_HPP

#include "Sensors.hpp"
#include "Sensor.hpp"
#include <vector>

class TestSensors : public Sensors
{
public:
    TestSensors();
    ~TestSensors();

    iterator begin() ;
    iterator end();
    size_t count();
    sensor::Sensor*& operator[](size_t idx);

private:
    container  sensors;
};

#endif // TESTSENSORS_HPP
