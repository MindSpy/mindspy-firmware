#include "SensorDetector.hpp"

#include "panic.hpp"
#include "Sensors.hpp"
#include "TestSensor.hpp"
//#include "ADS1x9y.hpp"

SensorDetector::SensorDetector() {
    sensors.push_back(new TestSensor("TestSensor", 128, 8));

    for (Sensors::iterator s = sensors.begin(); s != sensors.end(); s++) {
        (*s)->begin();
    }
}

SensorDetector::~SensorDetector() {
    for (Sensors::iterator s = sensors.begin(); s != sensors.end(); s++) {
        delete (*s);
    }
}

sensor::Sensor*& SensorDetector::operator[](const size_t idx)  {
    if (idx < 0 || idx >= sensors.size())
        panic();
    return sensors[idx];
}

size_t SensorDetector::count() {
    return sensors.size();
}

Sensors::iterator  SensorDetector::begin() {
    return sensors.begin();
}

Sensors::iterator  SensorDetector::end() {
    return sensors.end();
}
