/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  //  版权所有：
  //  @小崔  cuiwenjing@microduino.cc

  // 支持Microduino_Sensor_Ultrasonic

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // Wenjing Cui
  // cuiwenjing@microduino.cc

  // Weibo:
  // @路蝶-6

  //日期：2017.11
*********************************************************/

#include "Microduino_Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t _UltrasonicAddress) {
  UltrasonicAddress = (uint8_t) _UltrasonicAddress;
}

boolean Ultrasonic::begin() {
  Wire.begin();
  Wire.beginTransmission(UltrasonicAddress);
  first_byte = Wire.endTransmission();
  
  UltrasonicVersion = Ultrasonic::getVersion(); //获取固件版本号

  return !first_byte;
}

uint16_t Ultrasonic::getDistance() {

  if (UltrasonicVersion >= ULTRA_VERSION) { //固件版本大于等于3时执行此函数
    Wire.beginTransmission(UltrasonicAddress); // transmit to device #8
    Wire.write(ADDR16_DISTANCE);              // sends one byte
    Wire.endTransmission();
  }
  Wire.requestFrom((uint8_t)UltrasonicAddress, (uint8_t)2);
  while (Wire.available())
  {
    if (flag == 0)
    { flag++;
      first_byte = Wire.read();
    }
    else if (flag == 1)
    { flag = 0;
      second_byte = Wire.read();
      distance = second_byte + (first_byte << 8);
    }
  }
  return distance;
}

uint8_t Ultrasonic::getVersion() {
    Wire.beginTransmission(UltrasonicAddress); // transmit to device #8
    Wire.write(ADDR8_VERSION);              // sends one byte
    Wire.endTransmission();    // stop transmitting
    Wire.requestFrom(UltrasonicAddress, (uint8_t)1);
    while (Wire.available())
    {
      uint8_t i = Wire.read();
      return  i;
    }
}
