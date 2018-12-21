/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  //	for Microduino_Sensor_UV

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //	2018.8
*********************************************************/
#ifndef _MICRODUINO_SENSOR_UV_H_
#define _MICRODUINO_SENSOR_UV_H_

#include <Arduino.h>
#include <Wire.h>

#define UV_ADDRESS 0x38

#define ADDR16_UV  		0x02
#define ADDR8_VERSION  	0x00
#define UV_VERSION		0x00
#define ADDR8_RESET		0x04

#define RESET_UV		1

#define LOWER		1
#define MODERATE	2
#define NORMAL		3	
#define STRONG		4
#define EXTREME		5

#define VREF 3.34
class SensorUV {
  public:
    SensorUV(uint8_t _addr = 0x38);
    bool begin(void);
    uint8_t getVersion(void);
    uint16_t getUV(void);
	float getIntensity(void);
	uint8_t getUVIndex(void);
	void setReset(void);
    uint8_t requestData(uint8_t);
	float UV_voltage;
	float uvIntensity;
  private:
    uint8_t devAddr;
    int16_t uv_value;

	float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
	bool write8(uint8_t writeAddr, uint8_t data, uint8_t len);
    uint8_t SensorUVVersion;
};

#endif
