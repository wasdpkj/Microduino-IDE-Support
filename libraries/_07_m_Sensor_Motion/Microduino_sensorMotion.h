/*
    Microduino_sensorMotion.h

    Created on: 2018/01/19
        Author: cuiwenjing        
*/

#ifndef I2C_Motion_H_
#define I2C_Motion_H_

#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>

#define MOTION_IIC 0x6A

#define ADDR8_VERSION      0
#define ADDR32_AX          1
#define ADDR32_AY          2
#define ADDR32_AZ          3
#define ADDR32_GX          4
#define ADDR32_GY          5
#define ADDR32_GZ          6
#define ADDR32_YAW         7
#define ADDR32_PITCH       8
#define ADDR32_ROLL        9
#define ADDR8_RESET        40

class sensorMotion {

  public:
    sensorMotion();
    boolean begin(void);
	uint8_t getVersion();
    void getMotionRaw6(float* ax, float* ay, float* az, float* gx, float* gy, float* gz);
    void getAccelerationRaw(float* ax, float* ay, float* az);
    float getAccelerationRawX(void);
    float getAccelerationRawY(void);
    float getAccelerationRawZ(void);
    void getRotationRaw(float* gx, float* gy, float* gz);
    float getRotationRawX(void);
    float getRotationRawY(void);
    float getRotationRawZ(void);
    void getYawPitchRoll(float* _ypr);
    float getYaw(void);
    float getPitch(void);
    float getRoll(void);

  private:
    float getFloat(uint8_t ADDR);
    uint8_t sensorMotionAddress;
    
};

#endif 

