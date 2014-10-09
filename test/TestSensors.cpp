#include "TestSensors.hpp"

#include "TestSensor.hpp"

TestSensors::TestSensors() {
    sensors.push_back(new TestSensor("TestSensor", 128, 8));

    for (Sensors::iterator s = sensors.begin(); s != sensors.end(); s++) {
        (*s)->begin();
    }
}

TestSensors::~TestSensors() {
}


Sensors::iterator TestSensors::begin() {
    return sensors.begin();
}

Sensors::iterator TestSensors::end() {
    return sensors.end();
}

size_t TestSensors::count() {
    return sensors.size();
}

sensor::Sensor*& TestSensors::operator[](size_t idx) {
    return sensors[idx];
}
