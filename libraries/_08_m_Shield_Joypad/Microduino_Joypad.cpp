/*
  Joypad.cpp - Arduino Joypad board library
  Written by Enrico Gueli
  Copyright (c) 2012 Arduino(TM)  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "Microduino_Joypad.h"

_Joypad Joypad;

/*
 * The following constants tell, for each accelerometer
 * axis, which values are returned when the axis measures
 * zero acceleration.
 */

_Joypad::_Joypad() {
  for (byte p=0; p<4; p++) {
    pinMode(MUX_ADDR_PINS[p], OUTPUT);
  }
#if defined (ESP32)
  analogReadResolution(10);  //9-12 在此ADC设置为10bit精度
#endif  
}

unsigned int _Joypad::readChannel(byte channel) {
  (channel & 1) ? PIN_SET(MUX_ADDR_PINS[0]) : PIN_CLR(MUX_ADDR_PINS[0]);
  (channel & 2) ? PIN_SET(MUX_ADDR_PINS[1]) : PIN_CLR(MUX_ADDR_PINS[1]);
  (channel & 4) ? PIN_SET(MUX_ADDR_PINS[2]) : PIN_CLR(MUX_ADDR_PINS[2]);
  (channel & 8) ? PIN_SET(MUX_ADDR_PINS[3]) : PIN_CLR(MUX_ADDR_PINS[3]);
  
/*  digitalWrite(MUX_ADDR_PINS[0], (channel & 1) ? HIGH : LOW);
    digitalWrite(MUX_ADDR_PINS[1], (channel & 2) ? HIGH : LOW);
    digitalWrite(MUX_ADDR_PINS[2], (channel & 4) ? HIGH : LOW);
    digitalWrite(MUX_ADDR_PINS[3], (channel & 8) ? HIGH : LOW); */

  delayMicroseconds(10);

  return analogRead(MUX_COM_PIN);
}

boolean _Joypad::joyLowHalf(byte joyCh) {
  return (readChannel(joyCh) < 512 - JOYSTICK_DEAD_ZONE)
    ? LOW : HIGH;
}

boolean _Joypad::joyHighHalf(byte joyCh) {
  return (readChannel(joyCh) > 512 + JOYSTICK_DEAD_ZONE)
    ? LOW : HIGH;
}

boolean _Joypad::readButton(byte ch) {
  
  switch(ch) {
  case JOYSTICK_RIGHT:
    return joyLowHalf(CH_JOYSTICK_X);
  case JOYSTICK_LEFT:
    return joyHighHalf(CH_JOYSTICK_X);
  case JOYSTICK_UP:
    return joyLowHalf(CH_JOYSTICK_Y);
  case JOYSTICK_DOWN:
    return joyHighHalf(CH_JOYSTICK_Y);
  case JOYSTICK1_RIGHT:
    return joyLowHalf(CH_JOYSTICK1_X);
  case JOYSTICK1_LEFT:
    return joyHighHalf(CH_JOYSTICK1_X);
  case JOYSTICK1_UP:
    return joyLowHalf(CH_JOYSTICK1_Y);
  case JOYSTICK1_DOWN:
    return joyHighHalf(CH_JOYSTICK1_Y);
  }
    
  unsigned int val = readChannel(ch);
  return (val > 512) ? HIGH : LOW;
}