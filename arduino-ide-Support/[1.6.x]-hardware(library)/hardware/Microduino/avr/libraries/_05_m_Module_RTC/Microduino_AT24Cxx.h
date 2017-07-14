/*
 * AT24Cxx.h - library for AT24Cxx
 */

#ifndef _MICRODUINO_AT24Cxx_H_
#define _MICRODUINO_AT24Cxx_H_

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
