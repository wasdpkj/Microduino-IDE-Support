#ifndef __MICRODUINO_KEY_H__
#define __MICRODUINO_KEY_H__

#include <Arduino.h>

#define KEY_RELEASED	0
#define KEY_PRESSED		1
#define KEY_PRESSING	2
#define KEY_RELEASING	3

#define NOT_PRESS 		0
#define SHORT_PRESS 	1
#define LONG_PRESS 		2


class DigitalKey
{
public:
  DigitalKey(uint8_t _pin);  
  void begin(uint8_t _mode = INPUT);
  uint8_t readVal();
  uint8_t readEvent();

private:  
  uint8_t pin;
  uint8_t keyVal;
  uint8_t keyCache;
  uint32_t keyTimer;
};


class AnalogKey
{
public:
  AnalogKey(uint8_t _pin);
  void begin(uint8_t _mode = INPUT);
  uint8_t readVal(uint16_t _min, uint16_t _max);
  uint8_t readEvent(uint16_t _min, uint16_t _max);

private:  
  uint8_t pin;
  uint8_t keyVal;
  uint8_t keyCache;
  uint32_t keyTimer;
};


#endif
