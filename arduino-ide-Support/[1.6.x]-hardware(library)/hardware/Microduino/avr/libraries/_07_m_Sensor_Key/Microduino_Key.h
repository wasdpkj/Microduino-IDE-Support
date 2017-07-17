// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// Microduino IDE Support：
// https://github.com/wasdpkj/Microduino-IDE-Support/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

// ==============
// E-mail:
// Kejia Pan
// pankejia@microduino.cc

// ==============
// Weibo:
// @老潘orz

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

class VirtualKey
{
public:
  VirtualKey();  
  void begin();
  uint8_t readVal(uint8_t _val);
  uint8_t readEvent(uint8_t _val, uint16_t _time = 1000);

private:  
  uint8_t pin;
  uint8_t keyVal;
  uint8_t keyCache;
  uint32_t keyTimer;
};

class DigitalKey
{
public:
  VirtualKey* vKey;
  DigitalKey(uint8_t _pin);  
  void begin(uint8_t _mode = INPUT);
  uint8_t readVal();
  uint8_t readEvent(uint16_t _time = 1000);

private:  
  uint8_t pin;
  uint8_t keyVal;
  uint8_t keyCache;
  uint32_t keyTimer;
};


class AnalogKey
{
public:
  VirtualKey* vKey;
  AnalogKey(uint8_t _pin);
  void begin(uint8_t _mode = INPUT);
  uint8_t readVal(uint16_t _min, uint16_t _max);
  uint8_t readEvent(uint16_t _min, uint16_t _max, uint16_t _time = 1000);

private:  
  uint8_t pin;
  uint8_t keyVal;
  uint8_t keyCache;
  uint32_t keyTimer;
};


#endif
