#pragma once

#include "Arduino.h"

#define DEBOUNCE_THRESHOLD 50
#define HOLD_THRESHOLD   750
#define NO_CHANGE 0
#define BUTTON_CLICKED 1
#define BUTTON_HELD 2

class Button {
private:
  const int pin;
  int buttonState;
  int lastButtonState;
  int lastReading;
  long lastDebounceTime;
  bool heldOnce;
  long buttonHeldStart;
public:
  Button(int inputPin);
  void setup();
  int check();
};


