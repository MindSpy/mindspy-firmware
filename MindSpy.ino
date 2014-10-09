#include <Arduino.h>
#include <SPI.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "Device.hpp"

Device device;

void setup() {
    device.setup();
}

void loop() {
    device.protocolWrapper.handle();
}

