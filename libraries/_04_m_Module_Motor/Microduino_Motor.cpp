/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  //  版权所有：
  //  @Microduino_sy

  // 支持Microduino_Motor

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // shenyang@microduino.cc

  //修改记录：兼容ESP32, 2017-09-07，by CWJ
*********************************************************/

#include "Microduino_Motor.h"

#if defined (ESP32)
static uint8_t motorNum = 0;
#endif

Motor::Motor(uint8_t _pinA, uint8_t _pinB) {
  pinA = _pinA;
  pinB = _pinB;
#if defined (ESP32)
  if (motorNum < MAX_MOTOR) {
    LEDC_channel_0 = motorNum + 4;
    motorNum++;
  }
  else {
    LEDC_channel_0 = INVALID_MOTOR;
    motorNum == INVALID_MOTOR;
  }
#endif

#if defined(__AVR__)
  _pinApwm = digitalPinHasPWM(pinA);
  _pinBpwm = digitalPinHasPWM(pinB);
#endif
}


void Motor::begin() {
#if defined (ESP32)
  ForwFlag = true;
  ReveFlag = true;
  ledcSetup(LEDC_channel_0, LEDC_BASEFREQ, LEDC_TIMER_8BIT);
#endif
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  setSpeed(FREE);
}

void Motor::setSpeed(int16_t _speed) {
  _speed = constrain(_speed, -SPEED_MAX, SPEED_MAX);

#if defined (ESP32)
  if (_speed == FREE) {
    if (ForwFlag && !ReveFlag) {
      ledcDetachPin(pinA);
      ReveFlag = true;
    }
    else if (!ForwFlag && ReveFlag) {
      ledcDetachPin(pinB);
      ForwFlag = true;
    }
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
  else if (_speed > 0)  {
    ForwFlag = true;
    if (ForwFlag && ReveFlag) {
      ReveFlag = false;
      ledcDetachPin(pinB);
      ledcAttachPin(pinA, LEDC_channel_0);
    }
    ledcWrite(LEDC_channel_0, _speed);
    digitalWrite(pinB, LOW);
  }
  else {
    ReveFlag = true;
    if (ForwFlag && ReveFlag) {
      ForwFlag = false;
      ledcDetachPin(pinA);
      ledcAttachPin(pinB, LEDC_channel_0);
    }
    digitalWrite(pinA, LOW);
    ledcWrite(LEDC_channel_0, abs(_speed));
  }
#endif

#if defined(__AVR__)
  if (_speed == FREE) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
  else if (_speed > 0)  {
    if (_pinApwm) {
      analogWrite(pinA, _speed);
      digitalWrite(pinB, LOW);
    }
    else if (_pinBpwm) {
      digitalWrite(pinA, HIGH);
      analogWrite(pinB, 255 - _speed);
    }
  }
  else {
    if (_pinApwm) {
      analogWrite(pinA, 255 + _speed);
      digitalWrite(pinB, HIGH);
    }
    else if (_pinBpwm) {
      digitalWrite(pinA, LOW);
      analogWrite(pinB, abs(_speed));
    }
  }
#endif
}

void Motor::Brake() {
#if defined (ESP32)
  if (ForwFlag && !ReveFlag) {
    ledcDetachPin(pinA);
    ReveFlag = true;
  }
  else if (!ForwFlag && ReveFlag) {
    ledcDetachPin(pinB);
    ForwFlag = true;
  }
#endif

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
}

