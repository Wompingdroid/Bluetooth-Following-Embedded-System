#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#include <BLEBeacon.h>

#define BEACON_UUID        "e2c56db5-dffb-48d2-b060-d0f5a71096e0"
#define BEACON_MAJOR       1
#define BEACON_MINOR       1
#define BEACON_TX_POWER   -65  // Measured power (RSSI at 1 meter)

BLEAdvertising *pAdvertising;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ESP32 iBeacon...");

  BLEDevice::init("");

  //BLEServer *pServer = BLEDevice::createServer(); // Creates a generic bluetooth server

  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0xEFBE); // Unused manufacturer ID. This allows for easier filtering. This will set the manufacturere to BEEF.
  Serial.printf("0x%X", BLEUUID(BEACON_UUID));
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(BEACON_MAJOR);
  oBeacon.setMinor(BEACON_MINOR);
  oBeacon.setSignalPower(BEACON_TX_POWER);

  BLEAdvertisementData oAdvertisementData;
  //oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED
  oAdvertisementData.setManufacturerData(oBeacon.getData());

  pAdvertising = BLEDevice::getAdvertising();
  //pAdvertising->addTxPower();
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMaxPreferred(0x12);
  //pAdvertising->setMinInterval(0x20);
  //pAdvertising->setMaxInterval(0x20);

  BLEDevice::startAdvertising();

  Serial.println("iBeacon advertising started");
}

void loop() {
  delay(10);
}
