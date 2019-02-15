#ifndef _ironmanGesture_H_
#define _ironmanGesture_H_

#define SENSOR_ID         0x07

#define MODE_Gesture      0
#define MODE_Proximity    1

#define Gesture_ADDRESS   0x01
#define GestureID         0
#define GesturePOWER      2
#define GestureHANDWARE   3
#define GestureSOFTWARE   4
#define GestureERR        5
#define SENSORREST        6
#define NRFENABLE         7
#define SENSORPORT        8
#define SETAVRADDR        9
#define SETPOWER          10
#define SETMOVE           11
#define SETMODE           17
#define GestureValue      18
#define GestureProximity  19
#define GestureAmbient    21
#define GestureColorR     23
#define GestureColorG     25
#define GestureColorB     27

#include "Arduino.h"

/* Direction definitions */
enum {
  DIR_NONE,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,
};

class ironmanGesture {

  public:
    ironmanGesture();
    boolean begin(uint8_t i2cAddr = Gesture_ADDRESS);
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
    void setMode(uint8_t _mode);
    uint8_t readGesture();
    uint16_t readProximity();
    
  private:
    uint8_t _i2cAddr;
    uint8_t dir;
    boolean iicsta;

};

#endif
