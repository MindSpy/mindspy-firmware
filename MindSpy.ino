#include <Arduino.h>
#include <SPI.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "device.h"

Device device;

void setup() {
    // detect sensor
    device.SetupDevice();
}

void loop() {
    if (device.protocolWrapper != NULL) {
        device.protocolWrapper->handle();
    }
}

