/*
    Microduino_Ultrasonic.h

    Created on: 2017/6/20
        Author: cuiwenjing
*/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define ULTRASONIC_ADDR_1  0X31
#define ULTRASONIC_ADDR_2  0X32
#define ULTRASONIC_ADDR_3  0X33
#define ULTRASONIC_ADDR_4  0X34

#include <Arduino.h>
#include <Wire.h>

class Ultrasonic {

  public:
    Ultrasonic(uint8_t _UltrasonicAddress=ULTRASONIC_ADDR_1);
    boolean begin();  
    uint16_t requstDistance();

  private:
    uint8_t UltrasonicAddress;
    uint8_t first_byte;
    uint8_t second_byte;
    uint16_t distance;
    uint8_t flag;
};

#endif 

