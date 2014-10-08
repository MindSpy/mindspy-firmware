#include "SensorDetector.hpp"

#include "TestSensor.hpp"
//#include "ADS1x9y.hpp"

SensorDetector::SensorDetector() {
    sensors.push_back(new TestSensor("TestSensor", 128, 8));
    //sensors.push_back(new ADS1x9y(0));
}

SensorDetector::~SensorDetector() {
    for (Sensors::iterator it = sensors.begin(); it != sensors.end(); ++it)
        delete *it;
}

sensor::Sensor*& SensorDetector::operator[](size_t idx) {
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
