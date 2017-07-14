
#ifndef PULSE_H_
#define PULSE_H_

#define TIME1_MAX 200
#define TIME1_MIN 50  //过快的波
#define PULSE_MIN 40
#define PULSE_MAX 180
#define PULSE_UPDATA  3

#include <Arduino.h>
#include <Microduino_Key.h>

class Pulse {

  public:
    Pulse(uint8_t _Pin);
    void begin();
    uint16_t getPulse();
    bool available();

  private:
    DigitalKey *pulseKey;
    uint32_t getInterval();
    bool isUsefull(unsigned long _t);
    uint32_t pulse(unsigned long _t);
    int pinState;
    uint32_t thisTime = millis();
    uint32_t lastTime = millis();
    uint32_t time1;
    uint32_t time1_cache;
    boolean time1_sta_error = false;
    uint16_t pulse_num = 0;
    uint32_t pulse_vol = 0;
    uint32_t pulse_cache = 0;
    uint16_t pulse_t;
};

#endif

