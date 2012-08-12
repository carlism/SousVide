#include "Button.h"

Button::Button(int inputPin)
	: pin(inputPin), lastReading(LOW), lastButtonState(LOW), lastDebounceTime(0), buttonState(LOW)
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
    result = BUTTON_CLICKED;
  }

  lastButtonState = buttonState;
  return result;
}
