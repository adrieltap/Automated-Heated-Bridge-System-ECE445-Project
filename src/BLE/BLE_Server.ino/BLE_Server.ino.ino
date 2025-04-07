#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Unique IDs for service and characteristic.
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic* pCharacteristic;

void setup() {
  Serial.begin(115200);
  
  // Initialize BLE with the name "ESP32_BLE"
  BLEDevice::init("ESP32_BLE");
  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(SERVICE_UUID);
  
  // Create a characteristic with read, write, and notify properties.
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->addDescriptor(new BLE2902());
  
  pService->start();
  BLEDevice::getAdvertising()->start();
  
  // Initialize the random number generator seed.
  randomSeed(analogRead(0));
  Serial.println("BLE Server started and advertising...");
}

void loop() {
  // Generate a random integer (for testing purposes)
  int randomValue = random(0, 100);
  String valueString = String(randomValue);
  
  // Set and notify the new value.
  pCharacteristic->setValue(valueString.c_str());
  pCharacteristic->notify();
  
  Serial.print("Sent value: ");
  Serial.println(valueString);
  
  delay(5000);
}
