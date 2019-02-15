#ifndef _ironmanUltrasonic_H_
#define _ironmanUltrasonic_H_

#define SENSOR_ID            0x03
#define Ultrasonic_ADDRESS   SENSOR_ID

#define UltrasonicID         0
#define UltrasonicPOWER      2
#define UltrasonicHANDWARE   3
#define UltrasonicSOFTWARE   4
#define UltrasonicERR        5
#define SENSORREST           6
#define NRFENABLE            7
#define SENSORPORT           8
#define SETAVRADDR           9
#define SETPOWER             10
#define SETMOVE              11
#define UltrasonicAADDRESS16 16

#include "Arduino.h"

class ironmanUltrasonic {

  public:
    ironmanUltrasonic();
    boolean begin(uint8_t i2cAddr = Ultrasonic_ADDRESS);
    boolean available();
    uint8_t getReg8(uint8_t _reg);
    uint16_t getReg7to8(uint8_t _reg);
    uint16_t getID();
    uint8_t getPower();
    uint8_t getHandware();
    uint8_t getSoftware();
    uint8_t getErr();
    uint8_t getNrfEn();
    void write8(uint8_t dataaddr, uint8_t data);
    void Rest();
    void setNrfEN(boolean _en);
    void setPort(uint8_t port);
    void setAddr(uint8_t addr);
    void setPower(uint8_t power);
    void setMove(uint8_t _move);
    uint16_t getDistance();

  private:
    uint8_t _i2cAddr;
    boolean iicsta;

};

#endif
