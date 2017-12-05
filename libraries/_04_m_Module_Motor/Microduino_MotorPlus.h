/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_MotorPlus

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // cuiwenjing@microduino.cc

  //日期：2017.11
*********************************************************/

#ifndef _MICRODUINO_MOTOR_PLUSE_H_
#define _MICRODUINO_MOTOR_PLUSE_H_

#include <Arduino.h>
#include <Wire.h>

#define MOTOR_ADDR1  0x70
#define MOTOR_ADDR2  0x71
#define MOTOR_ADDR3  0x72
#define MOTOR_ADDR4  0x73

#define BIT_14  14  //速度设置值二进制位数
#define BIT_13  13
#define BIT_12  12
#define BIT_11  11
#define BIT_10  10
#define BIT_9  	9
#define BIT_8  	8

#define FREE  	0   //释放
#define BRAKE  	20000  //刹车

#define ADDR16_SPEED1     0X00
#define ADDR16_SPEED2     0X01
#define ADDR8_FAULT       0X04
#define ADDR8_VERSION     0X1F
#define ADDR8_RESET       0X20

#define MOTO_VERSION      2

class MotorPlus {

  public:
    MotorPlus(uint8_t _addr = 0X73);
    bool begin(uint8_t _bit = BIT_8);
    void reset(void);
    uint8_t getVersion(void);
    bool setSpeed1(int16_t _speed);
    bool setSpeed2(int16_t _speed);
    bool setSpeed(int16_t _speed1, int16_t _speed2);

  private:
    uint8_t devAddr;
    uint8_t multiple;
    int16_t speedRange;

    bool write16(uint8_t writeAddr, uint16_t *data, uint8_t len);
    uint8_t requestData(uint8_t);
    uint8_t motorPlusVersion;
};

#endif /* LINERCCDLIB_LINERCCD_H_ */

