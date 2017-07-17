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

//日期：2017.06
*********************************************************/

#include "Microduino_Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t _UltrasonicAddress) {
  UltrasonicAddress = _UltrasonicAddress;
}

boolean Ultrasonic::begin() {
  Wire.begin();
  Wire.beginTransmission(UltrasonicAddress);
  first_byte = Wire.endTransmission();
  return !first_byte;
}

uint16_t Ultrasonic::requstDistance() { 
  Wire.requestFrom(UltrasonicAddress, 2);
  while (Wire.available())
  {
    if (flag == 0)
    { flag++;
      first_byte = Wire.read(); // receive a byte as character
    }
    else if (flag == 1)
    { flag = 0;
      second_byte = Wire.read(); // receive a byte as character
      distance = second_byte + (first_byte << 8);
    }
  }
  return distance;
}
