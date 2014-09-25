#include <Arduino.h>
#include <SPI.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "Main.h"
#include "SensorHandler.h"
#include "SensorDetector.h"
#include "ADS1x9y.h"
#include "TestSensor.h"
#include "macros.h"

sensor::SensorHandler* handler = NULL;
uint64_t bootTime = 0;

void setupBluetooth() {
    // Initialize pins for bluetooth module
    pinMode(GREEN_LED, INPUT);
    pinMode(BLUE_LED, INPUT);
    pinMode(PB_2, OUTPUT);
    pinMode(PB_3, OUTPUT);

    // Reset bluetooth.
    digitalWrite(PB_2, LOW);
    delay(20);
    digitalWrite(PB_2, HIGH);

    // Setup bluetooth.
    digitalWrite(PB_3, HIGH);
    // Initialize speed
    BLUETOOTH_STREAM.begin(BLUETOOTH_STREAM_INIT_BAUD);
    // Set new speed
    BLUETOOTH_STREAM.print("AT+UART=");
    BLUETOOTH_STREAM.print(BLUETOOTH_STREAM_BAUD);
    BLUETOOTH_STREAM.print(",1,0\r\n");
    BLUETOOTH_STREAM.print("AT+NAME=MindSpy\r\n");
    BLUETOOTH_STREAM.print("AT+ROLE=0\r\n");

    delay(10);
    digitalWrite(PB_3, LOW);

    // Start bt stream on a new speed.
    BLUETOOTH_STREAM.end();
    BLUETOOTH_STREAM.begin(BLUETOOTH_STREAM_BAUD); // max. 1382400=1.3Mbps
}
void setupUsb() {
    USB_STREAM.begin(USB_STREAM_BAUD);
}

void setup() {
    // initialize BT
    setupBluetooth();
    // Initialize USB
    setupUsb();

    // TODO this will be in SensorDetector once it is finished
    sensor::Sensor* sensors[] = { new TestSensor("TestSensor", 128, 8), new ADS1x9y(0) };
    SensorDetector* detector = new SensorDetector(sensors, COUNT_OF(sensors));
    // initialize handler
    handler = new sensor::SensorHandler(&timestamp, &stopStream, detector);
}

void loop() {
    if (handler != NULL)
    {
        // volatile streams for nanopb library.
        pb_istream_t istream = { &read_callback, &PB_STREAM, SIZE_MAX };
        pb_ostream_t ostream = { &write_callback, &PB_STREAM, SIZE_MAX, 0 };

        // Handle incomming request.
        handler->handle(&istream, &ostream);
    }
}

bool stopStream(void) {
    return !!PB_STREAM.available();
}

uint64_t timestamp(void) {
    return bootTime + micros();
}

bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count) {
    Stream* s = (Stream*) stream->state;
    return s->write(buf, count) == count;
}

bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count) {
    Stream* s = (Stream*) stream->state;
    size_t avail = 0;

    // wait for enough data
    while (avail < count) {
        avail = s->available();
        delayMicroseconds(1);
    }

    size_t result = s->readBytes((char*) buf, constrain(avail, 0, count));
    return result == count;
}
