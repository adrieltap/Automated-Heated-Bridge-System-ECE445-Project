#include "Callbacks.h"
#include <Arduino.h>

float airTempThreshold = -127.0;
float surfaceTempThreshold = -127.0;
float surfaceTempOverheatThreshold = -127.0;
int moistureThreshold = 4096;

void Callbacks::onWrite(BLECharacteristic *pCharacteristic) {
  String value = pCharacteristic->getValue();
  if (value.length() > 0) {
    Serial.print("Received via BLE: ");
    Serial.println(value);

    if (value.startsWith("air=")) {
      airTempThreshold = value.substring(4).toFloat();
      Serial.print("Updated airTempThreshold: ");
      Serial.println(airTempThreshold);
    }
    else if (value.startsWith("surface=")) {
      surfaceTempThreshold = value.substring(8).toFloat();
      Serial.print("Updated surfaceTempThreshold: ");
      Serial.println(surfaceTempThreshold);
    }
    else if (value.startsWith("overheat=")) {
      surfaceTempOverheatThreshold = value.substring(9).toFloat();
      Serial.print("Updated surfaceTempOverheatThreshold: ");
      Serial.println(surfaceTempOverheatThreshold);
    }
    else if (value.startsWith("moisture=")) {
      moistureThreshold = value.substring(9).toInt();
      Serial.print("Updated moistureThreshold: ");
      Serial.println(moistureThreshold);
    }
  }
}
