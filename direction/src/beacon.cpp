#include <NimBLEDevice.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define TX_CHAR_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

NimBLECharacteristic *pTX;

void setup() {

  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 15, 2);

  NimBLEDevice::init("GPS-Beacon");

  NimBLEServer *server = NimBLEDevice::createServer();
  NimBLEService *service = server->createService(SERVICE_UUID);

  pTX = service->createCharacteristic(
        TX_CHAR_UUID,
        NIMBLE_PROPERTY::NOTIFY
  );

  service->start();
  server->getAdvertising()->start();

  Serial.println("Beacon advertising...");
}

void loop() {

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {

    char data[64];

    snprintf(data, sizeof(data), "%f,%f",
      gps.location.lat(),
      gps.location.lng());

    pTX->setValue(data);
    pTX->notify();

    Serial.println(data);
  } else {
    //pTX->notify();
  }

  delay(10);
}
