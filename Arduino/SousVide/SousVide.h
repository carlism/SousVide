#define INC_BUTTON 6
#define DEC_BUTTON 7
#define ONE_WIRE_BUS 10

#define TEMPERATURE_PRECISION 9
#define DISPLAY_DELAY 2000
#define TEMPCHAR 2
#define DIALCHAR 3
byte temp[8] = {
  0b00110,
  0b00010,
  0b00110,
  0b00010,
  0b00110,
  0b00010,
  0b00111,
  0b00111
};
byte dial[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b11101,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

