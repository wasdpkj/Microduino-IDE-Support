/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_MotorPlus

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // cuiwenjing@microduino.cc

  //日期：2017.11
*********************************************************/

#include "Microduino_MotorPlus.h"

MotorPlus::MotorPlus(uint8_t _addr) {
  devAddr = _addr;
}


bool MotorPlus::begin(uint8_t _bit) {
  _bit = constrain(_bit, BIT_8, BIT_14);
  multiple = 0X4000 >> _bit;
  speedRange = 1 << _bit - 1;
  Wire.begin();
  Wire.beginTransmission(devAddr);
  bool sta = (Wire.endTransmission() == 0);
  setSpeed(FREE, FREE);
  motorPlusVersion = getVersion();
  return sta;
}

bool MotorPlus::write16(uint8_t writeAddr, uint16_t *data, uint8_t len) {
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(writeAddr);              // sends one byte
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data[i] >> 8);            // sends one byte
    Wire.write(data[i] & 0xFF);            // sends one byte
  }
  return Wire.endTransmission();    // stop transmitting
}

uint8_t MotorPlus::requestData(uint8_t _dataAddr) {
  uint8_t returnByte = false;
  if(motorPlusVersion >= MOTO_VERSION){
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

void MotorPlus::reset(void) {
  if (motorPlusVersion < MOTO_VERSION) {
    return;
  }
  Wire.beginTransmission(devAddr); // transmit to device
  Wire.write(ADDR8_RESET);              // sends one byte
  Wire.write(0x0FE);
  Wire.endTransmission();
}

uint8_t MotorPlus::getVersion(void) {
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

bool MotorPlus::setSpeed1(int16_t _speed) {
  int16_t speedBuf;
  if (_speed == BRAKE) {
    speedBuf = _speed;
  }
  else {
    _speed = constrain(_speed, -speedRange, speedRange);
    speedBuf = _speed * multiple;
  }
  if (write16(ADDR16_SPEED1 * 2, (uint16_t *)&speedBuf, 1)) // stop transmitting
    return false;

  return requestData(ADDR8_FAULT);
}

bool MotorPlus::setSpeed2(int16_t _speed) {
  int16_t speedBuf;
  if (_speed == BRAKE) {
    speedBuf = _speed;
  }
  else {
    _speed = constrain(_speed, -speedRange, speedRange);
    speedBuf = _speed * multiple;
  }
  if (write16(ADDR16_SPEED2 * 2, (uint16_t *)&speedBuf, 1))   // sends one byte
    return false;

  return requestData(ADDR8_FAULT);
}

bool MotorPlus::setSpeed(int16_t _speed1, int16_t _speed2) {
  int16_t speedBuf[2];
  if (_speed1 == BRAKE) {
    speedBuf[0] = _speed1;
  }
  else {
    _speed1 = constrain(_speed1, -speedRange, speedRange);
    speedBuf[0] = _speed1 * multiple;
  }

  if (_speed2 == BRAKE) {
    speedBuf[1] = _speed2;
  }
  else {
    _speed2 = constrain(_speed2, -speedRange, speedRange);
    speedBuf[1] = _speed2 * multiple;
  }

  if (write16(ADDR16_SPEED1 * 2, (uint16_t *)speedBuf, 2))   // sends one byte
    return false;

  return requestData(ADDR8_FAULT);
}

