#define BLESERIAL_USE_NIMBLE true

#include <HardwareSerial.h>
#include <NimBLEDevice.h>
#include <nimble/nimble/host/include/host/ble_gap.h>

// Define the BLE Serial Service and Characteristic UUIDs
// 00
// #define SERVICE_UUID           "6e400001-b5a3-f393-e0a9-e50e24dcca9e"  // Custom service UUID (similar to SPP)
// #define RX_CHAR_UUID           "6e400002-b5a3-f393-e0a9-e50e24dcca9e"  // Characteristic for receiving data
// #define TX_CHAR_UUID           "6e400003-b5a3-f393-e0a9-e50e24dcca9e"  // Characteristic for transmitting data

// 01
#define SERVICE_UUID           "6e400001-b5a3-f393-e0a9-e50e24dcca9a"  // Custom service UUID (similar to SPP)
#define RX_CHAR_UUID           "6e400002-b5a3-f393-e0a9-e50e24dcca9a"  // Characteristic for receiving data
#define TX_CHAR_UUID           "6e400003-b5a3-f393-e0a9-e50e24dcca9a"  // Characteristic for transmitting data

const String device_name = "BLE-DEV-01";  // Arduino String object

NimBLECharacteristic *pRXCharacteristic;
NimBLECharacteristic *default_pRXCharacteristic;
NimBLECharacteristic* pTXCharacteristic;
NimBLEServer* pServer;
NimBLEService* pService;

String lastReceivedData = "";  // Variable to track the last received data

void setup() {
    Serial.begin(9600);
    
    // Initialize NimBLE with the device name
    NimBLEDevice::init(device_name.c_str());
    
    // Create the BLE server
    pServer = NimBLEDevice::createServer();

    // Create the BLE Serial service
    pService = pServer->createService(SERVICE_UUID);

    // Create the RX characteristic (for receiving data)
    pRXCharacteristic = pService->createCharacteristic(
        RX_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ
    );
    default_pRXCharacteristic = pRXCharacteristic;

    // Create the TX characteristic (for transmitting data)
    pTXCharacteristic = pService->createCharacteristic(
        TX_CHAR_UUID,
        NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ
    );

    // Start the service
    pService->start();

    // Start advertising the BLE server
    NimBLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    // Set the power level (optional)
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
}

void loop() {
    // Check if there's data available from BLE (via RX characteristic)
    if (pRXCharacteristic->getValue().length() > 0) {
      String receivedData = pRXCharacteristic->getValue().c_str();
    //   pRXCharacteristic->getHandle()
        
        // Process the received data only if it's different from the last received data
        if (receivedData != lastReceivedData) {
          Serial.println("Received BLE data: " + receivedData);
          
            
            // Update the last received data to the new value
            lastReceivedData = receivedData;
        }
    }

    // Forward data from Serial to BLE (TX characteristic)
    if (Serial.available()) {
        char data = Serial.read();
        pTXCharacteristic->setValue(String(data));  // Update TX characteristic with the new data
        pTXCharacteristic->notify();  // Notify connected clients with new data
    }

    std::vector<short unsigned int> devices =
        NimBLEDevice::getServer()->getPeerDevices();

    // Serial.println("Connected devices:");

    for (uint16_t dev_idx = 0; dev_idx < devices.size(); dev_idx++) {
      int8_t rssi;
      ble_gap_conn_rssi(NimBLEDevice::getServer()->getPeerInfo(dev_idx).getConnHandle(),  &rssi);
      Serial.println(rssi);
      
    //   NimBLEClient::getRssi();
    }

    delay(10);  // Add a small delay to prevent flooding the loop
}

class MyServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer) {
    Serial.println("Device Connected.");
  }

  void onDisconnect(NimBLEServer* pServer) {
    Serial.println("Device Disconnected.");
  }
};