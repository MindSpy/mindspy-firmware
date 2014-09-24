/*
 TestSensor for unit tests

 created 22 Sep 2014
 by pBorky
 */
#ifndef TestSensor_h
#define TestSensor_h

#include <Arduino.h>
#include "Sensor.h"
#include "proto.h"

/**
 * TestSensor for unit tests
 */
class TestSensor: public sensor::Sensor {
private:

	const char * _name;
	uint64_t _sequence;
	uint32_t _lastTime;
	uint8_t _rate;
	uint8_t _channels;
	uint8_t getState(uint32_t);
	bool setState(State state);

public:

	/**
	 * Contructor
	 * \param model name of the sensor
	 * \param rate of generated waveform
	 * \param number of generated channels
	 */
	TestSensor(const char* name, const uint8_t rate, const uint8_t chan) :
			_name(name), _rate(rate), _channels(chan) {}


	// override ISensor methods
	bool getSamples(uint32_t, Sample*);
	bool getState(uint32_t*, uint32_t, State*);
	bool setState(State*, uint32_t, void*);
	bool getModelName(char*);
	operator bool(void);

};

#endif
