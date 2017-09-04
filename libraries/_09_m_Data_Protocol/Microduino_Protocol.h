#ifndef _MICRODUINO_PROTOCOL_H_
#define _MICRODUINO_PROTOCOL_H_

#include <SoftwareSerial.h>
#include "DataParse.h"

#include <Arduino.h>


#define CMD_ROBOT 0xC8
#define NUM_ROBOT 16

//将两个字节组合成一个WORD
#define MAKE_WORD(HI,LO)		(((((uint16_t)((uint8_t)HI)))<<8)| ((uint16_t)((uint8_t)LO)))


class ProtocolSer{
  public:
  
	ProtocolSer(HardwareSerial *ser, uint8_t _len);
	ProtocolSer(SoftwareSerial *ser, uint8_t _len);
	
    void begin(uint16_t _baud);
	bool available(void);
    void readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len);
	void readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len);
	void write(uint8_t cmd, uint8_t *_data, uint8_t _len);

  private:
  
  	SoftwareSerial *pSwSerial;
	HardwareSerial *pHwSerial;
	DataParse *dataParse;
	uint16_t baud;
	uint8_t length;

	void commonInit(void);
};


#if defined (__AVR_ATmega128RFA1__)

#include <ZigduinoRadio.h>

#define PRO_PORT ZigduinoRadio


class ProtocolZig{
  public:
    ProtocolZig(uint8_t _len);
    void begin(uint8_t _channel);
	bool available(void);
    void readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len);
	void readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len);
	void write(uint8_t cmd, uint8_t *_data, uint8_t _len);
    int8_t getRSSI();

  private:
	
	DataParse *dataParse;	
    uint8_t length;
};

#endif


#endif