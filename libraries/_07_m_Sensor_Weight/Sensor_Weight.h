/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_Weight
  // 库版本
  // B1.0 新建传感器支持库
  // B1.1 更改函数名称，更易理解。
  //	  重量获取细化到小数(一位最佳)
  //	  固件更新为版本2
  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc
  //日期：2018.8
*********************************************************/
#ifndef _SENSOR_WEIGHT_H_
#define _SENSOR_WEIGHT_H_

#include <Arduino.h>
#include <Wire.h>

#define WEIGHT_ADDRESS 0x13             //device address

#define ADDR8_VERSION  			  0x00   //version 8bits
#define ADDR8_CHECK  		  	  0x01   //check 8bits
#define ADDR24_WEIGHT             0x05	//weight value 24bits from 0x05 32bits from 0x04
#define ADDR16_GAP				  0x0C
#define ADDR8_GAIN  		  	  0x0A   //gain 128/32/64
#define ADDR8_RESET				  0x0F   //reset sensor

#define ADDR8_SAVEGAP		      0x01  //flag for gap save to eeprom

#define GAIN_128		0
#define GAIN_64			2
#define RESET_WEIGHT	1

#define WEIGHT_VERSION 0x00

#define INTERVAL 2000
class SensorWeight {

  public:
    SensorWeight(uint8_t _addr = 0x18);
    bool begin(void);
    uint8_t getVersion(void);
	uint8_t getCheck(void);
    float getWeight(void);
    void  getMaopi(void);
	void  setZero(void);
    uint16_t calibration(uint16_t);
	uint16_t getGap();
    uint32_t getData(void);
	void setGain(uint8_t gain); 
	void setReset(void);
  private:
    uint8_t devAddr;
    int16_t uv_value;
	int round_float(float number);
	bool write8(uint8_t writeAddr, uint8_t data, uint8_t len);
    uint8_t requestData(uint8_t);
    uint8_t SensorWeightVersion;
    uint16_t cnt;
    uint32_t Weight;
    uint16_t GapValue;
    uint32_t Maopi;
};

#endif
