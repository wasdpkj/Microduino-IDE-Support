#ifndef _DATA_PARSE_H_
#define _DATA_PARSE_H_

#include <Arduino.h>

#define STEP_WAIT_AA 		0x01
#define STEP_WAIT_BB 		0x02
#define STEP_WAIT_CMD		0x03
#define STEP_WAIT_DATA		0x04
#define STEP_WAIT_SUM		0x05

#define STXA		0xAA
#define STXB		0xBB


class DataParse{
	
  public:
  
  DataParse(uint8_t _length);
  uint8_t getChecksum(uint8_t _cmd, uint8_t *_data, uint8_t _len);
  bool parse(uint8_t tempChar);
  void read(uint8_t *_cmd, uint8_t *_data, uint8_t _len);

  private:
  
  	uint8_t step;
	uint8_t length;
	uint8_t index;
	uint8_t cmd;
	uint8_t *data;
	
};

#endif