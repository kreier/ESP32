/**
 *  Recieve the BLE signal from an iOS device, running GoBLE
 * 
 *  App: https://github.com/CainZ/GoBle
 *  Should work with GoBLE_Test, but the library is not accepted
 * 
 */

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID    "0000dfb0-0000-1000-8000-00805f9b34fb" // service UUID for GoBLE App
#define SERIALPORT_UUID "0000dfb1-0000-1000-8000-00805f9b34fb" // is SerialPortID

BLEServer *pServer = NULL;
BLECharacteristic * SerialCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

int joystickX, joystickY;
int buttonState[7];        // skip zero and confusion, button 3 = buttonState[3]

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i], HEX);
          Serial.print(" ");
        }
        Serial.println(" ");
        // store values in buttonState[]
        for (int i = 0; i < 7; i++) buttonState[i] = 0;
        for (int i = 0; i < rxValue[3]; i++) buttonState[rxValue[i + 5]] = 1;
        // store values in joystickX and joystickY from 0 to 255
        int i = rxValue[3];
        joystickX = rxValue[i + 5];
        joystickY = rxValue[i + 6];
        // print results:
        Serial.print("Buttons [1-6]: ");
        for (int i = 1; i < 7; i++) {
          Serial.print(buttonState[i]);
          Serial.print(" ");
        }
        Serial.print("  joystickX: ");
        Serial.print(joystickX);
        Serial.print("  joystickY: ");
        Serial.print(joystickY);
        Serial.println(" ");
      }
   }
};

void setup() {
  Serial.begin(115200);
  // Create the BLE Device  
  BLEDevice::init("T-200 ESP32");
  // Create the BLE Server - to advertise the service, that the iPhone App can connect to
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic * SerialCharacteristic = pService->createCharacteristic(
                    SERIALPORT_UUID,
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_WRITE  |
                    BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_WRITE_NR  
                    );
  SerialCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");  
}

void loop() {
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}
