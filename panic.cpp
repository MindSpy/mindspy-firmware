/*
 * panic.cpp
 *
 *  Created on: 7. 10. 2014
 *      Author: Razor
 */


#ifndef _UNITTEST

#include "Arduino.h"

void panic() {
    pinMode(RED_LED, OUTPUT);

    // blink the red LED in a loop
    for (;;) {
        digitalWrite(RED_LED, HIGH);
        delay(200);
        digitalWrite(RED_LED, LOW);
        delay(200);
    }
}

#else

#include <stdlib.h>

void panic() {
    exit(1);
}

#endif
