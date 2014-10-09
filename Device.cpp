#include "Device.hpp"

#include "Arduino.h"

Device::Device()  {
}

Device::~Device() {
}

void Device::setupBluetooth() {
    // Initialize pins for bluetooth module
    pinMode(GREEN_LED, INPUT);
    pinMode(BLUE_LED, INPUT);
    pinMode(PB_2, OUTPUT);
    pinMode(PB_3, OUTPUT);

    // Reset bluetooth.
    digitalWrite(PB_2, LOW);
    delay(20);
    digitalWrite(PB_2, HIGH);

    // setup bluetooth.
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

void Device::setupUsb() {
    USB_STREAM.begin(USB_STREAM_BAUD);
}

void Device::setupDevice() {
    protocolWrapper.setStream(&PB_STREAM);
    protocolWrapper.setSensors(&sensorDetector);
}

void Device::setup() {
    //setupBluetooth();
    setupUsb();
    setupDevice();
}
