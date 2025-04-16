#include <OneWire.h>
#include <DallasTemperature.h>
#include "Simple-rain-sensor-easyC-SOLDERED.h"
#include <driver/adc.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define AIR_TEMPERATURE_SENSOR_PIN 19
#define SURFACE_TEMPERATURE_SENSOR_PIN 5
#define RAIN_SENSOR_DIGITAL_PIN 18
#define RAIN_SENSOR_ANALOG_PIN 2
#define RAIN_SENSOR_DRY_VAL 1800
#define RAIN_SENSOR_LED_PIN 21

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

OneWire airTemperatureOneWire(AIR_TEMPERATURE_SENSOR_PIN);
OneWire surfaceTemperatureOneWire(SURFACE_TEMPERATURE_SENSOR_PIN);
DallasTemperature airTemperatureSensor(&airTemperatureOneWire);
DallasTemperature surfaceTemperatureSensor(&surfaceTemperatureOneWire);

SimpleRainSensor rainSensor(RAIN_SENSOR_ANALOG_PIN);
BLECharacteristic* pCharacteristic;
float airTemp, surTemp, rainAmount;

void setup() {
  Serial.begin(115200);  // Start serial monitor

  /* Initialize the air and surface temperature sensors */
  airTemperatureSensor.begin();
  surfaceTemperatureSensor.begin(); 

  /* Initialize rain sensor, adc width and pin mode*/
  rainSensor.begin();
  rainSensor.setADCWidth(10);
  pinMode(RAIN_SENSOR_DIGITAL_PIN, INPUT_PULLUP);

  /* Setup LED Pins */
  pinMode(RAIN_SENSOR_LED_PIN, OUTPUT);  // Set the LED pin as an output

  //---BLE Setup---//
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
  Serial.println("BLE Server started and advertising...");

}

void loop() {

  airTemp = readAirTemperatureSensor();
  surTemp = readSurfaceTemperatureSensor();
  rainAmount = readRainSensor();
  sendValues();

  delay(2000);  // Wait 2 seconds before reading again
}

float readAirTemperatureSensor()
{
  airTemperatureSensor.requestTemperatures();
  float temperatureC = airTemperatureSensor.getTempCByIndex(0);  // Get temperature in Celsius

  Serial.print("Air Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
  return temperatureC;
}

float readSurfaceTemperatureSensor()
{
  surfaceTemperatureSensor.requestTemperatures();
  float temperatureC = surfaceTemperatureSensor.getTempCByIndex(0);  // Get temperature in Celsius

  Serial.print("Surface Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
  return temperatureC;
}

float readRainSensor()
{
  Serial.print("Raw value of rain sensor: "); // Print information message
  float rain = rainSensor.getRawValue();
  Serial.println(rain); // Prints raw value of rain sensor

  if (rainSensor.getRawValue() <  RAIN_SENSOR_DRY_VAL)
  {
      Serial.println("Rain is detected");
      digitalWrite(RAIN_SENSOR_LED_PIN, HIGH);
  }
  else
  {
      Serial.println("Rain is NOT detected");
      digitalWrite(RAIN_SENSOR_LED_PIN, LOW);
  }
  Serial.println();
  return rain;
}

void sendValues() {

  char buffer[32];
  sprintf(buffer, "%.2f;%.2f;%.2f", surTemp, airTemp, rainAmount);
  String valueString = String(buffer);


  // Set and notify the new value.
  pCharacteristic->setValue(valueString.c_str());
  pCharacteristic->notify();
  
  Serial.print("Sent values...");
}
