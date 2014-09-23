#ifndef Main_h
#define Main_h

#include <Arduino.h>
#include <pb.h>

// max. 1382400=1.3Mbps
#define BT_STREAM_BAUD 921600
#define BT_STREAM_INIT_BAUD 38400
#define LOG_STREAM_BAUD 115200
#define PB_STREAM Serial1
#define LOG_STREAM Serial

/**
 * Executed on boot by wiring.
 */
void setup(void);

/**
 * Executed within infinity loop by wiring
 */
void loop(void);

/**
 * Callback for nanopb output.
 */
bool write_callback(pb_ostream_t *, const uint8_t *, size_t);

/**
 * Callback for nanopb input.
 */
bool read_callback(pb_istream_t *, uint8_t*, size_t);

/**
 * Callback for SensorHandler::handle stop condition.
 * \return stop streaming
 */
bool stopStream(void);

/**
 * Callback for SensorHandler::handle timestamp generator
 * \return elapsed time from reference in microseconds
 */
uint64_t timestamp(void);

#endif
