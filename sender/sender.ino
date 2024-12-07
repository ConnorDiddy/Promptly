#include <Keypad.h>
#include <RadioLib.h>

/* SET THIS AS A UNIQUE ID FOR EACH CLICKER */
#define CLICKER_ID 0xC3

/************************
* LORA Initialization
************************/
SX1262 radio = new Module(8, 14, 12, 13);
int transmissionState = RADIOLIB_ERR_NONE;
volatile bool packetReceived = false;
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
  // we sent or received a packet, set the flag
  packetReceived = true;
}

/************************
* Keypad Initialization
************************/
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4}; 
byte colPins[COLS] = {3, 2, 1, 38}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
    {
      delay(10);
    }
  }
}


void loop() {
  char keyPressed = customKeypad.getKey();
  if (keyPressed) {
    Serial.println(keyPressed);
    
    const char data[2] = {keyPressed, CLICKER_ID};
    transmissionState = radio.startTransmit(data);
  }
}