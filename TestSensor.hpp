/*
 TestSensor for unit tests

 created 22 Sep 2014
 by pBorky
 */
#ifndef TestSensor_h
#define TestSensor_h

#include "Sensor.h"
#include "proto.h"

/*!
 * \brief TestSensor for unit tests
 */
class TestSensor: public sensor::Sensor {

public:

    /*!
     * \brief Contructor
     * \param model name of the sensor
     * \param rate of generated waveform
     * \param number of generated channels
     */
    TestSensor(const char*, const uint8_t, const uint8_t);

    // override Sensor methods
    bool getSamples(uint32_t, Sample*);
    bool getState(uint32_t*, uint32_t, State*);
    bool setState(State*, uint32_t, void*);
    bool getModelName(char*);
    bool operator!(void);
    void begin();
    void end();

private:

    const char * _name;
    uint64_t _sequence;
    uint8_t _rate;
    uint8_t _channels;
    unsigned long _lastTime;

    uint8_t getState(uint32_t);
    bool setState(State state);

};

#endif
