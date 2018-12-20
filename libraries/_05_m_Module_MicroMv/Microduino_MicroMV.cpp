#include "Microduino_MicroMV.h"
#include <Wire.h>
MicroMV::MicroMV() {
}
bool MicroMV::Init() {
  Wire.begin();
  delay(100);
  for (byte i = 0; i < 50; i++) {
    if (getData()) {
      return true;
    }
    delay(100);
  }
  return false;
}
bool MicroMV::getData() {
  for (byte i = 0; i < 10; i++) {
    Wire.requestFrom(IIC_ADDRESS, DATA_LENGTH);
    delay(1);
    if (Wire.available() == DATA_LENGTH) {
      byte temp = 0;
      while (Wire.available()) {
        buff[temp++] = Wire.read();
      }
      delay(1);
      return true;
    }
    else {
      byte read_count=0;
      while (Wire.available()) {
        read_count++;
        if(read_count>31){
          break;
        }
        Wire.read();
      }
    }
    delay(1);
  }
  return false;
}
bool MicroMV::getTarget(byte color) {
  return buff[color * 4];
}
int MicroMV::getX(byte color) {
  int tmp = buff[2 + color * 4];
  return tmp - 80;
}
int MicroMV::getY(byte color) {
  int tmp = buff[3 + color * 4];
  return 60 - tmp;
}
bool MicroMV::isFull(byte color) {
  return buff[1 + color * 4];
}

