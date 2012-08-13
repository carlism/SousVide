#include "Button.h"

Button::Button(int inputPin)
	: pin(inputPin), lastReading(LOW), lastButtonState(LOW), lastDebounceTime(0), buttonState(LOW),
          heldOnce(false), buttonHeldStart(0)
{}

void Button::setup()
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

int Button::check()
{
  int result = 0;
  int reading = digitalRead(pin);

  if (reading != lastReading) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > DEBOUNCE_THRESHOLD) {
    buttonState = reading;
  }
  lastReading = reading;  

  
  if(buttonState == HIGH && lastButtonState == LOW) {
    // button released
    if(!heldOnce) {
      result = BUTTON_CLICKED;
    }
  } else if(buttonState == LOW && lastButtonState == HIGH) {
    // button pressed
    buttonHeldStart = millis();
    heldOnce = false;
  } else if(buttonState == HIGH && lastButtonState == HIGH) {
    // button held
    if((millis() - buttonHeldStart) > HOLD_THRESHOLD) {
      result = BUTTON_HELD;
      buttonHeldStart = millis();
      heldOnce = true;
    }
  }

  lastButtonState = buttonState;
  return result;
}
