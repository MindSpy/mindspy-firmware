#include "SensorDetector.hpp"

SensorDetector::SensorDetector(sensor::Sensor** sensors, size_t countOfSensors) :
        _sensors(sensors), _countOfSensors(countOfSensors) {
    for (size_t i = 0; i < _countOfSensors; i++) {
        _sensors[i]->begin();
	}
}

SensorDetector::~SensorDetector() {

}

sensor::Sensor*& SensorDetector::operator[](size_t idx) {
    if (idx < 0)
        idx = 0;
    if (idx >= _countOfSensors)
        idx = _countOfSensors - 1;

    return _sensors[idx];
}

size_t SensorDetector::count() {
    return _countOfSensors;
}
