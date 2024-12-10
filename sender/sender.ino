#include <Keypad.h>
#include <RadioLib.h>

#define CLICKER_ID 'Z' // Each sender should have a unique ID, e.g. 'C'

SX1262 radio = new Module(8, 14, 12, 13);
volatile bool packetReceived = false;
int transmissionState = RADIOLIB_ERR_NONE;

#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  packetReceived = true;
}

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
  int state = radio.begin(434.0, 500.0, 6, 5, RADIOLIB_SX127X_SYNC_WORD, 10, 6, 1.6);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // Set up DIO1 interrupt for async reception
  radio.setDio1Action(setFlag);

  // Start in receive mode
  state = radio.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Failed to enter receive mode, code ");
    Serial.println(state);
  }
}

void loop() {
  char keyPressed = customKeypad.getKey();
  if (keyPressed) {
    Serial.println(keyPressed);
    bool success = sendAndWaitForAck(keyPressed, CLICKER_ID);
    if (success) {
      Serial.println("Message acknowledged by receiver!");
    } else {
      Serial.println("No acknowledgment received after retries.");
    }
  }
}

// Attempts to send `keyPressed` along with the ID, then waits for the correct ACK.
// Returns true if ACK received, false if not.
bool sendAndWaitForAck(char key, char id) {
  char dataToSend[3];
  dataToSend[0] = key;
  dataToSend[1] = id;
  dataToSend[2] = '\0';

  // The expected ack should be something like "ACKC" if ID is 'C'
  char expectedAck[5] = "ACK?";
  expectedAck[3] = id; 
  expectedAck[4] = '\0';

  const int maxRetries = 5;
  const unsigned long ackTimeout = 1000; // 1 second timeout
  bool ackReceived = false;

  for (int attempt = 1; attempt <= maxRetries && !ackReceived; attempt++) {
    Serial.print("Sending attempt ");
    Serial.println(attempt);

    // Transmit data
    transmissionState = radio.transmit(dataToSend);
    if (transmissionState == RADIOLIB_ERR_NONE) {
      Serial.println("Data sent, waiting for ACK...");

      // Switch to receive mode
      int state = radio.startReceive();
      if (state != RADIOLIB_ERR_NONE) {
        Serial.print("Failed to switch to receive mode, code ");
        Serial.println(state);
        // Retry next loop iteration
      } else {
        // Wait for ACK or timeout
        ackReceived = waitForAck(expectedAck, ackTimeout);
        if (!ackReceived) {
          Serial.println("ACK not received within timeout, retrying...");
          // Optional: Random backoff to reduce collisions
          delay(random(200, 500));
        }
      }
    } else {
      Serial.print("Error transmitting: ");
      Serial.println(transmissionState);
      // Wait before retrying to avoid immediate collision
      delay(random(200, 500));
    }
  }

  // Return to receive mode after all attempts
  radio.startReceive();
  return ackReceived;
}

// Wait for a packet that matches expectedAck within timeout.
// Returns true if the matching ACK is received, false otherwise.
bool waitForAck(const char* expectedAck, unsigned long timeout) {
  packetReceived = false;
  unsigned long startTime = millis();

  while (millis() - startTime < timeout) {
    if (packetReceived) {
      packetReceived = false;
      String incoming;
      int state = radio.readData(incoming);
      if (state == RADIOLIB_ERR_NONE) {
        Serial.print("Received: ");
        Serial.println(incoming);
        if (incoming.equals(expectedAck)) {
          Serial.println("Correct ACK received!");
          return true;
        } else {
          Serial.println("Received a packet but it wasn't the expected ACK.");
        }
      } else {
        Serial.print("Failed to read data, code ");
        Serial.println(state);
      }
      // If we got here, either it wasn't the correct ack or read failed, continue listening
    }
    delay(10);
  }

  // Timeout reached without receiving correct ACK
  return false;
}
