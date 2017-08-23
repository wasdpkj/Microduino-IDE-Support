/*
 * Tem_Hum_S2.cpp
 *
 * Author: Shenyang
 * Created: 2016/11/07
 */

#include <Wire.h>
#include <I2Cdev.h>
#include "Microduino_SHT2x.h"
#include <math.h>
#include <Arduino.h>

uint16_t Tem_Hum_S2::readRaw(uint8_t _reg) {
  uint8_t buf[3];
  I2Cdev::readBytes(devAddr, _reg, 3, buf);  
  return (uint16_t)buf[0]<<8 | buf[1];
}

bool Tem_Hum_S2::begin(Resolution resolution){
  Wire.begin();
  Wire.begin();
  Wire.beginTransmission(devAddr);
  if(Wire.endTransmission()){
    return false;
  }
  reset();
  delay(15);
  setResolution(resolution);
  return true;    
}

void Tem_Hum_S2::reset(void){
  Wire.beginTransmission(devAddr);
  Wire.write(Tem_Hum_S2_SOFT_RESET);
  Wire.endTransmission();

}

float Tem_Hum_S2::getTemperature() {
  uint16_t st = readRaw(Tem_Hum_S2_TEMP_HOLD);
  st &= ~0x0003;
  return convertStToTemperature(st);
}

float Tem_Hum_S2::getHumidity() {
  uint16_t srh = readRaw(Tem_Hum_S2_HUMI_HOLD);
  srh &= ~0x0003;
  return convertSrhToHumidity(srh);
}

void Tem_Hum_S2::heaterOn(void) {
  uint8_t value;
  I2Cdev::readByte(devAddr, Tem_Hum_S2_READ_USER, &value); 
  bitSet(value, 2);
  I2Cdev::writeByte(devAddr, Tem_Hum_S2_WRITE_USER, value);
}

void Tem_Hum_S2::heaterOff(void) {
  uint8_t value;
  I2Cdev::readByte(devAddr, Tem_Hum_S2_READ_USER, &value); 
  bitClear(value, 2);
  I2Cdev::writeByte(devAddr, Tem_Hum_S2_WRITE_USER, value);
}

void Tem_Hum_S2::otpReloadOff(void) {
  uint8_t value;
  I2Cdev::readByte(devAddr, Tem_Hum_S2_READ_USER, &value); 
  bitSet(value, 1);
  I2Cdev::writeByte(devAddr, Tem_Hum_S2_WRITE_USER, value);
}

void Tem_Hum_S2::otpReloadOn(void) {
  uint8_t value;
  I2Cdev::readByte(devAddr, Tem_Hum_S2_READ_USER, &value);
  bitClear(value, 1);
  I2Cdev::writeByte(devAddr, Tem_Hum_S2_WRITE_USER, value);
}

void Tem_Hum_S2::setResolution(Resolution resolution) {
  uint8_t value;
  I2Cdev::readByte(devAddr, Tem_Hum_S2_READ_USER, &value);
  value &= ~0b10000001;
  value |= static_cast<uint8_t>(resolution);
  I2Cdev::writeByte(devAddr, Tem_Hum_S2_WRITE_USER, value);
}

bool Tem_Hum_S2::isEOD(void) {
  uint8_t value;
  I2Cdev::readByte(devAddr, Tem_Hum_S2_READ_USER, &value);
  return bitRead(value, 6);
}
