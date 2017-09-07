// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc


#ifndef _MICRODUINO_AT24Cxx_H_
#define _MICRODUINO_AT24Cxx_H_

#ifndef BUFFER_LENGTH //for ESP32
#define BUFFER_LENGTH I2C_BUFFER_LENGTH
#endif

#include <Arduino.h>
#include <Wire.h>

#define AT24Cxx_ADDR 0x50

class AT24Cxx
{
  // user-accessible "public" interface
  public:
    AT24Cxx(uint8_t _addr = AT24Cxx_ADDR);
	
	bool begin(void);
    uint16_t read(uint16_t iAddr, uint8_t *buf, uint16_t iCnt);
    uint8_t write(uint16_t iAddr, uint8_t iVal);
    uint8_t write(uint16_t iAddr, const char *pBuf, uint16_t iCnt);

    uint16_t readStr(uint16_t iAddr, uint8_t *buf, uint16_t iBufLen);
    uint8_t writeStr(uint16_t iAddr, const char *pBuf);

  private:
	uint8_t devAddr;
  
};
#endif
