#include <OneWire.h>
#include <DallasTemperature.h>
#include "Simple-rain-sensor-easyC-SOLDERED.h"
#include <driver/adc.h>

#define AIR_TEMPERATURE_SENSOR_PIN 19
#define SURFACE_TEMPERATURE_SENSOR_PIN 5
#define RAIN_SENSOR_DIGITAL_PIN 18
#define RAIN_SENSOR_ANALOG_PIN 2
#define RAIN_SENSOR_DRY_VAL 1800
#define RAIN_SENSOR_LED_PIN 21

OneWire airTemperatureOneWire(AIR_TEMPERATURE_SENSOR_PIN);
OneWire surfaceTemperatureOneWire(SURFACE_TEMPERATURE_SENSOR_PIN);
DallasTemperature airTemperatureSensor(&airTemperatureOneWire);
DallasTemperature surfaceTemperatureSensor(&surfaceTemperatureOneWire);

SimpleRainSensor rainSensor(RAIN_SENSOR_ANALOG_PIN);

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

}

void loop() {

  readAirTemperatureSensor();
  readSurfaceTemperatureSensor();
  readRainSensor();

  delay(2000);  // Wait 2 seconds before reading again
}

void readAirTemperatureSensor()
{
  airTemperatureSensor.requestTemperatures();
  float temperatureC = airTemperatureSensor.getTempCByIndex(0);  // Get temperature in Celsius

  Serial.print("Air Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
}

void readSurfaceTemperatureSensor()
{
  surfaceTemperatureSensor.requestTemperatures();
  float temperatureC = surfaceTemperatureSensor.getTempCByIndex(0);  // Get temperature in Celsius

  Serial.print("Surface Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
}

void readRainSensor()
{
  Serial.print("Raw value of rain sensor: "); // Print information message
  Serial.println(rainSensor.getRawValue()); // Prints raw value of rain sensor

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
}
