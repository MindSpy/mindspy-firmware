#include "TestSensors.hpp"
#include "TestSensor.hpp"

namespace mindspy {
namespace test {

TestSensors::TestSensors() {
    sensors.push_back(new sensor::TestSensor("TestSensor", 128, 8));

    for (sensor::Sensors::iterator s = sensors.begin(); s != sensors.end(); s++) {
        (*s)->begin();
    }
}

TestSensors::~TestSensors() {
}


sensor::Sensors::iterator TestSensors::begin() {
    return sensors.begin();
}

sensor::Sensors::iterator TestSensors::end() {
    return sensors.end();
}

size_t TestSensors::count() {
    return sensors.size();
}

sensor::Sensor*& TestSensors::operator[](size_t idx) {
    return sensors[idx];
}

}
} // namespace
