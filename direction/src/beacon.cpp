#include <NimBLEDevice.h>

NimBLEServer *pServer = NULL;
NimBLECharacteristic *pCharacteristic = NULL;
NimBLEAdvertising *pAdvertising = NULL;

NimBLEClient *client00 = NULL;
NimBLEClient *client01 = NULL;

const std::string master00 = "44:1d:64:f8:ed:0e"; // Address of Master 00
const std::string master01 = "44:1d:64:f8:df:66"; // Address of Master 01

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  NimBLEDevice::init("MyPeripheral");

  // Create BLE server
  pServer = NimBLEDevice::createServer();

  // Add service
  NimBLEService *pService =
      pServer->createService("12345678-1234-1234-1234-1234567890ab");

  // Create a characteristic
  pCharacteristic = pService->createCharacteristic(
      "abcdef01-1234-1234-1234-1234567890ab",
      NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);

  // Start the service
  pService->start();

  // Start advertising to allow the master devices to connect
  pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  Serial.println("BLE Peripheral Started and Advertising...");

  // Initialize clients for both master devices
  client00 = NimBLEDevice::createClient();
  client01 = NimBLEDevice::createClient();

  // Try connecting directly to both master devices
  Serial.println("Connecting to Master 00...");
  if (client00->connect(master00)) {
    Serial.println("Connected to Master 00!");
  } else {
    Serial.println("Failed to connect to Master 00");
  }

  Serial.println("Connecting to Master 01...");
  if (client01->connect(master01)) {
    Serial.println("Connected to Master 01!");
  } else {
    Serial.println("Failed to connect to Master 01");
  }
}

void loop() {
  // Check if both clients are connected
  if (client00->isConnected() && client01->isConnected()) {
    // Send data to the first master
    String data00 = "Hello Master 0";
    pCharacteristic->setValue(data00.c_str());
    pCharacteristic->notify(); // Send notification to the first master

    // Send data to the second master
    String data01 = "Hello Master 1";
    pCharacteristic->setValue(data01.c_str());
    pCharacteristic->notify(); // Send notification to the second master

    Serial.println("Data sent to both masters.");
  } else {
    Serial.println("Waiting for connections...");
  }

  delay(10); // Delay before next loop
}