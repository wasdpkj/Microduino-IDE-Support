#ifndef _SHIELDNRF_H_
#define _SHIELDNRF_H_

#include <Arduino.h>

#define ShieldNrf_ADDRESS   0x014

#define NRFMODE 0
#define AVRMODE 1

#define I2C_R_SD_VERSION    0
#define I2C_R_HW_VERSION    1
#define I2C_R_FW_VERSION    2
#define I2C_R_Priority      3
#define I2C_R_8_MIC         4
#define I2C_R_8_BATTERY     5
#define I2C_R_KEY           6
#define Comm_OverTime       7

#define I2C_W_LED_R         0x20
#define I2C_W_LED_G         0x21
#define I2C_W_LED_B         0x22
#define I2C_W_BEEP_FH       0x26
#define I2C_W_MODE          0x30

#define PRESSED  1
#define RELEASED 2

#define KEY_X  1
#define KEY_Y  2


class ironmanShieldNrf {
  public:
    ironmanShieldNrf();
    boolean begin(uint8_t i2cAddr = ShieldNrf_ADDRESS);
    void write8(uint8_t dataaddr, uint8_t data);
    void writedata(uint8_t addr, uint8_t *_data, uint8_t _len);
    uint8_t getReg8(uint8_t _reg);
    uint8_t sdVersion();
    uint8_t hardwareVersion();
    uint8_t firmwareVersion();
    uint8_t priorityVal();
    uint8_t micVal();
    uint8_t batteryVal();
    uint8_t keyVal();
    uint8_t overtimeVal();
    void priorityEN(uint8_t _mode);
    void setColorLED(uint8_t colorR, uint8_t colorG, uint8_t colorB);
    void tone(uint16_t freq);
    void tone(uint16_t freq, uint16_t timer);
    boolean readKeyX();
    boolean readKeyY();
    void readKey();
    boolean shieldbuttonSta(uint8_t key, uint8_t mode);

  private:
    
    uint8_t _i2cAddr;
    boolean iicsta;
    uint8_t rgbdata[3];
    uint8_t tonefretime[4];
    boolean twobuttonsta[4];
    boolean ironmankey[2], ironmanvalue[2];
};

#endif
