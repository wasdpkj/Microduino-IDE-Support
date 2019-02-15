#ifndef _ironmanColorLED_H_
#define _ironmanColorLED_H_
#include "Arduino.h"

#define SENSOR_ID          0x06
#define ColorLED_ADDRESS   0x06
#define ColorLEDID         0
#define ColorLEDPOWER      2
#define ColorLEDHANDWARE   3
#define ColorLEDSOFTWARE   4
#define ColorLEDERR        5
#define SENSORREST         6
#define NRFENABLE          7
#define SENSORPORT         8
#define SETAVRADDR         9
#define SETPOWER           10
#define ColorLEDNUM        17

class ironmanColorLED {

  public:
    ironmanColorLED();
    bool begin(uint8_t i2cAddr = ColorLED_ADDRESS);
    boolean available();
    uint8_t getReg8(uint8_t _reg);
    uint16_t getReg7to8(uint8_t _reg);
    uint16_t getID();
    uint8_t getPower();
    uint8_t getHandware();
    uint8_t getSoftware();
    uint8_t getErr();
    void write8(uint8_t dataaddr, uint8_t data);
    void writecmddata(uint8_t addr, uint8_t cmd, uint8_t *_data, uint8_t _len);
    void Rest();
    void setNrfEN(boolean _en);
    void setPort(uint8_t port);
    void setAddr(uint8_t addr);
    void setPower(uint8_t _power);
    void setBrightness(uint8_t _bright = 255);
    void setAllLED(uint8_t colorR, uint8_t colorG, uint8_t colorB);
    void setAllLED(uint32_t _color);
    void setPixelColor(uint8_t n, uint8_t colorR, uint8_t colorG, uint8_t colorB);
    void setPixelColor(uint8_t n, uint32_t _color);
    void setOneLED(uint8_t n, uint8_t colorR, uint8_t colorG, uint8_t colorB);
    void setOneLED(uint8_t n, uint32_t _color);
    void clear();

  private:
    boolean iicsta;
    uint8_t _i2cAddr;
    uint8_t length;
    uint8_t cmd;
    uint8_t bright = 255;
    uint8_t brightrgb[3];
    uint8_t rgbdata[6];


};

#endif
