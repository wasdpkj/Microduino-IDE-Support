#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_Sensor_Ultrasonic.h"

ironmanUltrasonic::ironmanUltrasonic()
{

}

boolean ironmanUltrasonic::begin(uint8_t addr)
{
  delay(2000);
  _i2cAddr = addr;
  Wire.begin ();
  Wire.beginTransmission(_i2cAddr);
  iicsta = !Wire.endTransmission();
  setPower(1);
  return iicsta;
}

boolean ironmanUltrasonic::available() {
  if (iicsta) {
    if (getID() == SENSOR_ID){
      setPower(1);
      return true;
    }
    else
      return false;
  } else {
    Wire.begin ();
    Wire.beginTransmission(_i2cAddr);
    if (Wire.endTransmission())
      iicsta = true;
  }
}

uint8_t ironmanUltrasonic::getReg8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint16_t ironmanUltrasonic::getReg7to8(uint8_t _reg) {
  uint8_t readH, readL;
  uint8_t ressta[2] = {0};
  uint16_t result16;
  ressta[0] = I2Cdev::readByte(_i2cAddr, _reg, &readL);
  ressta[1] = I2Cdev::readByte(_i2cAddr, _reg + 1, &readH);
  if (!ressta[0] || !ressta[1]) {
    result16 = 0;
  } else {
    result16 = readH << 7 | readL;
  }
  return result16;
}

uint16_t ironmanUltrasonic::getID()
{
  return getReg7to8(UltrasonicID);
}

uint8_t ironmanUltrasonic::getPower()
{
  return getReg8(UltrasonicPOWER);
}

uint8_t ironmanUltrasonic::getHandware()
{
  return getReg8(UltrasonicHANDWARE);
}

uint8_t ironmanUltrasonic::getSoftware()
{
  return getReg8(UltrasonicSOFTWARE);
}

uint8_t ironmanUltrasonic::getErr()
{
  return getReg8(UltrasonicERR);
}

uint8_t ironmanUltrasonic::getNrfEn()
{
  return getReg8(NRFENABLE);
}

void ironmanUltrasonic::write8(uint8_t dataaddr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(data);
  Wire.endTransmission();
}

void ironmanUltrasonic::Rest() {
  write8(SENSORREST, 1);
}

void ironmanUltrasonic::setNrfEN(boolean _en) {
  write8(NRFENABLE, _en);
}

void ironmanUltrasonic::setPort(uint8_t port) {
  write8(SENSORPORT, port);
}

void ironmanUltrasonic::setAddr(uint8_t addr) {
  write8(SETAVRADDR, addr);
}

void ironmanUltrasonic::setPower(uint8_t power) {
  write8(SETPOWER, power);
}

void ironmanUltrasonic::setMove(uint8_t _move) {
  write8(SETMOVE, _move);
}

uint16_t ironmanUltrasonic::getDistance() {
  return getReg7to8(UltrasonicAADDRESS16);
}

