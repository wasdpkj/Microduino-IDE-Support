#if defined (ESP32)

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <mBattery_Pmu.h>
MbaPmu pmu;

//#define Serial_DEBUG
#define NORMAL_BAUD 115200

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define UUID_SERVICE_CHAR       BLEUUID((uint16_t)0xFFF0)  // UART service UUID
#define CHAR_UUID               BLEUUID((uint16_t)0xFFF6)  //PROPERTY_WRITE_NR PROPERTY_NOTIFY
#define BAUD_UUID               BLEUUID((uint16_t)0xFFF2)  //PROPERTY_WRITE_NR
#define RESET_UUID              BLEUUID((uint16_t)0xFFF1)  //PROPERTY_WRITE_NR

#define UUID_SERVICE_BATTERY    BLEUUID((uint16_t)0x180F)  // battery service UUID
#define BATTERY_UUID            BLEUUID((uint16_t)0x2A19)  //PROPERTY_READ PROPERTY_NOTIFY

#define UUID_SERVICE_INFO       BLEUUID((uint16_t)0x180A)  // info service UUID
#define MANUFACTURER_UUID       BLEUUID((uint16_t)0x2A29)  //PROPERTY_READ
#define MODELNUMBER_UUID        BLEUUID((uint16_t)0x2A24)  //PROPERTY_READ
#define HARDVERSION_UUID        BLEUUID((uint16_t)0x2A27)  //PROPERTY_READ
#define FIRMVERSION_UUID        BLEUUID((uint16_t)0x2A26)  //PROPERTY_READ

#define DATA_ROBOT_LEN 16
#define DATA_ALL_LEN (DATA_ROBOT_LEN + 4)

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic, *pBaudacteristic;
BLECharacteristic *batteryCharacteristic;
BLECharacteristic *nameCharacteristic, *modelCharacteristic, *hardCharacteristic, *firmCharacteristic;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

String bleName = "mCookieFFFF";
uint16_t channel[8];

uint8_t dataRX[DATA_ALL_LEN];
uint8_t dataLen = 0;
uint8_t dataStep = 0;
uint32_t dataTime = 0;

uint8_t getChecksum(uint8_t _cmd, uint8_t *_data, uint8_t _len) {
  uint8_t checksum = 0;
  checksum ^= _cmd;
  checksum ^= _len;
  for (uint8_t i = 0; i < _len; i++)
    checksum ^= _data[i];
  return checksum;
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


class BatteryCallbacks: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *batteryCharacteristic) {
      uint8_t batteryLevel[1];        //0-100
      if (pmu.getVersion() != 0) {
        if (pmu.isLow()) {
          batteryLevel[0] = 0;
        }
        else {
          uint16_t _batteryVal = pmu.getVoltage(5.0) * 100;
          _batteryVal = constrain(_batteryVal, 740, 840);
          batteryLevel[0] = map(_batteryVal, 740, 840, 0, 100);
        }
      }
      else {
        batteryLevel[0] = 0;
      }
      batteryCharacteristic->setValue(batteryLevel, 1);
    }
};

class BaudCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pBaudacteristic) {
      std::string _rxValueString = pBaudacteristic->getValue();
      uint8_t _rxLength = _rxValueString.length();
      uint8_t *_rxValue = pBaudacteristic->getData();

      if (_rxLength > 0) {  //高位在前
#ifdef Serial_DEBUG
        Serial.print(F("pBaudacteristic RAW Data: "));
        for (int i = 0; i < _rxLength; i++) {
          Serial.print(F(" 0x"));
          Serial.print(_rxValue[i], HEX);
        }
        Serial.println();
#endif
        uint16_t _baud = 0x0000;
        if (_rxLength == 2) {
          _baud = (uint16_t)(_rxValue[1] << 8) | (uint16_t)(_rxValue[0]);
        }
        else {
          Serial.print(F("ERROR-rx Length:"));
          Serial.println(_rxLength);
          return;
        }
        if ((_baud * 100) > NORMAL_BAUD) {
          Serial.print(F("ERROR-baud:"));
          Serial.println(_baud);
          Serial.updateBaudRate(NORMAL_BAUD);
          return;
        }
        Serial.updateBaudRate(_baud * 100);
        delay(10);
        Serial.print(F("Set baud:"));
        Serial.println(_baud * 100);
      } //end if (rxLength
    }
};


class CharCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValueString = pCharacteristic->getValue();
      uint8_t rxLength = rxValueString.length();
      uint8_t *rxValue = pCharacteristic->getData();

      if (rxLength > 0) {
        if (rxValue[0] == 0xAA && rxValue[1] == 0xBB && rxValue[2] == 0xC8) {
          dataStep = 0;
          memset(dataRX, 0, DATA_ALL_LEN);
          memcpy(dataRX, rxValue, rxLength);              //收头数据
          dataLen = rxLength;
        }
        else {
          if (dataLen + rxLength > DATA_ALL_LEN) {    //收到数据不准超过最大字节数
            return;
          }
          dataStep ++;
          memcpy(dataRX + dataLen, rxValue, rxLength);    //收尾数据
          dataLen += rxLength;                            //总数据字节数
        }

#ifdef Serial_DEBUG
        Serial.print("\t rxLength:");
        Serial.print(rxLength);
        Serial.print(",at step:");
        Serial.println(dataStep);
#endif
        if (dataLen == DATA_ALL_LEN) {
#ifdef Serial_DEBUG
          Serial.print(F("raw Data["));
          Serial.print(dataLen);
          Serial.print(F("byte]:"));
          for (int i = 0; i < dataLen; i++) {
            Serial.print(F(" 0x"));
            Serial.print(dataRX[i], HEX);
          }
          Serial.println();
#endif

          uint8_t _dataRobotLen = dataLen - 4;
          if (getChecksum(dataRX[2], dataRX + 3, _dataRobotLen) == dataRX[dataLen - 1]) {
            memset((uint8_t *)channel, 0, _dataRobotLen);
            memcpy((uint8_t *)channel, dataRX + 3, _dataRobotLen);
#ifdef Serial_DEBUG
            Serial.print("\t Robot data:");
            for (int i = 0; i < _dataRobotLen / 2; i++) {
              Serial.print(" ");
              Serial.print(channel[i], DEC);
            }
            Serial.println();
#endif
            dataTime = millis();
          } //end if (getChecksum
#ifdef Serial_DEBUG
          Serial.println();
#endif
        } //end if (dataLen

      } //end if (rxLength

    }
};


bool getChannel(uint16_t *_data) {
  bool _sta = false;
  //安全模式
  if (millis() - dataTime > 500) {
    _sta = false;
    for (int i = 0; i < 4; i++) {
      channel[i] =  1500;
    }
    for (int i = 4; i < 8; i++) {
      channel[i] =  1000;
    }
  }
  else {
    _sta = true;
  }
  for (int a = 0; a < 8; a++) {
    _data[a] = channel[a];
  }
  return _sta;
}


bool getBLEConnection() {
  return deviceConnected;
}


void refreshBLEConnection() {
  delay(5); //给BLE服务执行时间
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    for (int a = 0; a < 2; a++) {
      tone(PIN_BUZZER, NOTE_D6);
      delay(100);
      noTone(PIN_BUZZER);
      delay(100);
    }
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    tone(PIN_BUZZER, NOTE_D6);
    delay(500);
    noTone(PIN_BUZZER);
  }
}


//设备名称
void initBLEName() {
  uint16_t _macAddr16 = ESP.getEfuseMac();
  uint8_t _macAddr8[2];
  _macAddr8[0] = (_macAddr16 >> 8) & 0x00FF;
  _macAddr8[1] = _macAddr16 & 0x00FF;
  String bleMac = "";
  if (_macAddr8[0] < 0x10) bleMac += "0";
  bleMac += String(_macAddr8[0], HEX);
  if (_macAddr8[1] < 0x10) bleMac += "0";
  bleMac += String(_macAddr8[1], HEX);
  bleMac.toUpperCase();
  bleName = "mCookie" + bleMac;
  BLEDevice::init(bleName.c_str());
}

String getBLEName() {
  return bleName;
}

// Create the BLE Server
void initBLEServer() {
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
}


void setupCharService() {
  BLEService *pService = pServer->createService(UUID_SERVICE_CHAR);

  //pChar
  pCharacteristic = pService->createCharacteristic(
                      CHAR_UUID,
                      BLECharacteristic::PROPERTY_WRITE_NR  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->setCallbacks(new CharCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  //pBaud
  pBaudacteristic = pService->createCharacteristic(
                      BAUD_UUID,
                      BLECharacteristic::PROPERTY_WRITE_NR
                    );
  pBaudacteristic->setCallbacks(new BaudCallbacks());
  //  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();
}


void setupBatteryService() {
  //******PMU的初始化
  pmu.begin();

  BLEService *batteryService = pServer->createService(UUID_SERVICE_BATTERY);
  batteryCharacteristic = batteryService->createCharacteristic(
                            BATTERY_UUID,
                            BLECharacteristic::PROPERTY_READ   |
                            BLECharacteristic::PROPERTY_NOTIFY
                          );
  batteryCharacteristic->setCallbacks(new BatteryCallbacks());
  batteryCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  batteryService->start();
}


void setupInfoService() {
  BLEService *infoService = pServer->createService(UUID_SERVICE_INFO);

  //1-----------------------------------------------------------------------
  nameCharacteristic = infoService->createCharacteristic(MANUFACTURER_UUID, BLECharacteristic::PROPERTY_READ);
  nameCharacteristic->setValue("Microduino");
  //2-----------------------------------------------------------------------
  modelCharacteristic = infoService->createCharacteristic(MODELNUMBER_UUID, BLECharacteristic::PROPERTY_READ);
  modelCharacteristic->setValue("ESP32");
  //3-----------------------------------------------------------------------
  hardCharacteristic = infoService->createCharacteristic(HARDVERSION_UUID, BLECharacteristic::PROPERTY_READ);
  hardCharacteristic->setValue("2.5");
  //4-----------------------------------------------------------------------
  firmCharacteristic = infoService->createCharacteristic(FIRMVERSION_UUID, BLECharacteristic::PROPERTY_READ);
  firmCharacteristic->setValue("0.3.2.01");

  // Start the service
  infoService->start();
}


// Start advertising
void startBLEAdvertising() {
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(UUID_SERVICE_CHAR);
  pAdvertising->addServiceUUID(UUID_SERVICE_BATTERY);
  pAdvertising->addServiceUUID(UUID_SERVICE_INFO);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

#endif
