/*
 * panic.cpp
 *
 *  Created on: 7. 10. 2014
 *      Author: Razor
 */

#include "Arduino.h"

void panic() {
    pinMode(RED_LED, OUTPUT);

    // blink the red LED in a loop
    for (;;) {
        digitalWrite(RED_LED, HIGH);
        delay(500);
        digitalWrite(RED_LED, LOW);
        delay(500);
    }
}
