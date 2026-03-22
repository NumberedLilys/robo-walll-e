#pragma once
#include "Utility.h"

// ====== LED FUNCTIONS ======

// A function that turns on the led given a color
void ledOn(CRGB color);

// A function that turns off the led
void ledOff();

// A function that sets the led to the color responding to the state
void setLed(int state);

