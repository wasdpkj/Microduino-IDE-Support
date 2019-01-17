#ifndef _ironmanIRremote_H_
#define _ironmanIRremote_H_

#define SENSOR_ID          0x08
#define IRremote_ADDRESS   SENSOR_ID
#define IRremoteID         0
#define IRremotePOWER      2
#define IRremoteHANDWARE   3
#define IRremoteSOFTWARE   4
#define IRremoteERR        5
#define SENSORREST         6
#define NRFENABLE          7
#define SENSORPORT         8
#define SETAVRADDR         9
#define SETPOWER           10

//NC   12-16
#define ADDR8_SEND_MODE              17
#define ADDR40_IRSENDING_NECDATA     18
#define ADDR40_IRRECED_NECDATA       23
#define ADDR16U8_IRSENDING_SONYDATA  28
#define ADDR16D16_IRSENDING_SONYDATA 14

#define NEC 1
#define SONY 2

#define NEC_BITS 32
#define SONY_BITS 12

#include "Arduino.h"

class ironmanIRremote {

  public:
    ironmanIRremote ();
    bool begin(uint8_t i2cAddr = IRremote_ADDRESS);
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
    boolean irrecvAvailable();
    uint32_t irrecvValue();
    void sendNEC(uint32_t data);
    void sendSony(uint16_t data);

  private:

    uint32_t getReg40_7bitto32(uint8_t _reg);
    void setReg32to40_7bit(int32_t _data32);

    boolean iicsta;
    uint8_t _i2cAddr;
    uint8_t length;
    uint8_t cmd;
    uint32_t irrecdata;
    uint32_t irrecdataCache;
    uint8_t irsenddata[5];

};

#endif