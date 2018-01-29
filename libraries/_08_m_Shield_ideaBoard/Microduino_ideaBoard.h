/*
    Microduino_ideaBoard.h

    Created on: 2017/08/10
        Author: cuiwenjing
*/

#ifndef _IDEA_BOARD_H_
#define _IDEA_BOARD_H_

#include <Wire.h>
#include <Arduino.h>

#define IdeaBoard_ADDR 0x56

#define GET_DATA_LEN  8

enum I2C_R {
  I2C_R_8_VERSION = 0,
  I2C_R_8_LEFT_A,
  I2C_R_8_LEFT_B,
  I2C_R_8_LEFT_C,
  I2C_R_8_JOY_Y ,
  I2C_R_8_JOY_X ,
  I2C_R_8_LIGHT ,
  I2C_R_8_MIC   ,
  I2C_R_8_KEY   ,
  I2C_R_8_ALL_BUTTON
} ;

enum I2C_W {
  I2C_W_8_RESET = 32,
  I2C_W_8_RIGHT_A  ,
  I2C_W_8_RIGHT_B  ,
  I2C_W_8_RIGHT_C  ,
  I2C_W_8_LED_MODE ,
  I2C_W_8_RGB_R    ,
  I2C_W_8_RGB_G    ,
  I2C_W_8_RGB_B      ,
  I2C_W_16_BUZZER = 40,
  I2C_W_16_DURATION = 42,
  I2C_W_8_SENSITIVE  = 44
};

#define ON   1
#define OFF  0
#define SENSITIVE 50

enum ADCDATA {
  LEFT_A,
  LEFT_B,
  LEFT_C,
  JOY_Y,
  JOY_X,
  LIGHT,
  MIC,
  KEY,
  ALL_BUTTON
};

enum KEYDATA {
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_A,
  BUTTON_B
};

class IdeaBoard {

  public:
    IdeaBoard();
    bool begin();
    static uint8_t getIN(uint8_t _num);
    static uint8_t getLight(void);
    static uint8_t getMic(void);
    static uint8_t getJoystickX(void);
    static uint8_t getJoystickY(void);
    static bool getButtonA(void);
    static bool getButtonB(void);
    static bool getButtonUp(void);
    static bool getButtonDown(void);
    static bool getButtonLeft(void);
    static bool getButtonRight(void);
    static int8_t getRAW(uint8_t *_n);
    static uint8_t getKey(void);

    void setLED(uint8_t _number, bool _state);
    void setLED(uint8_t _state);
    void setColorLED(uint8_t _r, uint8_t _g, uint8_t _b);
	void setColorLED(uint32_t _rgb);
    void tone(uint16_t _frequency, uint16_t _duration = -1);
    void noTone(void);
    void setOUT(uint8_t _numb, uint8_t _pwm);
    void setSensitive(uint8_t _range);

    static uint8_t i2cData[9];
  private:
    static uint8_t requstData(uint8_t _dataAddr);
    static uint8_t writeData(uint8_t _dataAddr, uint8_t _data);
    static uint8_t Button(void);
    uint8_t LED_State = 0x00;
};

extern IdeaBoard ideaBoard;

#endif

