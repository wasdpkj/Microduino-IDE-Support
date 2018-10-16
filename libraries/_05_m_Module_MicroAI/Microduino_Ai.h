#ifndef __MICRODUINO_AI_H__
#define __MICRODUINO_AI_H__

#include <Arduino.h>

#if defined (__AVR__)
#include <SoftwareSerial.h>
#endif

//#define DEBUG

#define MICRO_AI_VERSION  "1.0.1"

#define MODE_WIFI   0x01
#define MODE_BT     0x02
#define MODE_MQTT   0x03
#define MODE_BLE    0x04
#define MODE_TEST   0x05

#define CMD_PAUSE       0x01
#define CMD_STOP        0x02
#define CMD_FORWARD     0x03
#define CMD_BACKWARD    0x04


class MicroAi {
  public:
#if defined (__AVR__)
    MicroAi(HardwareSerial *ser);
    MicroAi(SoftwareSerial *ser);
#elif defined (ESP32)
    MicroAi(HardwareSerial *ser, int _rx = D2, int _tx = D3 );
#endif

    bool begin(uint8_t mode);
    bool isOn();
    char* getVersion();
    int32_t getFreeRam();
    bool reset();
    bool setMode(uint8_t mode);
    int8_t getMode();
    bool setWifi(char* ssid, char* pwd);
    int8_t getRssi();
    bool nameBT(char* name);
    bool cmdBT(uint8_t cmd);
    int8_t getBTstatus();
    bool playFile(char* file);
    bool playCmd(uint8_t cmd);
    char* getPlayFile();
    bool startRecord(char* file);
    bool stopRecord();
    int32_t getRecordTime();
    bool mqttSetServer(char* server);
    bool mqttConnect(char* id, char* usr, char* pwd);
    int8_t mqttGetStatus();
    bool mqttSetSubscrib(char* sub);
    bool mqttPublish(char* pub, char* msg);
    int32_t mqttQuery(char* res); 
    bool volumeUp();
    bool volumeDown();
    bool setVolume(uint8_t vol);
    int8_t getVolume();
    bool startRest();
    int32_t waitResult(char* res);
    uint32_t testQuery(); 

  private:
    HardwareSerial *aiHwSerial;
#if defined (__AVR__)
    SoftwareSerial *aiSwSerial;
#elif defined (ESP32)
    uint8_t pinRX = D2;
    uint8_t pinTX = D3;
#endif

    void common_init(void);
    int available();
    int read();
    void write(char* str);
    void write(uint8_t c);
    void clear();
    char* recvWait(uint32_t timeout, char* str); 
}; 


#endif
//
// END OF FILE
//