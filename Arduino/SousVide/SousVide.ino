#include "SousVide.h"

#include<stdlib.h>
#include <LiquidCrystal.h>
#include "Button.h"

// This library from http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip
#include <OneWire.h> 

// clone this library from https://github.com/milesburton/Arduino-Temperature-Control-Library.git
#include <DallasTemperature.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Button increaseButton(INC_BUTTON);
Button decreaseButton(DEC_BUTTON);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress waterThermometer;

float temp_setting = 130.0;
float temp_reading = 1024.0;
unsigned long temp_start = 0;

char printBuffer[25];

void writeReading() {
  lcd.setCursor(0, 0);
  lcd.write(TEMPCHAR);
  lcd.print(dtostrf(temp_reading,7,2,printBuffer));
}

void writeSetting() {
  lcd.setCursor(0, 1);
  lcd.write(DIALCHAR);
  lcd.print(dtostrf(temp_setting,7,2,printBuffer));
}

void setup() {
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  lcd.createChar(TEMPCHAR, temp);
  lcd.createChar(DIALCHAR, dial);
  increaseButton.setup();
  decreaseButton.setup();
  
  delay(DISPLAY_DELAY);  
  lcd.setCursor(0, 0);
  lcd.print("Locating devices...");
  sensors.begin();
  lcd.setCursor(0, 1);
  lcd.print("Found ");
  lcd.print(sensors.getDeviceCount(), DEC);

  delay(DISPLAY_DELAY);
  lcd.clear();
  lcd.setCursor(0, 0);
  if (!sensors.getAddress(waterThermometer, 0)) {
    lcd.print("Can't find address, Device 0"); 
  } else {
    lcd.print("Found address");
    sensors.setResolution(waterThermometer, TEMPERATURE_PRECISION);
  }
  
  sensors.setWaitForConversion(false);

  pinMode(PUMP_CONTROL, OUTPUT); 
  digitalWrite(PUMP_CONTROL, HIGH);
  pinMode(HEATER_CONTROL, OUTPUT); 
  digitalWrite(HEATER_CONTROL, LOW);
  
  delay(DISPLAY_DELAY);
  lcd.clear();
}

void loop() {
  int button = increaseButton.check();
  if (button == BUTTON_CLICKED) {
    Serial.print("increase button: ");
    Serial.println(button);
    temp_setting++;
  }

  button = decreaseButton.check();
  if (button == BUTTON_CLICKED) {
    Serial.print("decrease button: ");
    Serial.println(button);
    temp_setting--;
  }
  
  if(temp_start == 0) {
    // initialize timer and make request
    sensors.requestTemperatures();
    temp_start = millis();
  } else if ((millis() - temp_start) > 750) {
    // timed out, read value and reset
    temp_reading = sensors.getTempF(waterThermometer);
    sensors.requestTemperatures();
    temp_start = millis();
  }    
  
  writeReading();
  writeSetting();
  
  if(temp_reading >= temp_setting) {  
    digitalWrite(HEATER_CONTROL, LOW);
  } else {
    digitalWrite(HEATER_CONTROL, HIGH);
  }
}
