#pragma once
#include "Utility.h"

// ====== LED FUNCTIONS ======

void ledOn(CRGB color);

void ledOff();

void initializeLedColors(CRGB colorList[]);

void setLed(int state);

