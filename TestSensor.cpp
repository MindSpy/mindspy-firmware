#include <limits.h>
#include "TestSensor.hpp"
#include "Sensor.hpp"
#include "proto.h"

#ifndef _UNITTEST
#include <Arduino.h>
#else
#include "compat.hpp"
#endif

namespace mindspy {

namespace sensor {

TestSensor::TestSensor(const char* name, const uint8_t rate, const uint8_t chan) :
        name(name), sequence(0), rate(rate), srate(rate), channels(chan), amplitude(4096), lastTime(ULONG_MAX) {
}

bool TestSensor::getSamples(uint32_t count, mindspy_protobufs_Sample* result) {
    for (unsigned int i = 0; i < count; i++) {
        while (!(*this))
            delayMicroseconds(10);

        result[i].sequence = sequence++;
        result[i].payload_count = channels;
        for (uint8_t j = 0; j < channels; j++) {
            result[i].payload[j] = ((amplitude * rate * lastTime / 1000) % amplitude) - amplitude/2;
        }

        lastTime += 1000 / srate;

        delayMicroseconds(10);
    }
    return true;
}

uint32_t TestSensor::getState(uint32_t address) {
    delayMicroseconds(10);
    switch (address) {
    case 0:  // deal with rate
        return srate;
    case 1:
        return channels;
    case 2:
        return rate;
    case 3:
        return amplitude;
    default:
        return 0xff;
    }
}
bool TestSensor::setState(mindspy_protobufs_State state) {
    switch (state.address) {
    case 0:  // deal with rate
        srate = state.payload;
        break;
    case 1:
        channels = state.payload;
        break;
    case 2:
        rate = state.payload;
        break;
    case 3:
        amplitude = state.payload;
        break;
    default:
        break;
    }
    delayMicroseconds(10);
    return true;
}

bool TestSensor::getState(uint32_t* addresses, uint32_t count, mindspy_protobufs_State* result) {
    for (uint32_t i = 0; i < count; i++) {
        result[i].address = addresses[i];
        result[i].payload = getState(addresses[i]);
    }
    return true;
}

bool TestSensor::setState(mindspy_protobufs_State* states, uint32_t count, void*result) {
    for (uint32_t i = 0; i < count; i++) {
        setState(states[i]);
    }
    return true;
}

bool TestSensor::getModelName(char* result) {
    // check overflows !
    return strcpy(result, name);
}

bool TestSensor::operator!(void) {
    delayMicroseconds(1);
    if (lastTime == ULONG_MAX)
        return true;
    return (lastTime + 1000 / srate) > millis();
}

void TestSensor::begin() {
    lastTime = millis();
}

void TestSensor::end() {
    lastTime = ULONG_MAX;
}

} // namespace

} // namespace
