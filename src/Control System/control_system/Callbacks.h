#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

extern float airTempThreshold;
extern float surfaceTempThreshold;
extern float surfaceTempOverheatThreshold;
extern int moistureThreshold;

class Callbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic);
};

#endif
