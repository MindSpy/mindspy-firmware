#include <cassert>
#include "Panic.hpp"
#include "SensorDetector.hpp"

SensorDetector::SensorDetector(std::vector<sensor::Sensor*> &sensors)
    : sensors(sensors) {
    for (std::vector<sensor::Sensor*>::iterator s = sensors.begin(); s != sensors.end(); s++) {
        (*s)->begin();
    }
}

SensorDetector::~SensorDetector() {

}


sensor::Sensor*& SensorDetector::operator[](size_t idx) {
    if (idx < 0 || idx >= sensors.size())
        panic();
    return sensors[idx];
}
