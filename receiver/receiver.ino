/*****************************
 * Bluetooth Setup
 ****************************/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include "HT_SSD1306Wire.h"

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
 * Screen Setup
 ****************************/
static SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

void printToScreen(String s) {
  display.clear();
  display.drawString(0, 0, s);
  display.display();
}


/*****************************
 * LORA Setup
 ****************************/
#include <RadioLib.h>

// SX1262 has the following connections:
SX1262 radio = new Module(8, 14, 12, 13);

// flag to indicate that a packet was sent or received
volatile bool packetReceived = false;

// this function is called when a complete packet
// is transmitted or received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
  // we sent or received a packet, set the flag
  packetReceived = true;
}

void setup()
{
  Serial.begin(9600);

  /* Setup Bluetooth*/
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

  // set the function that will be called
  // when new packet is received
  radio.setDio1Action(setFlag);

  // start listening for LoRa packets on this node
  Serial.print(F("[SX1262] Starting to listen ... "));
  state = radio.startReceive();
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

void loop()
{
  // check if the previous operation finished
  if (packetReceived)
  {

    packetReceived = false;
    // the previous operation was reception
    // print data and send another packet
    String str;
    int state = radio.readData(str);

    if (state == RADIOLIB_ERR_NONE)
    {
      // packet was successfully received
      char senderID = str.charAt(1);
      char receivedChar = str.charAt(0);
      
      Serial.print("Received: ");
      Serial.print(receivedChar);
      Serial.print(" from board ");
      Serial.println(senderID, HEX);

      String receivedString = String(receivedChar) + ":" + String(senderID);
      pCharacteristic->setValue(receivedString.c_str());
      pCharacteristic->notify();
    }
  }
}