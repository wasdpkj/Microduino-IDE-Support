#ifndef _ironmanmotorPro_H_
#define _ironmanmotorPro_H_

#include <Arduino.h>

#define MOTORPRO_ID  0x04

//电机模式预定义
#define MODE_OPEN        0x00
#define MODE_SPEED       0x01
#define MODE_POSITION    0x02

#define CLOSEMAXSPEED    500

//电机可设参数预定义
#define BRAKE   16383  //刹车
#define FREE    0       //释放 
#define BIT_13  13    //速度分辨率，13位分辨率，-8191~+8191
#define BIT_12  12
#define BIT_11  11
#define BIT_10  10
#define BIT_9   9
#define BIT_8   8     //默认速度分辨率，-255~+255

//电机IIC协议寄存器地址预定义
#define ADDR16_ID               0
#define ADDR8_MOTOR_POWER       2
#define ADDR8_HANDWARE_VERSION  3
#define ADDR8_SOFTWARE_VERSION  4
#define ADDR8_FAULT             5
#define ADDR8_RESET             6
#define ADDR8_NRF_ENABLE        7
#define ADDR8_PORT              8
#define ADDR8_I2CADDR           9
#define ADDR8_NRF_POWER         10
#define ADDR8_BLINK             11
//NC   12-15
#define ADDR32_REAL_POSITION    16
#define ADDR16_REAL_SPEED       20
#define ADDR16_SET_SPEED        22
#define ADDR32_SET_POSITION     24
#define ADDR8_MODE              28
//NC   29
#define ADDR16_SPD_KP           30
#define ADDR16_SPD_KI           32
#define ADDR16_SPD_KD           34
#define ADDR16_PST_KP           36
#define ADDR16_PST_KI           38
#define ADDR16_PST_KD           40

class ironmanmotorPro {

  public:
    ironmanmotorPro(uint8_t i2caddr);
    uint8_t begin(uint8_t _bit = BIT_8);
    boolean available();
    void setRatio(int16_t _ratio);//电机减速比
    void setResolution(uint8_t _resolution);//电机转一圈脉冲数
    int16_t getRatio();
    int8_t getResolution();

/****************发送IIC数据函数*************/
    void reset(uint8_t _reset = 1);    //复位，AVR/NRF优先级高的控制   
//    void setNrfEN(boolean _en);        //设控制优先级，仅NRF控制
//    void setAddr(uint8_t addr);        //设AVR的IIC地址，仅NRF控制
//    void setPower(bool power);         //设电源为电池电压/5V，仅NRF控制
//    void setBlink(uint8_t blink);      //设拖动程序块时灯效，仅NRF控制
    void setSpeed(int16_t speed);      //设置速度，开环时速度范围-8191~+8191,设为16383时刹车,闭环时为实际转速
    void setPosition(int32_t position);//设置位置，位置环时使用
    void setMode(uint8_t _mode);       //设置电机模式，开环模式、闭环速度、闭环位置
    void setS_PID_P(uint16_t skp);     //设置速度环PID参数P
    void setS_PID_I(uint16_t ski);     //设置速度环PID参数I
    void setS_PID_D(uint16_t skd);     //设置速度环PID参数D
    void setP_PID_P(uint16_t pkp);     //设置位置环PID参数P
    void setP_PID_I(uint16_t pki);     //设置位置环PID参数I
    void setP_PID_D(uint16_t pkd);     //设置位置环PID参数D

/****************获取IIC数据函数*************/
    uint16_t getID();            //获取传感器ID号
    bool getPowerFlag();         //获取传感器功率标志位
    uint8_t getHandwareVersion();//获取传感器硬件版本号
    uint8_t getSoftwareVersion();//获取传感器软件版本号
    uint8_t getFault();          //获取错误标志位
    bool getSetPower();          //获取NRF设置的传感器功率
    int8_t getMode();            //获取电机所处模式
    int16_t getSetSpeed();       //获取电机已设置的速度
    int32_t getSetPosition();    //获取电机已设置的位置
    int16_t getSpeed();          //获取有实际意义的电机速度，。。。待验证
    int16_t getSpeedRaw();       //获取电机速度原始脉冲数
    int32_t getPosition();       //获取电机位置 
    uint16_t getS_PID_P();       //获取速度环PID参数P      
    uint16_t getS_PID_I();       //获取速度环PID参数I 
    uint16_t getS_PID_D();       //获取速度环PID参数D 
    uint16_t getP_PID_P();       //获取位置环PID参数P 
    uint16_t getP_PID_I();       //获取位置环PID参数I 
    uint16_t getP_PID_D();       //获取位置环PID参数D 
    
  private:
    bool beginFlag;
    uint8_t _i2cAddr;
    void write8(uint8_t dataaddr, uint8_t data);
    void writeInt16(uint8_t dataaddr, int16_t data);
    void writeInt16_U(uint8_t dataaddr, int16_t data);
    void writeInt32(uint8_t dataaddr, int32_t data);
    uint8_t getReg7to8(uint8_t _reg);
    uint16_t getReg14to16(uint8_t _reg);
    uint32_t getReg28to32(uint8_t _reg);
    int16_t ratio = 60;
    uint8_t resolution = 12;
    uint8_t motorMode;
    uint8_t Multiple;
};

#endif

