#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_Sensor_Gesture.h"

irconmanGesture::irconmanGesture ()
{

}

boolean irconmanGesture::begin(uint8_t addr)
{
  _i2cAddr = addr;
  Wire.begin ();
  Wire.beginTransmission(_i2cAddr);
  iicsta = !Wire.endTransmission();
  return iicsta;
}

boolean irconmanGesture::available () {
  if (iicsta) {
    if (getID() == SENSOR_ID)
      return true;
    else
      return false;
  } else {
    Wire.begin ();
    Wire.beginTransmission(_i2cAddr);
    if (Wire.endTransmission())
      iicsta = true;
  }
}

uint8_t irconmanGesture::getReg8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint16_t irconmanGesture::getReg7to8(uint8_t _reg) {
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

uint16_t irconmanGesture::getID()
{
  return getReg7to8(GestureID);
}

uint8_t irconmanGesture::getPower()
{
  return getReg8(GesturePOWER);
}

uint8_t irconmanGesture::getHandware()
{
  return getReg8(GestureHANDWARE);
}

uint8_t irconmanGesture::getSoftware()
{
  return getReg8(GestureSOFTWARE);
}

uint8_t irconmanGesture::getErr()
{
  return getReg8(GestureERR);
}

uint8_t irconmanGesture::getNrfEn()
{
  return getReg8(NRFENABLE);
}

void irconmanGesture::write8(uint8_t dataaddr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(data);
  Wire.endTransmission();
}

void irconmanGesture::Rest() {
  write8(SENSORREST, 1);
}

void irconmanGesture::setNrfEN(boolean _en) {
  write8(NRFENABLE, _en);
}

void irconmanGesture::setPort(uint8_t port) {
  write8(SENSORPORT, port);
}

void irconmanGesture::setAddr(uint8_t addr) {
  write8(SETAVRADDR, addr);
}

void irconmanGesture::setPower(uint8_t power) {
  write8(SETPOWER, power);
}

void irconmanGesture::setMove(uint8_t _move) {
  write8(SETMOVE, _move);
}

void irconmanGesture::setMode(uint8_t _mode) {
  if (_mode > 1)
    return ;
  write8(SETMODE, _mode);
}

uint8_t irconmanGesture::readGesture() {
  uint8_t data = getReg8(GestureValue);
  if (dir != data && data <= DIR_DOWN) {
    dir = data;
    return dir;
  }
  return 0;
}

uint16_t irconmanGesture::readProximity() {
  return getReg7to8(GestureProximity);
}
