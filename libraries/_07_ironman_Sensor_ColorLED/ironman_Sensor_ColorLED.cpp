#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_Sensor_ColorLED.h"

ironmanColorLED::ironmanColorLED()
{

}

boolean ironmanColorLED::begin(uint8_t addr)
{
  delay(2000);
  _i2cAddr = addr;
  Wire.begin ();
  Wire.beginTransmission(_i2cAddr);
  iicsta = !Wire.endTransmission();
  return iicsta;
}

boolean ironmanColorLED::available() {
  if (iicsta) {
    if (getID() == SENSOR_ID)
      return true;
    else
      return false;
  } else {
    Wire.begin ();
    Wire.beginTransmission(_i2cAddr);
    if (Wire.endTransmission())
      iicsta = true;
  }
}

uint8_t ironmanColorLED::getReg8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint16_t ironmanColorLED::getReg7to8(uint8_t _reg) {
  uint8_t readH, readL;
  uint8_t ressta[2] = {0};
  uint16_t result16;
  ressta[0] = I2Cdev::readByte(_i2cAddr, _reg, &readL);
  ressta[1] = I2Cdev::readByte(_i2cAddr, _reg + 1, &readH);
  if (!ressta[0] || !ressta[1]) {
    result16 = 0;
  } else {
    result16 = readH << 7 | readL;
  }
  return result16;
}

uint16_t ironmanColorLED::getID()
{
  return getReg7to8(ColorLEDID);
}

uint8_t ironmanColorLED::getPower()
{
  return getReg8(ColorLEDPOWER);
}

uint8_t ironmanColorLED::getHandware()
{
  return getReg8(ColorLEDHANDWARE);
}

uint8_t ironmanColorLED::getSoftware()
{
  return getReg8(ColorLEDSOFTWARE);
}

uint8_t ironmanColorLED::getErr()
{
  return getReg8(ColorLEDERR);
}

void ironmanColorLED::write8(uint8_t dataaddr, uint8_t data) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(dataaddr);
  Wire.write(data);
  Wire.endTransmission();
}

void ironmanColorLED::writecmddata(uint8_t addr, uint8_t cmd, uint8_t *_data, uint8_t _len) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);
  Wire.write(cmd);
  Wire.write(_data, _len);
  Wire.endTransmission();
}

void ironmanColorLED::Rest() {
  write8(SENSORREST, 1);
}

void ironmanColorLED::setNrfEN(boolean _en) {
  write8(NRFENABLE, _en);
}

void ironmanColorLED::setPort(uint8_t port) {
  write8(SENSORPORT, port);
}

void ironmanColorLED::setAddr(uint8_t addr) {
  write8(SETAVRADDR, addr);
}

void ironmanColorLED::setPower(uint8_t _power) {
  write8(SETPOWER, _power);
}

void ironmanColorLED::setBrightness(uint8_t _bright) {
  bright = _bright;
}

void ironmanColorLED::setAllLED(uint8_t colorR, uint8_t colorG, uint8_t colorB) {
  cmd = 0;
  if (bright == 0) {
    memset(rgbdata, 0, 6);
  } else {
    brightrgb[0] = map(colorR, 0, 255, 0, bright);
    brightrgb[1] = map(colorG, 0, 255, 0, bright);
    brightrgb[2] = map(colorB, 0, 255, 0, bright);
    rgbdata[0] = brightrgb[0] & 0x7F;
    rgbdata[1] = brightrgb[0] >> 7;
    rgbdata[2] = brightrgb[1] & 0x7F;
    rgbdata[3] = brightrgb[1] >> 7;
    rgbdata[4] = brightrgb[2] & 0x7F;
    rgbdata[5] = brightrgb[2] >> 7;
  }
  writecmddata(ColorLEDNUM, cmd, rgbdata, 6);
  delay(10);
}

void ironmanColorLED::setAllLED(uint32_t _color) {
  setAllLED(_color >> 16 & 0xFF, _color >> 8 & 0xFF, _color & 0xFF);
}

void ironmanColorLED::setPixelColor(uint8_t n, uint8_t colorR, uint8_t colorG, uint8_t colorB) {
  if (n == 0)
    return;
  cmd = n;
  if (bright == 0) {
    memset(rgbdata, 0, 6);
  } else {
    brightrgb[0] = map(colorR, 0, 255, 0, bright);
    brightrgb[1] = map(colorG, 0, 255, 0, bright);
    brightrgb[2] = map(colorB, 0, 255, 0, bright);
    rgbdata[0] = brightrgb[0] & 0x7F;
    rgbdata[1] = brightrgb[0] >> 7;
    rgbdata[2] = brightrgb[1] & 0x7F;
    rgbdata[3] = brightrgb[1] >> 7;
    rgbdata[4] = brightrgb[2] & 0x7F;
    rgbdata[5] = brightrgb[2] >> 7;
  }
  writecmddata(ColorLEDNUM, cmd, rgbdata, 6);
  delay(10);
}

void ironmanColorLED::setPixelColor(uint8_t n, uint32_t _color) {
  setPixelColor(n, _color >> 16 & 0xFF, _color >> 8 & 0xFF, _color & 0xFF);
}

void ironmanColorLED::setOneLED(uint8_t n, uint8_t colorR, uint8_t colorG, uint8_t colorB) {
  setPixelColor(n, colorR, colorG, colorB);
}

void ironmanColorLED::setOneLED(uint8_t n, uint32_t _color) {
  setPixelColor(n, _color);
}

void ironmanColorLED::clear() {
  setAllLED(0, 0, 0);
}

