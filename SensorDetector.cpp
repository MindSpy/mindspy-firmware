#include "SensorDetector.hpp"

SensorDetector::SensorDetector(sensor::Sensor** sensors, size_t countOfSensors) :
        sensors(sensors), countOfSensors(countOfSensors) {
    for (size_t i = 0; i < countOfSensors; i++) {
        sensors[i]->begin();
    }
}

SensorDetector::~SensorDetector() {

}

sensor::Sensor*& SensorDetector::operator[](size_t idx) {
    if (idx < 0)
        idx = 0;
    if (idx >= countOfSensors)
        idx = countOfSensors - 1;

    return sensors[idx];
}

size_t SensorDetector::count() {
    return countOfSensors;
}
