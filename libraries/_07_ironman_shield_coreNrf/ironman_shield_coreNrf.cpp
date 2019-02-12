#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_shield_coreNrf.h"

ironmanShieldNrf::ironmanShieldNrf() {
}

boolean ironmanShieldNrf::begin(uint8_t addr) {
  _i2cAddr = addr;
  Wire.begin();
  Wire.beginTransmission(_i2cAddr);
  iicsta = !Wire.endTransmission();
  return iicsta;
}

void ironmanShieldNrf::write8(uint8_t dataaddr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(data);
  Wire.endTransmission();
}

void ironmanShieldNrf::writedata(uint8_t addr, uint8_t *_data, uint8_t _len) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);
  Wire.write(_data, _len);
  Wire.endTransmission();
}

uint8_t ironmanShieldNrf::getReg8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint8_t ironmanShieldNrf::sdVersion()
{
  return getReg8(I2C_R_SD_VERSION);
}

uint8_t ironmanShieldNrf::hardwareVersion()
{
  return getReg8( I2C_R_HW_VERSION);
}

uint8_t ironmanShieldNrf::firmwareVersion()
{
  return getReg8(I2C_R_FW_VERSION);
}

uint8_t ironmanShieldNrf::priorityVal()
{
  return getReg8(I2C_R_Priority);
}

uint8_t ironmanShieldNrf::micVal()
{
  return getReg8(I2C_R_8_MIC);
}

uint8_t ironmanShieldNrf::batteryVal()
{
  return getReg8(I2C_R_8_BATTERY);
}

uint8_t ironmanShieldNrf::keyVal()
{
  return getReg8(I2C_R_KEY);
}

uint8_t ironmanShieldNrf::overtimeVal()
{
  return getReg8(Comm_OverTime);
}

void ironmanShieldNrf::priorityEN(uint8_t _mode) {
  write8(I2C_W_MODE, _mode);
}

void ironmanShieldNrf::tone(uint16_t freq) {
  tonefretime[0] = freq >> 8 & 0xFF;
  tonefretime[1] = freq & 0xFF;
  tonefretime[2] = 255;
  tonefretime[3] = 255;
  writedata(I2C_W_BEEP_FH, tonefretime, 4);
}

void ironmanShieldNrf::tone(uint16_t freq, uint16_t timer) {
  tonefretime[0] = freq >> 8 & 0xFF;
  tonefretime[1] = freq & 0xFF;
  tonefretime[2] = timer >> 8 & 0xFF;
  tonefretime[3] = timer & 0xFF;
  writedata(I2C_W_BEEP_FH, tonefretime, 4);
}

boolean ironmanShieldNrf::readKeyX() {
  return keyVal() & 0x01;
}

boolean ironmanShieldNrf::readKeyY() {
  return keyVal() & 0x02;
}

void ironmanShieldNrf::readKey() {
  uint8_t _key = keyVal();
  ironmankey[0] = _key & 0x01;
  ironmankey[1] = _key & 0x02;
  if (ironmanvalue[0] != ironmankey[0]) {
    if (ironmankey[0])
      twobuttonsta[0] = true;
    else
      twobuttonsta[1] = true;
    ironmanvalue[0] = ironmankey[0];
  }
  if (ironmanvalue[1] != ironmankey[1]) {
    if (ironmankey[1])
      twobuttonsta[2] = true;
    else
      twobuttonsta[3] = true;
    ironmanvalue[1] = ironmankey[1];
  }
}

boolean ironmanShieldNrf::shieldbuttonSta(uint8_t key, uint8_t mode) {
  readKey();

  if (twobuttonsta[0]  && mode == PRESSED && key == KEY_X && ironmanvalue[0]) {
    twobuttonsta[0] = false;
    return true;
  } if (twobuttonsta[1] && mode == RELEASED && key == KEY_X && !ironmanvalue[0]) {
    twobuttonsta[1] = false;
    return true;
  }
  if (twobuttonsta[2]  && mode == PRESSED && key == KEY_Y && ironmanvalue[1]) {
    twobuttonsta[2] = false;
    return true;
  } if (twobuttonsta[3] && mode == RELEASED && key == KEY_Y && !ironmanvalue[1]) {
    twobuttonsta[3] = false;
    return true;
  }
  return false;
}