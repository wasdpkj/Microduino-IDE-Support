/*********************************************************
//  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

//  版权所有：
//  @沈阳

// 支持Microduino_MotorPlus

// Microduino wiki:
// http://wiki.microduino.cn

// E-mail:
// shenyang@microduino.cc

//日期：2017.06
*********************************************************/

#include "Microduino_Motor.h"

Motor::Motor(uint8_t _pinA, uint8_t _pinB) {
	pinA = _pinA;
	pinB = _pinB;
	_pinApwm = digitalPinHasPWM(pinA);
	_pinBpwm = digitalPinHasPWM(pinB);
	
}


void Motor::begin() {
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	setSpeed(FREE);
}

void Motor::setSpeed(int16_t _speed){
  _speed = constrain(_speed, -SPEED_MAX, SPEED_MAX);
	
  if (_speed == FREE)	{
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
  else if (_speed > 0)	{
	if(_pinApwm){
	  analogWrite(pinA, _speed);
      digitalWrite(pinB, LOW);		
	}
	else if(_pinBpwm){
	  digitalWrite(pinA, HIGH);
      analogWrite(pinB, 255 - _speed);	  		
	}
  }
  else {
	if(_pinApwm){
      analogWrite(pinA, 255 + _speed);
      digitalWrite(pinB, HIGH);	  	
	}
	else if(_pinBpwm){
      digitalWrite(pinA, LOW);	
	  analogWrite(pinB, abs(_speed));  		
	}
  }
}


void Motor::Brake() {
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
}