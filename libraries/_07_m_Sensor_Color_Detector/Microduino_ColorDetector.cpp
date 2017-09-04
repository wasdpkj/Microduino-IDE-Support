/****************************************************************************/
//
// TCS3414Lib is an Arduino library to communicate to and obtain values from
// the TCS3414 RGB color sensor.
//
// This work took minor parts (declarations)
// from the existing code by FrankieChu from www.seeedstudio.com
// available at https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor

// Copyright (C) 2014, J.F. Omhover (jf.omhover@gmail.com)
//
// This file is part of TCS3414Lib
//
// TCS3414Lib is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// TCS3414Lib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TCS3414Lib.  If not, see <http://www.gnu.org/licenses/>.
//
/******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "Microduino_ColorDetector.h"


static inline void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(TCS3414_ADDR);
  Wire.write(reg | CMD_WRITE | CMD_TRANSACTION_BYTE);  // systematically use CMD_WRITE bit to write values into registers
  Wire.write(val);
  Wire.endTransmission();
}

void ColorDetector::begin(uint8_t mode) {
  Wire.begin();
  switch(mode) {
  case TCS3414_LEVELINTERRUPTMODE:
    // the process below will set an interrupt each time the value of the CLEAR channel goes above 65530 (overflow, right ?)
    setLevelThreshold(REG_LOW_THRESH, 0x0000);
    setLevelThreshold(REG_HIGH_THRESH, 4000);
    writeReg(REG_INT_SOURCE, INT_SOURCE_CLEAR); // set the interrupt source to the CLEAR channel
    break; 
  case TCS3414_FREEMODE:
  default:
    writeReg(REG_TIMING, INTEG_MODE_FREE | INTEG_PARAM_INTTIME_12MS);
    writeReg(REG_INT, INTR_CTL_DISABLE);
    setGain(GAIN_1, PRESCALER_1); // set default gain value and prescaler value
    break;
  };
  setIntegrationTime(INTEG_PARAM_INTTIME_12MS);
  setGain(GAIN_1, PRESCALER_1);
  start();
}

void ColorDetector::start() {
  enableADC();
}
void ColorDetector::stop() {
  disableADC();
}

void ColorDetector::getRGB(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clr) {
  Wire.beginTransmission(TCS3414_ADDR);
  Wire.write(CMD_WRITE | REG_BLOCK_READ);
  Wire.endTransmission();
  Wire.beginTransmission(TCS3414_ADDR);
  Wire.requestFrom(TCS3414_ADDR, 8);
  while (Wire.available() < 8);  // TODO : do we really want to force to receive 8 bytes ???

  uint8_t *b = (uint8_t*)green;
  b[0] = Wire.read();
  b[1] = Wire.read();
  b = (uint8_t*)red;
  b[0] = Wire.read();
  b[1] = Wire.read();
  b = (uint8_t*)blue;
  b[0] = Wire.read();
  b[1] = Wire.read();
  b = (uint8_t*)clr;
  b[0] = Wire.read();
  b[1] = Wire.read();
  Wire.endTransmission();
}

void ColorDetector::disableADC() {
  writeReg(REG_CTL, CTL_POWER); 
}

void ColorDetector::enableADC() {
  writeReg(REG_CTL, CTL_ADC_EN | CTL_POWER); 
}

void ColorDetector::powerOn() {
  writeReg(REG_CTL, CTL_POWER); 
}

void ColorDetector::setLevelThreshold(uint8_t reg, uint16_t thresh) {
  Wire.beginTransmission(TCS3414_ADDR);
  Wire.write(reg | CMD_WRITE | CMD_TRANSACTION_WORD);
  Wire.write(thresh&0xFF);
  Wire.write(thresh>>8);
  Wire.endTransmission();
}

void ColorDetector::setIntegrationTime(uint8_t itime) {
  writeReg(REG_TIMING, itime);
}

void ColorDetector::setGain(uint8_t gain, uint8_t prescaler) {
  writeReg(REG_GAIN, gain | prescaler);
}

void ColorDetector::clearInterrupt() {
  Wire.beginTransmission(TCS3414_ADDR);
  Wire.write(CMD_CLEARINT);
  Wire.endTransmission();
}

