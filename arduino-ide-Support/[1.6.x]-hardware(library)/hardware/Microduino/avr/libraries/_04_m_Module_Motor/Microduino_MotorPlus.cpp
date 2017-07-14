 /*
    Microduino_Motor.cpp

    Created on: 2017/3/24
        Author: cuiwenjing
*/

#include "Microduino_MotorPlus.h"



MotorPlus::MotorPlus(uint8_t _addr) {
  devAddr = _addr;
}


bool MotorPlus::begin(uint8_t _bit) {
  _bit = constrain(_bit, BIT_8, BIT_14);
  multiple = 0X4000>>_bit;  
  Wire.begin();
  Wire.beginTransmission(devAddr);
  return (Wire.endTransmission() == 0);
}

bool MotorPlus::write16(uint8_t writeAddr, uint16_t *data, uint8_t len) {
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(writeAddr);              // sends one byte
  for(uint8_t i=0; i<len; i++){
    Wire.write(data[i]>>8);              // sends one byte
    Wire.write(data[i]&0xFF);              // sends one byte   
  }
  return Wire.endTransmission();    // stop transmitting
}

bool MotorPlus::requestData(){
  uint8_t returnByte = false;
  Wire.requestFrom(devAddr, (uint8_t)1);
  while (Wire.available())  {
    returnByte = Wire.read(); // receive a byte as character
  }
  return returnByte;
}

bool MotorPlus::setSpeed1(int16_t _speed) {
  int16_t speedBuf;
  if(_speed == BRAKE){
    speedBuf = _speed;
  }
  else{
    speedBuf = _speed * multiple;
  }
  if(write16(ADDR16_MOTOR1, (uint16_t *)&speedBuf, 1))    // stop transmitting
	return false;
	
  return requestData();
}

bool MotorPlus::setSpeed2(int16_t _speed) {
  int16_t speedBuf;
  if(_speed == BRAKE){
    speedBuf = _speed;
  }
  else{
    speedBuf = _speed * multiple;
  }
  if(write16(ADDR16_MOTOR2, (uint16_t *)&speedBuf, 1))      // sends one byte   
	return false;
	
  return requestData();
}

bool MotorPlus::setSpeed(int16_t _speed1, int16_t _speed2) {
  int16_t speedBuf[2];
  if(_speed1 == BRAKE){
    speedBuf[0] = _speed1;
  }
  else{
    speedBuf[0] = _speed1 * multiple;
  }
  
  if(_speed2 == BRAKE){
    speedBuf[1] = _speed2;
  }
  else{
    speedBuf[1] = _speed2 * multiple;
  } 
  
  if(write16(ADDR16_MOTOR1, (uint16_t *)speedBuf, 2))      // sends one byte   
	return false;
	
  return requestData();
}

