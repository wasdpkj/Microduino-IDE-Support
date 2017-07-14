#include "Microduino_Motor.h"


Motor::Motor(uint8_t _pinA, uint8_t _pinB) {
	pinA = _pinA;
	pinB = _pinB;
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
	if(digitalPinHasPWM(pinA)){
	  analogWrite(pinA, _speed);
      digitalWrite(pinB, LOW);		
	}
	else if(digitalPinHasPWM(pinB)){
	  digitalWrite(pinA, HIGH);
      analogWrite(pinB, 255 - _speed);	  		
	}
  }
  else {
	if(digitalPinHasPWM(pinA)){
      analogWrite(pinA, 255 + _speed);
      digitalWrite(pinB, HIGH);	  	
	}
	else if(digitalPinHasPWM(pinB)){
      digitalWrite(pinA, LOW);	
	  analogWrite(pinB, abs(_speed));  		
	}
  }
}


void Motor::Brake() {
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
}