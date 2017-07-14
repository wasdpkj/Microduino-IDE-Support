#include "Microduino_Key.h"

/****************** DigitalKey ******************************/

DigitalKey::DigitalKey(uint8_t _pin){
	pin = _pin;
}


void DigitalKey::begin(uint8_t _mode){
	if(_mode == INPUT || _mode == INPUT_PULLUP){
		pinMode(pin, _mode);
	}
}


uint8_t DigitalKey::readVal(){ // return the value as degrees

    keyCache = keyVal; //缓存作判断用
    keyVal = digitalRead(pin); //触发时

    if(keyCache && !keyVal)
		return KEY_PRESSING;
	else if(!keyCache && keyVal)
		return KEY_RELEASING;
	else if(keyCache && keyVal)
		return KEY_RELEASED;
	else if(!keyCache && !keyVal)
		return KEY_PRESSED;
}


uint8_t DigitalKey::readEvent(){ // return the value as degrees
	
	uint8_t keyBuf = readVal();
	
	if(keyBuf == KEY_PRESSING){
		keyTimer = millis();
	}
	
    if (millis() - keyTimer > 1000) {
	if (keyBuf == KEY_PRESSING || keyBuf == KEY_PRESSED)
        return LONG_PRESS;
      else
        return NOT_PRESS;
    }
    else if (keyBuf == KEY_RELEASED) {  //按下松开后
      keyTimer = 0;
      return SHORT_PRESS;
    }
    else
      return NOT_PRESS;	
}


/****************** AnalogKey ******************************/


AnalogKey::AnalogKey(uint8_t _pin){
	pin = _pin;
}


void AnalogKey::begin(uint8_t _mode){
	if(_mode == INPUT || _mode == INPUT_PULLUP){
		pinMode(pin, _mode);
	}
}

uint8_t AnalogKey::readVal(uint16_t _min, uint16_t _max){ // return the value as degrees

    uint16_t keyBuf = analogRead(pin);
	keyCache = keyVal; 
    keyVal = !(keyBuf>=_min && keyBuf<_max);

    if(keyCache && !keyVal)
		return KEY_PRESSING;
	else if(!keyCache && keyVal)
		return KEY_RELEASING;
	else if(keyCache && keyVal)
		return KEY_RELEASED;
	else if(!keyCache && !keyVal)
		return KEY_PRESSED;
}


uint8_t AnalogKey::readEvent(uint16_t _min, uint16_t _max){ // return the value as degrees
	
	uint8_t keyBuf = readVal(_min, _max);
	
	if(keyBuf == KEY_PRESSING){
		keyTimer = millis();
	}
	
    if (millis() - keyTimer > 1000) {
	if (keyBuf == KEY_PRESSING || keyBuf == KEY_PRESSED)
        return LONG_PRESS;
      else
        return NOT_PRESS;
    }
    else if (keyBuf == KEY_RELEASED) {  //按下松开后
      keyTimer = 0;
      return SHORT_PRESS;
    }
    else
      return NOT_PRESS;	
}