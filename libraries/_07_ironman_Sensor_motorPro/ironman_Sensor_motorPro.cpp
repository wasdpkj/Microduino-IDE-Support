#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_Sensor_motorPro.h"

ironmanmotorPro::ironmanmotorPro(uint8_t i2caddr)
{
  _i2cAddr = i2caddr;
}

uint8_t ironmanmotorPro::begin(uint8_t _bit)
{
  Wire.begin ();
  _bit = constrain(_bit, 8, 13);
  Multiple = 0X2000 >> _bit;
  Wire.beginTransmission(_i2cAddr);
  beginFlag = !Wire.endTransmission();
  reset();
  delay(2000);
  return beginFlag;
}

boolean ironmanmotorPro::available() {
  if (beginFlag) {
    if (getID() == MOTORPRO_ID)
      return true;
    else
      return false;
  } else {
    Wire.begin ();
    Wire.beginTransmission(_i2cAddr);
    if (Wire.endTransmission()){
      return false;
    }else{
      beginFlag = true;
      return true;
    }
  }
}

void ironmanmotorPro::setRatio(int16_t _ratio) {
  ratio = _ratio;
}

void ironmanmotorPro::setResolution(uint8_t _resolution) {
  resolution = _resolution;
}

/****************设置IIC数据函数*************/
void ironmanmotorPro::reset(uint8_t _reset) {
  write8(ADDR8_RESET, _reset);
}

void ironmanmotorPro::setSpeed(int16_t speed) {
  int16_t speedBuf;
  int16_t _speed;
  if (speed == BRAKE) {
    speedBuf = speed;
  }
  else {
    if ((motorMode == MODE_OPEN)) {
      switch (Multiple) {
        case 0x20: speed = constrain(speed, -255, 255); break;
        case 0x10: speed = constrain(speed, -511, 511); break;
        case 0x8: speed = constrain(speed, -1023, 1023); break;
        case 0x4: speed = constrain(speed, -2047, 2047); break;
        case 0x2: speed = constrain(speed, -4095, 4095); break;
        case 0x1: speed = constrain(speed, -8191, 8191); break;
        default: break;
      }
      speedBuf = speed * Multiple;
    }
    else {
      if(speed>255)
        _speed=CLOSEMAXSPEED;
      else if(speed<-255)
        _speed=-CLOSEMAXSPEED;
      else
        _speed=map(speed,-255,255,-CLOSEMAXSPEED,CLOSEMAXSPEED);
      if(_speed==0){
        speedBuf=BRAKE;
      }else{
        float _speedBuf = ((float)_speed / 100.0);
        speedBuf = ratio * resolution * _speedBuf;
      }
    }
  }
  writeInt16(ADDR16_SET_SPEED, speedBuf);
}

void ironmanmotorPro::setPosition(int32_t position) {
  if ((motorMode == MODE_POSITION)) {
    writeInt32(ADDR32_SET_POSITION, position);
  }
}

void ironmanmotorPro::setMode(uint8_t _mode) {
  motorMode = _mode;
  write8(ADDR8_MODE, _mode);
}

void ironmanmotorPro::setS_PID_P(uint16_t skp) {
  writeInt16_U(ADDR16_SPD_KP, skp);
}

void ironmanmotorPro::setS_PID_I(uint16_t ski) {
  writeInt16_U(ADDR16_SPD_KI, ski);
}

void ironmanmotorPro::setS_PID_D(uint16_t skd) {
  writeInt16_U(ADDR16_SPD_KD, skd);
}

void ironmanmotorPro::setP_PID_P(uint16_t pkp) {
  writeInt16_U(ADDR16_PST_KP, pkp);
}
void ironmanmotorPro::setP_PID_I(uint16_t pki) {
  writeInt16_U(ADDR16_PST_KI, pki);
}

void ironmanmotorPro::setP_PID_D(uint16_t pkd) {
  writeInt16_U(ADDR16_PST_KD, pkd);
}

/****************获取IIC数据函数*************/
int16_t ironmanmotorPro::getRatio() {
  return ratio;
}

int8_t ironmanmotorPro::getResolution() {
  return resolution;
}

uint16_t ironmanmotorPro::getID()
{
  return getReg14to16(ADDR16_ID);
}

bool ironmanmotorPro::getPowerFlag() {
  return getReg7to8(ADDR8_MOTOR_POWER);
}

uint8_t ironmanmotorPro::getHandwareVersion()
{
  return getReg7to8(ADDR8_HANDWARE_VERSION);
}

uint8_t ironmanmotorPro::getSoftwareVersion()
{
  return getReg7to8(ADDR8_SOFTWARE_VERSION);
}

uint8_t ironmanmotorPro::getFault() {
  return getReg7to8(ADDR8_FAULT);
}

bool ironmanmotorPro::getSetPower()
{
  return (bool)getReg7to8(ADDR8_NRF_POWER);
}

int8_t ironmanmotorPro::getMode() {
  return getReg7to8(ADDR8_MODE);
}

int16_t ironmanmotorPro::getSetSpeed() {
  int16_t speedBuf = getReg14to16(ADDR16_SET_SPEED);
  if ((motorMode == MODE_OPEN)) {
    speedBuf = speedBuf / Multiple;
    if(speedBuf<0)
      return speedBuf-1;
    else
      return speedBuf;
  }
  else if ((motorMode == MODE_SPEED)) {
    float getSpeedBuf;
    getSpeedBuf = (float)speedBuf / (float)(ratio * resolution);
    speedBuf = getSpeedBuf * 100;
    int16_t _speedBuf=map(speedBuf,-CLOSEMAXSPEED,CLOSEMAXSPEED,-255,255);
    if(_speedBuf>0&&_speedBuf!=255)
      return _speedBuf+1;
    else 
      return _speedBuf;
  }
}

