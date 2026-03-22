#include "Led.h"

// ====== LED FUNCTIONS ======

// A function that turns on the led given a color
void ledOn(CRGB color) {
  leds[0] = color;
  FastLED.show();
}

// A function that turns off the led
void ledOff() {
  leds[0] = CRGB::Black;
  FastLED.show();
}

// A function that sets the led to the color responding to the state
void setLed(int state){
  ledOn(stateColors[state]);
}