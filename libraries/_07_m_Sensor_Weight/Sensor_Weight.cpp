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
#include "Sensor_Weight.h"
#include <math.h>
SensorWeight::SensorWeight(uint8_t _addr) {
  devAddr = _addr;
}


bool SensorWeight::begin(void) {
  Wire.begin();
  Wire.beginTransmission(devAddr);
  bool sta = (Wire.endTransmission() == 0);
  SensorWeightVersion = getVersion();
  return sta;
}

bool SensorWeight::write8(uint8_t writeAddr, uint8_t data, uint8_t len) {
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(writeAddr);              // sends one byte
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data & 0xFF);            // sends one byte
  }
  return (Wire.endTransmission() == 0);    // stop transmitting
}

void SensorWeight::setReset()
{
	write8(ADDR8_RESET, RESET_WEIGHT, 1);
}

void SensorWeight::setGain(uint8_t gain)
{
	write8(ADDR8_GAIN, gain, 1);
}

uint8_t SensorWeight::requestData(uint8_t _dataAddr) {
  uint8_t returnByte = false;
  long previous;
  if (SensorWeightVersion >= WEIGHT_VERSION) {
    Wire.beginTransmission(devAddr); 		// transmit to device
    Wire.write(_dataAddr);            	  // sends one byte
    Wire.endTransmission();
  }
  Wire.requestFrom(devAddr, (uint8_t)1);
  previous = millis();
  while (Wire.available())  {        //timeout
  	  if(millis() - previous > INTERVAL)
	  {
		  return 0;
	  }
    returnByte = Wire.read(); // receive a byte as character
  }
  return returnByte;
}

uint16_t SensorWeight::getGap(){
  uint32_t returnByte;
  uint8_t first_byte, second_byte;
  long previous;
  cnt = 0;
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(ADDR16_GAP);              // sends one byte
  Wire.endTransmission();
  Wire.requestFrom(devAddr, (uint8_t)2);
  previous = millis();
  while (Wire.available())
  {
	if(millis() - previous > INTERVAL)
	{
		return 0;
	}
    if (cnt == 0)
    {
      cnt++;
      first_byte = Wire.read();
    }
    else if (cnt == 1)
    {
      cnt = 0;
      second_byte = Wire.read();
      returnByte = second_byte + (first_byte << 8);
    }
  }
  GapValue = returnByte & 0xFFFF;
  return GapValue;
}
//计算Gap并存入传感器EEP

int SensorWeight::round_float(float number)
{
    return (number > 0.0) ? floor(number + 0.5) : ceil(number - 0.5);
}

uint16_t SensorWeight::calibration(uint16_t weight) {   
	uint32_t temp;
	float gap_temp;
	if(weight>2000)
	{
		return 0;
	}
	Weight = getData();
	temp = Weight - Maopi;
	gap_temp = temp / weight;
	GapValue = round_float(gap_temp);
	uint8_t first_byte, second_byte;
	first_byte = (GapValue >> 8) & 0xFF;
	second_byte = GapValue & 0xFF;
	write8(ADDR16_GAP, first_byte, 1);
	write8(ADDR16_GAP+1, second_byte, 1);
	write8(ADDR8_CHECK,ADDR8_SAVEGAP,1);
	return GapValue;
}
float SensorWeight::getWeight(void) {
  float returnWeight = 0;
  Weight = getData();
  /*
  Serial.print(Weight);
 Serial.print(" and ");
 Serial.println(Maopi);
 */
  if (Weight < Maopi)
  {
    Weight = Maopi;
  }
  returnWeight = Weight - Maopi;
  returnWeight = (float)((float)returnWeight / GapValue);
  return returnWeight;
}
//==============================================================================
//获取原始数据
uint32_t SensorWeight::getData(void) {   
  uint32_t returnByte;
  uint8_t first_byte, second_byte, third_byte;
  long previous;
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(ADDR24_WEIGHT);              // sends one byte
  Wire.endTransmission();
  Wire.requestFrom(devAddr, (uint8_t)3);
  previous = millis();
  while (Wire.available())
  {
	  if(millis() - previous > INTERVAL)
	  {
		  return 0;
	  }
    if (cnt == 0)
    {
      cnt++;
      first_byte = Wire.read();
    }
    else if (cnt == 1)
    {
      cnt++;
      second_byte = Wire.read();
      returnByte = second_byte + (first_byte << 8);
    } else if (cnt == 2)
    {
      cnt = 0;
      third_byte = Wire.read();
      returnByte = third_byte + (returnByte << 8);
    }
  }
  return returnByte & 0xFFFFFF;
}
//取得毛重
void SensorWeight::getMaopi(void) {
  Maopi = getData();
}
//重新取毛重，即示数清零
void SensorWeight::setZero(void) {
	getMaopi();
}
//==============================================================================
uint8_t SensorWeight::getVersion(void) {
  uint8_t returnByte = false;
  long previous;
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(ADDR8_VERSION);              // sends one byte
  Wire.endTransmission();
  Wire.requestFrom(devAddr, (uint8_t)1);
  previous = millis();
  while (Wire.available())  {
	  if(millis() - previous > INTERVAL)
	  {
		  return 0;
	  }
    returnByte = Wire.read(); // receive a byte as character
  }
  return returnByte;
}
//调试用
uint8_t SensorWeight::getCheck(void) {
  uint8_t returnByte = false;
  long previous;
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(ADDR8_CHECK);              // sends one byte
  Wire.endTransmission();
  Wire.requestFrom(devAddr, (uint8_t)1);
  previous = millis();
  while (Wire.available())  {
	  	  if(millis() - previous > INTERVAL)
	  {
		  return 0;
	  }
    returnByte = Wire.read(); // receive a byte as character
  }
  return returnByte;
}
