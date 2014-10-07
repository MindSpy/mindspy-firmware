#include "SensorDetector.hpp"

SensorDetector::SensorDetector(std::vector<sensor::Sensor*> &sensor) {
    for (unsigned int i = 0; i < sensor.size(); i++) {
        sensor[i]->begin();
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
