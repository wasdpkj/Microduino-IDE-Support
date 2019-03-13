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

#define G98 9.80665

#define IIC_ADDR_MMA7660 0x4C
#define IIC_ADDR_SENSORMOTION 0x6A

enum SENSOR_TYPE{
  SENSOR_TYPE_UNKNOW = 0,
  SENSOR_TYPE_MMA7660,
  SENSOR_TYPE_SENSORMOTION
};


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


#define MMA7660_ADDR  0x4c

#define MMA7660_X     0x00
#define MMA7660_Y     0x01
#define MMA7660_Z     0x02
#define MMA7660_TILT  0x03
#define MMA7660_SRST  0x04
#define MMA7660_SPCNT 0x05
#define MMA7660_INTSU 0x06
  #define MMA7660_SHINTX 0x80
  #define MMA7660_SHINTY 0x40
  #define MMA7660_SHINTZ 0x20
  #define MMA7660_GINT 0x10
  #define MMA7660_ASINT 0x08
  #define MMA7660_PDINT 0x04
  #define MMA7660_PLINT 0x02
  #define MMA7660_FBINT 0x01
#define MMA7660_MODE  0x07
  #define MMA7660_STAND_BY 0x00
  #define MMA7660_ACTIVE   0x01
#define MMA7660_SR    0x08      //sample rate register
  #define AUTO_SLEEP_120  0X00//120 sample per second
  #define AUTO_SLEEP_64   0X01
  #define AUTO_SLEEP_32   0X02
  #define AUTO_SLEEP_16   0X03
  #define AUTO_SLEEP_8    0X04
  #define AUTO_SLEEP_4    0X05
  #define AUTO_SLEEP_2    0X06
  #define AUTO_SLEEP_1    0X07
#define MMA7660_PDET  0x09
#define MMA7660_PD    0x0A


class sensorMotion {

  public:
    sensorMotion();
    boolean begin(void);
	uint8_t getVersion();
	uint8_t getSensorType();
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
    uint8_t sensorType;
    uint8_t devAddr;
    float getFloat(uint8_t ADDR);
    uint8_t sensorMotionAddress;
};

#endif 

