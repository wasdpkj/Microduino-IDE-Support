/*
    Microduino_MicroRobot.h

    Created on: 2018/02/01
        Author: cuiwenjing

修改记录：
  添加舵机原始脉冲控制角度方法
  将Pulse方法名改为Resolution
  将电机IIC地址设成数组
  减速比脉冲数乘积默认设为乐高电机的参数
  while里加超时处理      
  电机版本查询
*/

#ifndef __MICRO_ROBOT_H__
#define __MICRO_ROBOT_H__

#include <inttypes.h>
#include "mBattery_Pmu.h"

//****************servo预定义*****************
#define MAX_SERVOS        5
#define MIN_PULSE_WIDTH   544
#define MAX_PULSE_WIDTH   2450
#define OUTPUT_LOW      0
#define OUTPUT_HIGH     3000
#define ADDR16_BAT       2
#define ADDR16_SERVO     3
//**************************************

//****************motor预定义*****************
//电机IIC地址预定义
#define MOTORPRO_ADDR1 0x6F
#define MOTORPRO_ADDR2 0x6D
#define MOTORPRO_ADDR3 0x6E
#define MOTORPRO_ADDR4 0x6C

//电机模式预定义
#define MODE_OPEN        0x00
#define MODE_SPEED       0x01
#define MODE_POSITION     0x02
#define MODE_DIR_OPEN     0x10
#define MODE_DIR_SPEED    0x11
#define MODE_DIR_POSITION  0x12

//电机可设参数预定义
#define BRAKE  20000  //刹车
#define FREE  0       //释放
#define BIT_14  14    //速度分辨率，14位分辨率，-16384~+16384
#define BIT_13  13
#define BIT_12  12
#define BIT_11  11
#define BIT_10  10
#define BIT_9   9
#define BIT_8   8     //默认速度分辨率，-255~+255

//电机IIC协议寄存器地址预定义
#define ADDR8_VERSION           0
#define ADDR16_REAL_SPEED       2
#define ADDR16_REAL_POSITION    3
#define ADDR16_SET_SPEED        4
#define ADDR16_SET_POSITION     5
#define ADDR8_MODE              12
#define ADDR8_RESET             13
#define ADDR8_FAULT             14
#define ADDR8_RESERVED          15
#define ADDR32_SPD_KP           4
#define ADDR32_SPD_KI           5
#define ADDR32_SPD_KD           6
#define ADDR32_PST_KP           7
#define ADDR32_PST_KI           8
#define ADDR32_PST_KD           9

//结构体，解析电机IIC数据用
typedef union fourbyte1 {
  float pid;
  char Pid[4];
} fourbyte1;
typedef union twobyte1 {
  int twobyte;
  char Twobyte[2];
} twobyte1;
typedef union fourbyte2 {
  float fourbyte;
  uint8_t Fourbyte[4];
} fourbyte2;
//*********************************

//****************class:MicroRobot*****************************
//MicroRobot继承mBattery+的PMU方法
//MicroRobot可控制5路舵机，序号为0~4
//MicroRobot可控制4路电机，序号为1~4
class MicroRobot: public MbaPmu {
  public:
    MicroRobot();

    //***********************public：servo*********************************
    float getVoltage();
    void servoWrite(uint8_t index, int16_t value);
    uint16_t servoRead(uint8_t index);
    void servoWriteRaw(uint8_t index, int16_t value);
    uint16_t servoReadRaw(uint8_t index);
    void digitalWrite(uint8_t index, uint8_t value);
    //********************************************************

    //**************************public：motor******************************
    uint8_t motorInit(uint8_t index, uint8_t _bit = BIT_8);
    uint8_t getMotorVersion(uint8_t index);
    void setRatio(uint8_t index, int16_t _ratio);
    void setResolution(uint8_t index, uint8_t _resolution);
    void reset(uint8_t index, uint8_t _reset = 1);// _reset默认参数为1，此时速度、位置的设置参数会归零
                                                  //如果_reset>1，电机实际位置参数也会归零
    void setMode(uint8_t index, uint8_t _mode);
    void setS_PID(uint8_t index, float p, float i, float d);
    void setS_PID_P(uint8_t index, float p);
    void setS_PID_I(uint8_t index, float i);
    void setS_PID_D(uint8_t index, float d);
    void setP_PID(uint8_t index, float p, float i, float d);
    void setP_PID_P(uint8_t index, float p);
    void setP_PID_I(uint8_t index, float i);
    void setP_PID_D(uint8_t index, float d);
    void setSpeed(uint8_t index, int16_t speed);
    void setPosition(uint8_t index, int16_t position);

    int16_t getRatio(uint8_t index);
    int8_t getResolution(uint8_t index);
    int8_t getMode(uint8_t index);
    bool getFault(uint8_t index);
    float getS_PID_P(uint8_t index);
    float getS_PID_I(uint8_t index);
    float getS_PID_D(uint8_t index);
    float getP_PID_P(uint8_t index);
    float getP_PID_I(uint8_t index);
    float getP_PID_D(uint8_t index);
    int16_t getSetSpeed(uint8_t index);
    int16_t getSetPosition(uint8_t index);
    int16_t getSpeed(uint8_t index);
    int16_t getPosition(uint8_t index);
    int16_t getSpeedRaw(uint8_t index);
    //********************************************************

  private:
    //**************************private：motor******************************
    void write8(uint8_t index, uint8_t addr, uint8_t *data, uint8_t len);
    void write16(uint8_t index, uint8_t addr, int16_t *data, uint8_t len);
    void write32(uint8_t index, uint8_t addr, float *data, uint8_t len);
    int8_t get8_t(uint8_t index, uint8_t ADDR);
    int16_t get16_t(uint8_t index, uint8_t ADDR);
    float get32_t(uint8_t index, uint8_t ADDR);
    int16_t ratio[4] = {60, 60, 60, 60};
    uint8_t resolution[4] = {12, 12, 12, 12};
    uint8_t motorMode[4];
    uint8_t Multiple[4];
    uint8_t MotorAddress[4]={MOTORPRO_ADDR1,MOTORPRO_ADDR2,MOTORPRO_ADDR3,MOTORPRO_ADDR4};
    fourbyte1 val1;  //写PID参数用
    twobyte1 val2;   //
    fourbyte2 val3;  //获取PID参数用
    //********************************************************
};

#endif
