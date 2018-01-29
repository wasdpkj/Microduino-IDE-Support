/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  //  版权所有：
  //  @Microduino_sy

  // 支持Microduino_MotorPlus

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // shenyang@microduino.cc

  //修改记录：兼容ESP32, 2017-09-07，by CWJ
*********************************************************/

#ifndef _MICRODUINO_MOTOR_H_
#define _MICRODUINO_MOTOR_H_

#include <Arduino.h>

#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define MOTOR0_PINA 6  
#define MOTOR0_PINB 8  //PWM
#define MOTOR1_PINA 7  //PWM   
#define MOTOR1_PINB 5
#else
#define MOTOR0_PINA 6  //PWM
#define MOTOR0_PINB 8  
#define MOTOR1_PINA 7
#define MOTOR1_PINB 5  //PWM  
#endif

#define FREE    0   //释放
#define SPEED_MAX 255

#if defined (ESP32)
#define MOTOR0_PINA D6  //PWM
#define MOTOR0_PINB D8  //PWM
#define MOTOR1_PINA D7  //PWM 
#define MOTOR1_PINB D5  //PWM
#define LEDC_TIMER_8BIT   8
#define LEDC_BASEFREQ     1000
#define SPEED_MAX         256
#define MAX_MOTOR         4
#define INVALID_MOTOR     255
#endif

class Motor {

  public:
    Motor(uint8_t _pinA, uint8_t _pinB);
    void begin();
    void setSpeed(int16_t _speed);
    void Brake();

  private:
    uint8_t pinA;
    uint8_t pinB;
#if defined(__AVR__)
    uint8_t _pinApwm;
    uint8_t _pinBpwm;
#endif
#if defined (ESP32)
    uint8_t LEDC_channel_0;
    bool ForwFlag;
    bool ReveFlag;
#endif

};
#endif
