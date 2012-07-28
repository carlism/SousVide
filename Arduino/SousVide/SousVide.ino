#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "SousVide.h"
#include "MomentaryButton.h"
#include "floatToString.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float temp_setting = 130.0;
MomentaryButton increaseButton(INC_BUTTON);
MomentaryButton decreaseButton(DEC_BUTTON);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress waterThermometer;
int     pump = 8;
int     heater = 13;

char printBuffer[25];

void writeReading(float reading) {
  lcd.setCursor(0, 0);
  lcd.write(TEMPCHAR);
  lcd.print(floatToString(printBuffer,reading,2,7,true));
}

void writeSetting() {
  lcd.setCursor(0, 1);
  lcd.write(DIALCHAR);
  lcd.print(floatToString(printBuffer,temp_setting,2,7,true));
}

void setup() {
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
  lcd.print(" device(s).");

  delay(DISPLAY_DELAY);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parasite mode:"); 
  lcd.setCursor(0, 1);
  if (sensors.isParasitePowerMode()) lcd.print("ON");
  else lcd.println("OFF");

  delay(DISPLAY_DELAY);
  lcd.clear();
  lcd.setCursor(0, 0);
  if (!sensors.getAddress(waterThermometer, 0)) {
    lcd.print("Can't find address, Device 0"); 
  } else {
    lcd.print("Found address");
    sensors.setResolution(waterThermometer, TEMPERATURE_PRECISION);
  }

  delay(DISPLAY_DELAY);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting circulator"); 
  pinMode(pump, OUTPUT); 
  digitalWrite(pump, HIGH);
  pinMode(heater, OUTPUT); 
  digitalWrite(heater, LOW);
  
  delay(DISPLAY_DELAY);
  lcd.clear();
}

void loop() {
  increaseButton.check();
  if (increaseButton.wasHeld()) {
    temp_setting+=5;
  } else if (increaseButton.wasClicked()) {
    temp_setting++;
  }

  decreaseButton.check();
  if (decreaseButton.wasHeld()) {
    temp_setting-=5;
  } else if (decreaseButton.wasClicked()) {
    temp_setting--;
  }
  
  sensors.requestTemperatures();
  float tempF = sensors.getTempF(waterThermometer);
  
  writeReading(tempF);
  writeSetting();
  
  
  if(tempF >= temp_setting) {  
    digitalWrite(heater, LOW);
  } else {
    digitalWrite(heater, HIGH);
  }
}
