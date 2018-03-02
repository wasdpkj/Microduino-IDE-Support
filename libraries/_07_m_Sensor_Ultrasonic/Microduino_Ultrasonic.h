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

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <Arduino.h>
#include <Wire.h>

#define ULTRASONIC_ADDR_1  0X31
#define ULTRASONIC_ADDR_2  0X32
#define ULTRASONIC_ADDR_3  0X33
#define ULTRASONIC_ADDR_4  0X34

#define ULTRA_VERSION          3

#define ADDR16_DISTANCE       0
#define ADDR8_VERSION         2
#define ADDR8_BLIND           3

class Ultrasonic {

  public:
    Ultrasonic(uint8_t _UltrasonicAddress=ULTRASONIC_ADDR_1);
    boolean begin();  
    uint16_t getDistance();
    uint8_t getVersion();

  private:
    uint8_t UltrasonicAddress;
    uint8_t first_byte;
    uint8_t second_byte;
    uint16_t distance;
    uint8_t flag;
	uint8_t UltrasonicVersion;
};

#endif 

