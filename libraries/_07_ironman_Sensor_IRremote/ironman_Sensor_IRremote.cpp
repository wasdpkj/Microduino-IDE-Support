#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_Sensor_IRremote.h"

ironmanIRremote::ironmanIRremote()
{

}

boolean ironmanIRremote::begin(uint8_t addr)
{
  delay(2000);
  _i2cAddr = addr;
  Wire.begin ();
  Wire.beginTransmission(_i2cAddr);
  iicsta = !Wire.endTransmission();
  return iicsta;
}

boolean ironmanIRremote::available() {
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

uint8_t ironmanIRremote::getReg8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint16_t ironmanIRremote::getReg7to8(uint8_t _reg) {
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

uint32_t ironmanIRremote::getReg40_7bitto32(uint8_t _reg) {
  uint8_t readByte1, readByte2, readByte3, readByte4, readByte5;
  uint8_t ressta[5] = {0};
  uint32_t _result32;
  ressta[0] = I2Cdev::readByte(_i2cAddr, _reg, &readByte1);
  ressta[1] = I2Cdev::readByte(_i2cAddr, _reg + 1, &readByte2);
  ressta[2] = I2Cdev::readByte(_i2cAddr, _reg + 2, &readByte3);
  ressta[3] = I2Cdev::readByte(_i2cAddr, _reg + 3, &readByte4);
  ressta[4] = I2Cdev::readByte(_i2cAddr, _reg + 4, &readByte5);

  if (!ressta[0] || !ressta[1] || !ressta[2] || !ressta[3] || !ressta[4]) {
    _result32 = 0;
  } else {
    _result32 = (uint32_t)readByte5 << 28 | (uint32_t)readByte4 << 21 | (uint32_t)readByte3 << 14 |
                ((uint32_t)readByte2) << 7 | (uint32_t)readByte1;
  }
  //delay(50);
  return _result32;
}

uint16_t ironmanIRremote::getID()
{
  return getReg7to8(IRremoteID);
}

uint8_t ironmanIRremote::getPower()
{
  return getReg8(IRremotePOWER);
}

uint8_t ironmanIRremote::getHandware()
{
  return getReg8(IRremoteHANDWARE);
}

uint8_t ironmanIRremote::getSoftware()
{
  return getReg8(IRremoteSOFTWARE);
}

uint8_t ironmanIRremote::getErr()
{
  return getReg8(IRremoteERR);
}

void ironmanIRremote::setReg32to40_7bit(int32_t _data32) {
  irsenddata[0] = _data32 & 0x7F;
  irsenddata[1] = _data32 >> 7 & 0x7F;
  irsenddata[2] = _data32 >> 14 & 0x7F;
  irsenddata[3] = _data32 >> 21 & 0x7F;
  irsenddata[4] = _data32 >> 28 & 0x0F;
}

void ironmanIRremote::write8(uint8_t dataaddr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(data);
  Wire.endTransmission();
}

void ironmanIRremote::writecmddata(uint8_t addr, uint8_t cmd, uint8_t *_data, uint8_t _len) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);
  Wire.write(cmd);
  Wire.write(_data, _len);
  Wire.endTransmission();
}

void ironmanIRremote::Rest() {
  write8(SENSORREST, 1);
}

void ironmanIRremote::setNrfEN(boolean _en) {
  write8(NRFENABLE, _en);
}

void ironmanIRremote::setPort(uint8_t port) {
  write8(SENSORPORT, port);
}

void ironmanIRremote::setAddr(uint8_t addr) {
  write8(SETAVRADDR, addr);
}

void ironmanIRremote::setPower(uint8_t _power) {
  write8(SETPOWER, _power);
}

boolean ironmanIRremote::irrecvAvailable() {
  if(getReg8(ADDR8_SEND_ALLOW)){
    irrecdata = getReg40_7bitto32(ADDR40_IRRECED_NECDATA);
    // Serial.print("irrecdata:");
    // Serial.println(irrecdata,HEX);
  }
  
  if(irrecdata>0)
    return true;

  return false;
}

uint32_t ironmanIRremote::irrecvValue() {
  return irrecdata;
}

void ironmanIRremote::sendNEC(uint32_t data) {
  if (data > 0xFFFFFFFF)
    return;
  setReg32to40_7bit(data);
  writecmddata(ADDR8_SEND_MODE, NEC, irsenddata , 5);
  delay(150);
}

void ironmanIRremote::sendSony(uint16_t data) {
  if (data > 0x0FFF)
    return;
  write8(ADDR8_SEND_MODE, SONY);
  Wire.beginTransmission(_i2cAddr);
  Wire.write(ADDR16U8_IRSENDING_SONYDATA);
  Wire.write(data & 0x7F);
  Wire.write(data >> 7 & 0x7F);
  Wire.endTransmission();
  delay(150);
}

void ironmanIRremote::remoteSta() {
  if (irrecvAvailable()) {
    remotevalue = irrecvValue();
    if (irrecdataCache!=remotevalue||remotevalue==0xFFFFFFFF){
      if (remotevalue > 0) {
        for (uint8_t i = 0; i < 16; i++) {
          if (remotevalue == remoteCode[i]) {
            remotevalueCache = remotevalue;
            twoirsta[0] = true;
            twoirsta[2] = true;
          }
        }
      }
      irrecdataCache=remotevalue;
    }
  }else if(twoirsta[2]){
    twoirsta[1] = true;
    twoirsta[2] = false;
  }
}

boolean ironmanIRremote::remotecontrolSta(uint32_t _value, uint8_t mode) {
  remoteSta();
  if (twoirsta[0] && remotevalueCache == _value && mode == PRESSED) {
    twoirsta[0] = false;
    return true;
  } else if (twoirsta[1] && remotevalueCache == _value && mode == RELEASED) {
    twoirsta[1] = false;
    return true;
  }
  return false;
}
