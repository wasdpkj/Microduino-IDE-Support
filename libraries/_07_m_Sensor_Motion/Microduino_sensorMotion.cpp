/*
    Microduino_sensorMotion.cpp

    Created on: 2018/01/19
        Author: cuiwenjing
*/

#include "Microduino_sensorMotion.h"
sensorMotion::sensorMotion() {

}

boolean sensorMotion::begin() {
  Wire.begin();
  Wire.beginTransmission(MOTION_IIC);
  return Wire.endTransmission();
}

uint8_t sensorMotion::getVersion(){
  uint8_t version = 0;
  I2Cdev::readByte(MOTION_IIC, ADDR8_VERSION, &version);    
  return version;
}

float sensorMotion::getFloat(uint8_t ADDR) {
  uint8_t buffer[4];
  I2Cdev::readBytes(MOTION_IIC, ADDR * 4, 4, buffer);
  return *((float *)buffer);
}

void sensorMotion::getMotionRaw6(float* ax, float* ay, float* az, float* gx, float* gy, float* gz) {
  uint8_t buffer[24];
  I2Cdev::readBytes(MOTION_IIC, 0x04, 24, buffer);
  *ax = *((float *)buffer);
  *ay = *((float *)(buffer + 4));
  *az = *((float *)(buffer + 8));
  *gx = *((float *)(buffer + 12));
  *gy = *((float *)(buffer + 16));
  *gz = *((float *)(buffer + 20));
}

void sensorMotion::getAccelerationRaw(float* ax, float* ay, float* az) {
  uint8_t buffer[12];
  I2Cdev::readBytes(MOTION_IIC, 0x04, 12, buffer);
  *ax = *((float *)buffer);
  *ay = *((float *)(buffer + 4));
  *az = *((float *)(buffer + 8));
}

float sensorMotion::getAccelerationRawX(void) {
  return getFloat(ADDR32_AX);
}
float sensorMotion::getAccelerationRawY(void) {
  return getFloat(ADDR32_AY);
}

float sensorMotion::getAccelerationRawZ(void) {
  return getFloat(ADDR32_AZ);
}

void sensorMotion::getRotationRaw(float* gx, float* gy, float* gz) {
  uint8_t buffer[12];
  I2Cdev::readBytes(MOTION_IIC, 0x10, 12, buffer);
  *gx = *((float *)buffer);
  *gy = *((float *)(buffer + 4));
  *gz = *((float *)(buffer + 8));
}

float sensorMotion::getRotationRawX(void) {
  return getFloat(ADDR32_GX);
}

float sensorMotion::getRotationRawY(void) {
  return getFloat(ADDR32_GY);
}
float sensorMotion::getRotationRawZ(void) {
  return getFloat(ADDR32_GZ);
}

void sensorMotion::getYawPitchRoll(float* _ypr) {
  uint8_t buffer[12];
  I2Cdev::readBytes(MOTION_IIC, 0x1C, 12, buffer);
  memcpy(_ypr, buffer, 12);
}

float sensorMotion::getYaw(void) {
  return getFloat(ADDR32_YAW);
}
float sensorMotion::getPitch(void) {
  return getFloat(ADDR32_PITCH);
}

float sensorMotion::getRoll(void) {
  return getFloat(ADDR32_ROLL);
}

