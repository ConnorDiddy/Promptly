/*****************************
 * Screen Setup
 ****************************/
#include "HT_SSD1306Wire.h"
#include "heltec.h"

static SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

void printToScreen(String s) {
  display.clear();
  display.drawString(0, 0, s);
  display.display();
}

/*****************************
 * Bluetooth Setup
 ****************************/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>

#define DEVICE_NAME         "Promptly Receiver"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
String message = "";

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      printToScreen("BLE client connected.");
    };

    void onDisconnect(BLEServer* pServer) {
      printToScreen("BLE client disconnected.");
      BLEDevice::startAdvertising();
    }
};

class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    message = String(characteristic->getValue().c_str());
    printToScreen("Received:\n" + message);
  }
};

/*****************************
 * LORA Setup
 ****************************/
#include <RadioLib.h>

// SX1262 connections:
SX1262 radio = new Module(8, 14, 12, 13);

// flag to indicate that a packet was sent or received
volatile bool packetReceived = false;

#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  packetReceived = true;
}

void setup() {
  Serial.begin(9600);

  /* Setup Bluetooth */
  pinMode(0, INPUT_PULLUP);
  display.init();

  printToScreen("Starting BLE!");

  BLEDevice::init(DEVICE_NAME);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic->setValue("Init");

  pService->start();
  pServer->getAdvertising()->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(434.0, 500.0, 6, 5, RADIOLIB_SX127X_SYNC_WORD, 10, 6, 1.6);
  Serial.println("BEGAN");
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }

  // set the callback for reception
  radio.setDio1Action(setFlag);

  // start listening
  Serial.print(F("[SX1262] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }
}

void loop() {
  if (packetReceived) {
    packetReceived = false;
    String str;
    int state = radio.readData(str);

    if (state == RADIOLIB_ERR_NONE) {
      Serial.print("Full string: ");
      Serial.println(str);

      // If the received message starts with "ACK", it's likely an ACK from another device.
      // If you only have one receiver and multiple senders, you might never get here.
      // But if you do want to ignore such messages:
      if (str.startsWith("AC")) {
        Serial.println("Received what appears to be an ACK, ignoring...");
        radio.startReceive();
        return;
      }

      // Extract sender ID and the sent character
      if (str.length() < 2) {
        // Not a valid message, ignore
        radio.startReceive();
        return;
      }

      char receivedChar = str.charAt(0);
      char senderID = str.charAt(1);

      Serial.print("Received: ");
      Serial.print(receivedChar);
      Serial.print(" from sender: ");
      Serial.println(senderID);

      String receivedString = String(receivedChar) + ":" + String(senderID);
      pCharacteristic->setValue(receivedString.c_str());
      pCharacteristic->notify();

      // Send ACK back: "ACK" + senderID
      String ackMessage = "ACK";
      ackMessage += senderID;

      Serial.print("Sending ACK: ");
      Serial.println(ackMessage);
      
      state = radio.transmit(ackMessage);
      
      if (state == RADIOLIB_ERR_NONE) {
        Serial.println("ACK sent successfully!");
      } else {
        Serial.print("ACK failed, code ");
        Serial.println(state);
      }

      // Return to receive mode
      radio.startReceive();
    } else {
      Serial.print(F("Failed to read data, code "));
      Serial.println(state);
      // Resume listening
      radio.startReceive();
    }
  }
}
