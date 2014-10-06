/*
 TestSensor for unit tests

 created 22 Sep 2014
 by pBorky
 */

#include "TestSensor.hpp"
#include <limits.h>
#include "Sensor.h"
#include "proto.h"

#ifndef _UNITTEST
#include <Arduino.h>
#else
#include "compat.hpp"
#endif

TestSensor::TestSensor(const char* name, const uint8_t rate, const uint8_t chan) :
        _name(name), _rate(rate), _channels(chan), _lastTime(ULONG_MAX) {}

bool TestSensor::getSamples(uint32_t count, Sample* result) {
    for (int i = 0; i < count; i++) {
        while (!(*this))
            delayMicroseconds(10);

        result[i].sequence = _sequence++;
        result[i].payload_count = _channels;
        for (uint8_t j = 0; j < _channels; j++) {
            result[i].payload[j] = (result[i].sequence & 0xff) - 0x80 + j;
        }

        _lastTime += 1000 / _rate;

        delayMicroseconds(10);
    }
    return true;
}

uint8_t TestSensor::getState(uint32_t address) {
    delayMicroseconds(10);
    switch (address) {
    case 0:  // deal with rate
        return _rate;
    case 1:
        return _channels;
    default:
        return 0xff;
    }
}
bool TestSensor::setState(State state) {
    switch (state.address) {
    case 0:  // deal with rate
        _rate = state.payload;
        break;
    case 1:
        _channels = state.payload;
        break;
    }
    delayMicroseconds(10);
    return true;
}

bool TestSensor::getState(uint32_t* addresses, uint32_t count, State* result) {
    for (uint32_t i = 0; i < count; i++) {
        result[i].address = addresses[i];
        result[i].payload = getState(addresses[i]);
    }
    return true;
}

bool TestSensor::setState(State* states, uint32_t count, void*result) {
    for (uint32_t i = 0; i < count; i++) {
        setState(states[i]);
    }
    return true;
}

bool TestSensor::getModelName(char* result) {
    // check overflows !
    return strcpy(result, _name);
}

bool TestSensor::operator! (void) {
    delayMicroseconds(1);
    if (_lastTime == ULONG_MAX)
        return true;
    return (_lastTime + 1000 / _rate) > millis();
}

void TestSensor::begin() {
    _lastTime = millis();
}

void TestSensor::end() {
    _lastTime = ULONG_MAX;
}
