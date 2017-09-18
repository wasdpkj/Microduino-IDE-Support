// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// Microduino IDE Support：
// https://github.com/wasdpkj/Microduino-IDE-Support/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

// ==============
// E-mail:
// Kejia Pan
// pankejia@microduino.cc

// ==============
// Weibo:
// @老潘orz

#include "Microduino_Key.h"


/****************** VirtualKey ******************************/

VirtualKey::VirtualKey(){
}

void VirtualKey::begin(){
	keyCache = true;
	keyVal = false;
	keyTimer = 0;
  buttonsta[0] = false;
  buttonsta[1] = false;
  buttonsta[2] = false;
  buttonsta[3] = false;
}


uint8_t VirtualKey::readVal(uint8_t _val, uint8_t _sta) { // return the value as degrees
  keyCache = keyVal;
  keyVal = _val; //触发时

  if(_sta == KEY_NONE){
        if(!keyCache && keyVal)
		return KEY_PRESSING;
	else if(keyCache && !keyVal)
		return KEY_RELEASING;
	else if(!keyCache && !keyVal)
		return KEY_RELEASED;
	else if(keyCache && keyVal)
		return KEY_PRESSED;
  }
  else{
    if (!keyCache && keyVal) {//按下
      if (!buttonsta[3])
      {
        buttonsta[1] = true;
        buttonsta[3] = true;
      }
    }
    else if (keyCache && !keyVal) {//松开
      if (buttonsta[3])
      {
        buttonsta[2] = true;
        buttonsta[3] = false;
      }
    }
    else if (!keyCache && !keyVal) {//一直松开
      buttonsta[0] = false;
    }
    else if (keyCache && keyVal) {//一直按住
      buttonsta[0] = true;
    }
    if (_sta == KEY_RELEASED && !buttonsta[0] && !_val) {//一直松开
      return true;
    }
    else if (_sta == KEY_PRESSING && buttonsta[1]) {//按下
      buttonsta[1] = false;
      return true;
    }
    else if (_sta == KEY_PRESSED && buttonsta[0] && _val) {//一直按着
      return true;
    }
    else if (_sta == KEY_RELEASING && buttonsta[2]) {//松开
      buttonsta[2] = false;
      return true;
    }
    return false;
  }
}


uint8_t VirtualKey::readEvent(uint8_t _val, uint16_t _time){ // return the value as degrees
	uint8_t keyBuf = readVal(_val);
	
	if(keyBuf == KEY_PRESSING){
		keyTimer = millis();
	}
	
    if (millis() - keyTimer > _time) {
	if (keyBuf == KEY_PRESSING || keyBuf == KEY_PRESSED)
        return LONG_PRESS;
      else
        return NOT_PRESS;
    }
    else if (keyBuf == KEY_RELEASING) {  //按下松开后
      keyTimer = 0;
      return SHORT_PRESS;
    }
    else
      return NOT_PRESS;	
}



/****************** DigitalKey ******************************/

DigitalKey::DigitalKey(uint8_t _pin){
	pin = _pin;
	vKey = new VirtualKey;
}


void DigitalKey::begin(uint8_t _mode){
	if(_mode == INPUT || _mode == INPUT_PULLUP){
		pinMode(pin, _mode);
	}
	vKey->begin();
}


uint8_t DigitalKey::readVal(uint8_t _sta) { // return the value as degrees
  return (vKey->readVal(!digitalRead(pin), _sta));
}


uint8_t DigitalKey::readEvent(uint16_t _time){ // return the value as degrees
	return (vKey->readEvent(!digitalRead(pin),_time));
}


/****************** AnalogKey ******************************/


AnalogKey::AnalogKey(uint8_t _pin){
	pin = _pin;
	vKey = new VirtualKey;
}


void AnalogKey::begin(uint8_t _mode){
	if(_mode == INPUT || _mode == INPUT_PULLUP){
		pinMode(pin, _mode);
	}
	vKey->begin();
}

uint8_t AnalogKey::readVal(uint16_t _min, uint16_t _max, uint8_t _sta) { // return the value as degrees
  uint16_t keyBuf = analogRead(pin);
  return (vKey->readVal(keyBuf >= _min && keyBuf < _max, _sta));
}


uint8_t AnalogKey::readEvent(uint16_t _min, uint16_t _max, uint16_t _time){ // return the value as degrees	
    uint16_t keyBuf = analogRead(pin);
	return (vKey->readEvent(bool(keyBuf>=_min && keyBuf<_max),_time));
}