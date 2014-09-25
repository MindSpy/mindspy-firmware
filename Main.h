#ifndef Main_h
#define Main_h

#include <Arduino.h>

// max. for HC05 is 1382400=1.3Mbps
const int BLUETOOTH_STREAM_BAUD = 921600;
const int BLUETOOTH_STREAM_INIT_BAUD = 38400;
const int USB_STREAM_BAUD = 115200;
// this must be macro ..
#define BLUETOOTH_STREAM Serial1
#define USB_STREAM Serial
// choose one stream for protocol buffers
#define PB_STREAM BLUETOOTH_STREAM

/*!
 * \brief Executed on boot by wiring.
 */
void setup(void);

/*!
 * \brief setupBT
 */
void setupBT(void);

/*!
 * \brief setupUsb
 */
void setupUsb(void);

/*!
 * \brief Executed within infinity loop by wiring
 */
void loop(void);

#endif
