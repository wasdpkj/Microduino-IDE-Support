#ifndef _MICRODUINO_PROTOCOL_H_
#define _MICRODUINO_PROTOCOL_H_

#include "DataParse.h"
#if defined (__AVR__)
#include <SoftwareSerial.h>
#endif
#include <RF24.h>
#include <Arduino.h>


#define CMD_ROBOT 0xC8
#define NUM_ROBOT 16

//将两个字节组合成一个WORD
#define MAKE_WORD(HI,LO)		(((((uint16_t)((uint8_t)HI)))<<8)| ((uint16_t)((uint8_t)LO)))

#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE 64
#endif /* SERIAL_RX_BUFFER_SIZE */

class ProtocolSer{
  public:
  
#if defined (__AVR__)
	ProtocolSer(SoftwareSerial *ser, uint8_t _len);
	ProtocolSer(HardwareSerial *ser, uint8_t _len);
#elif defined (ESP32)
	ProtocolSer(HardwareSerial *ser, uint8_t _len, int _rx = D2, int _tx = D3);
#endif

    void begin(uint16_t _baud);
	bool available(void);
    void readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len);
	void readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len);
	void write(uint8_t cmd, uint8_t *_data, uint8_t _len);

  private:
  
	HardwareSerial *pHwSerial;
#if defined (__AVR__)
  	SoftwareSerial *pSwSerial;
#elif defined (ESP32)
    uint8_t pinRX = D2;
    uint8_t pinTX = D3;
#endif
	DataParse *dataParse;
	uint16_t baud;
	uint8_t length;

	void commonInit(void);
};


class ProtocolnRF{
  public:

		ProtocolnRF(RF24 *_rf24, uint8_t _len); 
		bool begin(uint8_t _channel, const uint8_t *rxAddress, const uint8_t *txAddress);
  	bool available(void);
  	void readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len);
  	void readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len);
  	bool write(uint8_t cmd, uint8_t *_data, uint8_t _len);

  private:

		RF24 *rf24;
		uint8_t *dataBuf;
  	uint8_t length;
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