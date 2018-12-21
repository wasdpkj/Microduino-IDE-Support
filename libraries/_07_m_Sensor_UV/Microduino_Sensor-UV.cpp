/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // ֧��Microduino_Sensor_UV

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //���ڣ�2018.2
*********************************************************/
#include "Microduino_Sensor-UV.h"

SensorUV::SensorUV(uint8_t _addr) {
  devAddr = _addr;
}

bool SensorUV::write8(uint8_t writeAddr, uint8_t data, uint8_t len) {
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(writeAddr);              // sends one byte
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data & 0xFF);            // sends one byte
  }
  return Wire.endTransmission();    // stop transmitting
}

void SensorUV::setReset()
{
	write8(ADDR8_RESET, RESET_UV, 1);
}


bool SensorUV::begin(void) {
  Wire.begin();
  Wire.beginTransmission(devAddr);
  bool sta = (Wire.endTransmission() == 0);
  SensorUVVersion = getVersion();
  return sta;
}

uint8_t SensorUV::requestData(uint8_t _dataAddr) {
  uint8_t returnByte = false;
  if (SensorUVVersion >= UV_VERSION) {
    Wire.beginTransmission(devAddr); // transmit to device
    Wire.write(_dataAddr);              // sends one byte
    Wire.endTransmission();
  }
  Wire.requestFrom(devAddr, (uint8_t)1);
  while (Wire.available())  {
    returnByte = Wire.read(); // receive a byte as character
  }
  return returnByte;
}

uint16_t SensorUV::getUV(void) {
  uint16_t returnByte = false;
  uint8_t a, b;
  a = requestData(ADDR16_UV);
  delay(2);
  b = requestData(ADDR16_UV + 1);
  returnByte = (a << 8) + b;
  return returnByte;
}
float SensorUV::getIntensity(void)
{
  UV_voltage = getUV();
  UV_voltage = (VREF * UV_voltage) / 1023;
  //The UV Index is a linear scale,range from 0~15
  uvIntensity = mapfloat(UV_voltage, 0.98, VREF, 0.0, 15.0);   //限制大小
  if(uvIntensity < 0.0)
  {
	  uvIntensity = 0.0;
  }else if(uvIntensity > 15.0)
  {
	  uvIntensity = 15.0;
  }
  return uvIntensity;
}

float SensorUV::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t SensorUV::getUVIndex(void)
{
	//uvIntensity = getIntensity();
	uint8_t UVIndex;
	if(uvIntensity >= 0.0 && uvIntensity< 3.0)
	{
		UVIndex = LOWER;
	}else if(uvIntensity >= 3.0 && uvIntensity < 6.0)
	{
		UVIndex = MODERATE;
	}else if(uvIntensity >=6.0 && uvIntensity < 8.0)
	{
		UVIndex = NORMAL;
	}else if(uvIntensity >=8.0 && uvIntensity < 11.0)
	{
		UVIndex = STRONG;
	}else if(uvIntensity >= 11.0)
	{
		UVIndex = EXTREME;
	}
	return UVIndex;
}
uint8_t SensorUV::getVersion(void) {
  uint8_t returnByte = false;
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(ADDR8_VERSION);              // sends one byte
  Wire.endTransmission();
  Wire.requestFrom(devAddr, (uint8_t)1);
  while (Wire.available())  {
    returnByte = Wire.read(); // receive a byte as character
  }
  return returnByte;
}
