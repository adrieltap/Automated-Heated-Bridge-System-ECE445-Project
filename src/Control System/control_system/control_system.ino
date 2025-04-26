#include <OneWire.h>
#include <DallasTemperature.h>
#include "Simple-rain-sensor-easyC-SOLDERED.h"
#include <driver/adc.h>
#include "Callbacks.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/* ESP32 Pins */
#define AIR_TEMPERATURE_SENSOR_PIN 19
#define SURFACE_TEMPERATURE_SENSOR_PIN 17
#define RAIN_SENSOR_DIGITAL_PIN 34
#define RAIN_SENSOR_ANALOG_PIN 35
#define RAIN_SENSOR_DRY_VAL 1800
#define RAIN_SENSOR_LED_PIN 21
#define MOSFET_CONTROL_PIN 26


/* LOOP timer */
#define INTERVAL 3000


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

OneWire airTemperatureOneWire(AIR_TEMPERATURE_SENSOR_PIN);
OneWire surfaceTemperatureOneWire(SURFACE_TEMPERATURE_SENSOR_PIN);
DallasTemperature airTemperatureSensor(&airTemperatureOneWire);
DallasTemperature surfaceTemperatureSensor(&surfaceTemperatureOneWire);

SimpleRainSensor rainSensor(RAIN_SENSOR_ANALOG_PIN);
BLECharacteristic* pCharacteristic;
float airTemp, surfaceTemp, rainAmount;
bool heaterState = false;

void setup() {
  Serial.begin(115200);  // Start serial monitor

  /* Initialize the air and surface temperature sensors */
  airTemperatureSensor.begin();
  surfaceTemperatureSensor.begin(); 

  /* Initialize rain sensor, adc width and pin mode*/
  rainSensor.begin();
  rainSensor.setADCWidth(10);
  pinMode(RAIN_SENSOR_DIGITAL_PIN, INPUT_PULLUP);
  pinMode(AIR_TEMPERATURE_SENSOR_PIN, INPUT_PULLUP);
  pinMode(SURFACE_TEMPERATURE_SENSOR_PIN, INPUT_PULLUP);
  pinMode(MOSFET_CONTROL_PIN, OUTPUT);

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
  pCharacteristic->setCallbacks(new Callbacks());
  
  pService->start();
  BLEDevice::getAdvertising()->start();
  Serial.println("BLE Server started and advertising...");
}

void loop() {

  surfaceTemp = 0;
  airTemp = 0;
  rainAmount = 0;

  surfaceTemp = readSurfaceTemperatureSensor();
  rainAmount = readRainSensor();
  airTemp = readAirTemperatureSensor();

  /* check if temp sensor is valid value */
  if ((surfaceTemp <= -127.0) || (airTemp <= -127.0))
  {
    heaterState = false;
    updateSystemState();
    return;
  }

  /* heater mode is ON */
  if (heaterState)
  {
    if (surfaceTemp >= surfaceTempOverheatThreshold)
    {
      heaterState = false;
    }
    updateSystemState();
    return;
  }

  /* No water detected if true */
  if (rainAmount >= moistureThreshold) 
  {
    heaterState = false;
    updateSystemState();
    return;
  }


  if ((airTemp < airTempThreshold) || (surfaceTemp < surfaceTempThreshold))
  {
    heaterState = true;
  }
  else
  {
    heaterState = false;
  }
  
  updateSystemState();
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

  return rain;
}

void updateHeaterState()
{
  if (heaterState)
  {
    digitalWrite(MOSFET_CONTROL_PIN, HIGH);
  }
  else
  {
    digitalWrite(MOSFET_CONTROL_PIN, LOW);
  }
}

void sendValues() {

  char buffer[32];
  sprintf(buffer, "%.2f;%.2f;%.2f; %.2f", surfaceTemp, airTemp, rainAmount, (float)heaterState);
  String valueString = String(buffer);


  // Set and notify the new value.
  pCharacteristic->setValue(valueString.c_str());
  pCharacteristic->notify();
  
  Serial.println("Sent values...");
}

void updateSystemState()
{
  updateHeaterState();
  sendValues();
  Serial.println("");
  delay(INTERVAL);  // Wait 2 seconds before reading again
}