int32_t ironmanmotorPro::getSetPosition() {
  return getReg28to32(ADDR32_SET_POSITION);
}

int32_t ironmanmotorPro::getPosition() {
  int32_t result32;
  result32 = getReg28to32(ADDR32_REAL_POSITION);
  return result32;
}

int16_t ironmanmotorPro::getSpeedRaw() {
  int16_t result16;
  result16 = getReg14to16(ADDR16_REAL_SPEED);
  return result16;
}

int16_t ironmanmotorPro::getSpeed() {
  int16_t _realSpeed = getReg14to16(ADDR16_REAL_SPEED);
  float realSpeed = (_realSpeed * 100.0) / (ratio * resolution);
  if (motorMode == MODE_OPEN) {
    return (int16_t)realSpeed;
  }
  else if (motorMode == MODE_SPEED) {
    return (int16_t)map(realSpeed,-CLOSEMAXSPEED,CLOSEMAXSPEED,-255,255);
  }
  
}

uint16_t ironmanmotorPro::getS_PID_P() {
  return getReg14to16(ADDR16_SPD_KP);
}

uint16_t ironmanmotorPro::getS_PID_I() {
  return getReg14to16(ADDR16_SPD_KI);
}

uint16_t ironmanmotorPro::getS_PID_D() {
  return getReg14to16(ADDR16_SPD_KD);
}

uint16_t ironmanmotorPro::getP_PID_P() {
  return getReg14to16(ADDR16_PST_KP);
}

uint16_t ironmanmotorPro::getP_PID_I() {
  return getReg14to16(ADDR16_PST_KI);
}

uint16_t ironmanmotorPro::getP_PID_D() {
  return getReg14to16(ADDR16_PST_KD);
}

/****************私有函数*************/
uint8_t ironmanmotorPro::getReg7to8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint16_t ironmanmotorPro::getReg14to16(uint8_t _reg) {
  uint8_t readH, readL;
  uint16_t result16;
  uint8_t ressta[2] = {0};
  ressta[0] = I2Cdev::readByte(_i2cAddr, _reg, &readL);
  ressta[1] = I2Cdev::readByte(_i2cAddr, _reg + 1, &readH);
  if (!ressta[0] || !ressta[1]) {
    result16 = 0;
  } else {
    result16 = (uint16_t)readH << 7 | (uint16_t)readL;
    if (_reg < ADDR16_SPD_KP) {
      if (result16 > 0x1fff) result16 -= 0x3fff;
    }
  }
  return result16;
}

uint32_t ironmanmotorPro::getReg28to32(uint8_t _reg) {
  uint8_t readByte1, readByte2, readByte3, readByte4;
  uint32_t _result32;
  uint8_t ressta[4] = {0};
  ressta[0] = I2Cdev::readByte(_i2cAddr, _reg, &readByte1);
  ressta[1] = I2Cdev::readByte(_i2cAddr, _reg + 1, &readByte2);
  ressta[2] = I2Cdev::readByte(_i2cAddr, _reg + 2, &readByte3);
  ressta[3] = I2Cdev::readByte(_i2cAddr, _reg + 3, &readByte4);
  if (!ressta[0] || !ressta[1] || !ressta[2] || !ressta[3]) {
    _result32 = 0;
  } else {
    _result32 = (uint32_t)readByte4 << 21 | (uint32_t)readByte3 << 14 |
                ((uint32_t)readByte2) << 7 | (uint32_t)readByte1;
    if (_result32 > 0x7ffffff) _result32 -= 0xfffffff;
  }
  return _result32;
}

void ironmanmotorPro::write8(uint8_t dataaddr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(data);
  Wire.endTransmission();
}

void ironmanmotorPro::writeInt16(uint8_t dataaddr, int16_t data) {
  uint16_t _data16;
  _data16 = (uint16_t)data;
  if ((_data16 > 0x1fff) && (_data16 < BRAKE)) {
    _data16 = data + 0x3fff;
  }
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(_data16 & 0x7F);
  // Serial.print(_data16 & 0x7F);
  // Serial.print(",");
  Wire.write((_data16 >> 7) & 0x7F);
  // Serial.print((_data16 >> 7) & 0x7F);
  // Serial.print(",");
  Wire.endTransmission();
}

void ironmanmotorPro::writeInt16_U(uint8_t dataaddr, int16_t data) {
  uint16_t _data16;
  _data16 = (uint16_t)data;
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(_data16 & 0x7F);
  Wire.write((_data16 >> 7) & 0x7F);
  Wire.endTransmission();
}

void ironmanmotorPro::writeInt32(uint8_t dataaddr, int32_t data) {
  uint32_t _data32;
  _data32 = (uint32_t)data;
  if (_data32 >= 0x7ffffff) {
    _data32 = data + 0xfffffff;
  }
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(_data32 & 0x7F);
  Wire.write(_data32 >> 7 & 0x7F);
  Wire.write(_data32 >> 14 & 0x7F);
  Wire.write(_data32 >> 21 & 0x7F);
  Wire.endTransmission();
}