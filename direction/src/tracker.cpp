#include <NimBLEDevice.h>

#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define GPS

#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define TX_CHAR_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

static void notifyCB(
  NimBLERemoteCharacteristic* c,
  uint8_t* data,
  size_t len,
  bool isNotify) {

  String msg = String((char*)data);
  Serial.println("GPS: " + msg);
}

#ifdef GPS
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);
#endif


NimBLEClient* client;

void setup() {

  Serial.begin(115200);

#ifdef GPS
  gpsSerial.begin(9600, SERIAL_8N1, 15, 2);
#endif

  NimBLEDevice::init("");

  NimBLEScan* scan = NimBLEDevice::getScan();
  scan->setActiveScan(true);

  NimBLEScanResults results = scan->start(5);

  for(int i=0;i<results.getCount();i++){

    NimBLEAdvertisedDevice dev = results.getDevice(i);

    if(dev.getName() == "GPS-Beacon"){

      client = NimBLEDevice::createClient();

      client->connect(&dev);

      NimBLERemoteService* svc =
        client->getService(SERVICE_UUID);

      NimBLERemoteCharacteristic* chr =
        svc->getCharacteristic(TX_CHAR_UUID);

      chr->subscribe(true, notifyCB);

      Serial.println("Connected to beacon!");
    }
  }
}

void loop() {
#ifdef GPS
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {

    char data[64];

    snprintf(data, sizeof(data), "%f,%f",
      gps.location.lat(),
      gps.location.lng());


    Serial.println(data);
  }
#endif
  if (client != NULL) {
    Serial.println(client->getRssi());
  }
  delay(50);
}
