#ifndef _MICRODUINO_Tem_Hum_H_
#define _MICRODUINO_Tem_Hum_H_

#include <Arduino.h>

#define I2C_ADDR_Tem_Hum                 (0xB8 >> 1)          //Tem_Hum温湿度计I2C地址

#define PARAM_Tem_Hum_READ                0x03                //读寄存器命令
#define REG_Tem_Hum_HUMIDITY_MSB          0x00                //湿度寄存器高位
#define REG_Tem_Hum_HUMIDITY_LSB          0x01                //湿度寄存器低位
#define REG_Tem_Hum_TEMPERATURE_MSB       0x02                //温度寄存器高位
#define REG_Tem_Hum_TEMPERATURE_LSB       0x03                //温度寄存器低位
#define REG_Tem_Hum_DEVICE_ID_BIT_24_31   0x0B                //32位设备ID高8位

class Tem_Hum
{
public:
	Tem_Hum();
	bool begin();
	uint32_t getUid();
	float getTemperature();
	float getHumidity();

private:
	uint8_t devAddr;  
	bool readRaw(uint8_t cmd, uint8_t readAddr, uint8_t readLen, uint8_t *buf);
};


#endif
//
// END OF FILE
//