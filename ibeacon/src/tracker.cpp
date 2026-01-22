#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>

// Beacon identifiers (must match transmitter)
#define BEACON_UUID  "e2c56db5-dffb-48d2-b060-d0f5a71096e0"
#define BEACON_MAJOR 1
#define BEACON_MINOR 1
#define MANUFACTURER_ID 0xBEEF  // Same as beacon (BEEF)

BLEScan* pBLEScan;

// Sampling variables
#define SAMPLE_PERIOD

uint32_t lastPrint = 0;
int rssi = 0;
float ema = 0.0f;
const float smoothing_factor = 2.0f / (SAMPLE_PERIOD + 1.0f); // Alpha = 2/(N+1)


float update_ema(float new_value, float previous_ema, float smoothing_factor) {
    // The formula: EMA_current = (New_value * alpha) + (Previous_EMA * (1 - alpha))
    float current_ema = (new_value * smoothing_factor) + (previous_ema * (1.0f - smoothing_factor));
    return current_ema;
}


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {

    // Must have manufacturer data (iBeacon uses this)
    if (!advertisedDevice.haveManufacturerData()){
      //Serial.println("Device missing manufacturer data");
      return;
    }


    std::string manufacturerData = advertisedDevice.getManufacturerData();
    if (manufacturerData.length() >= 25) {
      std::string beaconData = manufacturerData.substr(0, 25); // only first 25 bytes
      BLEBeacon beacon;
      beacon.setData(beaconData);

      // Now filter by manufacturer ID, UUID, Major, Minor as before
      if (beacon.getManufacturerId() != MANUFACTURER_ID){
        //Serial.println("Failed to match the manufacturer ID");
        return;
      }

      Serial.printf("0x%X\n", beacon.getManufacturerId());
      Serial.printf("0x%X, compared with 0x%X\n", beacon.getProximityUUID(), BLEUUID(BEACON_UUID));
      //if (beacon.getProximityUUID().toString() != BLEUUID(BEACON_UUID).toString()){
      //  Serial.println("proximity id does not match");
      //  return;
      //}

      Serial.printf("0x%x\n", beacon.getMajor());
      //if (beacon.getMajor() != BEACON_MAJOR) return;

      Serial.printf("0x%x\n", beacon.getMinor());
      //if (beacon.getMinor() != BEACON_MINOR) return;

      rssi = advertisedDevice.getRSSI();
      ema = update_ema(rssi, ema, smoothing_factor);

      Serial.print("Beacon RSSI: ");
      Serial.print(rssi);
      Serial.print(", ");
      Serial.print(ema);
      Serial.println(" dBm");
    } else {
      //Serial.println("data length too short!");
    }

  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting BLE scanner...");

  BLEDevice::init("tracker");

  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);   // Request scan responses (more accurate RSSI)
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  Serial.println("Scanning...");
  pBLEScan->start(0, nullptr, false);
}

void loop() {
  // Scan for 1 second, then repeat
  // Scan for 1 second, then repeat
  if (millis() - lastPrint >= 100) { // 10 Hz output
    Serial.println(rssi);
    Serial.println(ema);
    lastPrint = millis();
  }
  //pBLEScan->start(1, false);
  //pBLEScan->clearResults();
  //Serial.println("Scanning...");

}
