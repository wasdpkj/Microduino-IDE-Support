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

  Wire.beginTransmission(IIC_ADDR_SENSORMOTION);
  if(Wire.endTransmission() == 0){
    devAddr = IIC_ADDR_SENSORMOTION;
    sensorType = SENSOR_TYPE_SENSORMOTION;
    return true;
  }
  else{
    Wire.beginTransmission(IIC_ADDR_MMA7660);
    if(Wire.endTransmission() == 0){
  	  devAddr = IIC_ADDR_MMA7660;
      sensorType = SENSOR_TYPE_MMA7660;
	  
      I2Cdev::writeByte(devAddr, MMA7660_MODE, MMA7660_STAND_BY);	// Select mode register
      I2Cdev::writeByte(devAddr, MMA7660_SR, AUTO_SLEEP_32);		// Select sample rate register register
      I2Cdev::writeByte(devAddr, MMA7660_MODE, MMA7660_ACTIVE);	// Select mode register

      return true;
    }
  }
  sensorType = SENSOR_TYPE_UNKNOW;
  return false;
}

uint8_t sensorMotion::getSensorType(){
  return sensorType;
}

uint8_t sensorMotion::getVersion(){
  uint8_t version = 0;
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    I2Cdev::readByte(devAddr, ADDR8_VERSION, &version);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	version = 1;
  }
  return version;
}

float sensorMotion::getFloat(uint8_t ADDR) {
  uint8_t buffer[4];
  I2Cdev::readBytes(devAddr, ADDR * 4, 4, buffer);
  return *((float *)buffer);
}

void sensorMotion::getMotionRaw6(float* ax, float* ay, float* az, float* gx, float* gy, float* gz) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    uint8_t buffer[24];
    I2Cdev::readBytes(devAddr, 0x04, 24, buffer);
    *ax = *((float *)buffer);
    *ay = *((float *)(buffer + 4));
    *az = *((float *)(buffer + 8));
    *gx = *((float *)(buffer + 12));
    *gy = *((float *)(buffer + 16));
    *gz = *((float *)(buffer + 20));
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	getAccelerationRaw(ax ,ay ,az);	
	memset(gx, 0, 4);
	memset(gy, 0, 4);
	memset(gz, 0, 4);
  }
}

void sensorMotion::getAccelerationRaw(float* ax, float* ay, float* az) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    uint8_t buffer[12];
    I2Cdev::readBytes(devAddr, 0x04, 12, buffer);
    *ax = *((float *)buffer);
    *ay = *((float *)(buffer + 4));
    *az = *((float *)(buffer + 8));
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	int8_t x,y,z;
	uint8_t buffer[3];
	I2Cdev::readBytes(devAddr, 0x00, 3, buffer);
	
    x = ((int8_t)(buffer[0]<<2))/4;
    y = ((int8_t)(buffer[1]<<2))/4;
    z = ((int8_t)(buffer[2]<<2))/4;


    *ax = G98*x/21.00;
    *ay = G98*y/21.00;
    *az = G98*z/21.00;		
  }
}

float sensorMotion::getAccelerationRawX(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_AX);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	float _ax,_ay,_az;
	getAccelerationRaw(&_ax ,&_ay ,&_az);	

    return _ax;
  }
}
float sensorMotion::getAccelerationRawY(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_AY);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	float _ax,_ay,_az;
	getAccelerationRaw(&_ax ,&_ay ,&_az);	

    return _ay;
  }
}

float sensorMotion::getAccelerationRawZ(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_AZ);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	float _ax,_ay,_az;
	getAccelerationRaw(&_ax ,&_ay ,&_az);	

    return _az;
  }
}

void sensorMotion::getRotationRaw(float* gx, float* gy, float* gz) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    uint8_t buffer[12];
    I2Cdev::readBytes(devAddr, 0x10, 12, buffer);
    *gx = *((float *)buffer);
    *gy = *((float *)(buffer + 4));
    *gz = *((float *)(buffer + 8));
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	memset(gx, 0, 4);
	memset(gy, 0, 4);
	memset(gz, 0, 4);
  }
}

float sensorMotion::getRotationRawX(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_GX);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	return 0;
  }
}

float sensorMotion::getRotationRawY(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_GY);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	return 0;
  }
}
float sensorMotion::getRotationRawZ(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_GZ);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	return 0;
  }
}

void sensorMotion::getYawPitchRoll(float* _ypr) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    uint8_t buffer[12];
    I2Cdev::readBytes(devAddr, 0x1C, 12, buffer);
    memcpy(_ypr, buffer, 12);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	memset(_ypr, 0, 12);
  }
}

float sensorMotion::getYaw(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_YAW);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	return 0;
  }
}
float sensorMotion::getPitch(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_PITCH);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	return 0;
  }
}

float sensorMotion::getRoll(void) {
  if(sensorType == SENSOR_TYPE_SENSORMOTION){
    return getFloat(ADDR32_ROLL);
  }
  else if(sensorType == SENSOR_TYPE_MMA7660){
	return 0;
  }
}

