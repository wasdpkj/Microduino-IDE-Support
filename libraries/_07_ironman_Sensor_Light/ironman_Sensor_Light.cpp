#include <Wire.h>
#include <I2Cdev.h>
#include "ironman_Sensor_Light.h"

ironmanLight::ironmanLight()
{

}

boolean ironmanLight::begin (uint8_t addr)
{
  delay(2000);
  _i2cAddr = addr;
  Wire.begin ();
  Wire.beginTransmission(_i2cAddr);
  iicsta = !Wire.endTransmission();
  return iicsta;
}

boolean ironmanLight::available() {
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

void ironmanLight::write8(uint8_t addr, uint8_t d) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);
  Wire.write(d);
  Wire.endTransmission();
}

void ironmanLight::writecmddata(uint8_t addr, uint8_t cmd, uint8_t *_data, uint8_t _len) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(addr);
  Wire.write(cmd);
  Wire.write(_data, _len);
  Wire.endTransmission();
}

void ironmanLight::Rest() {
  write8(SENSORREST, 1);
}

void ironmanLight::setPort(uint8_t port) {
  write8(SENSORPORT, port);
}

void ironmanLight::setAddr(uint8_t addr) {
  write8(SETAVRADDR, addr);
}

void ironmanLight::setNrfEN(boolean _en) {
  write8(NRFENABLE, _en);
}
void ironmanLight::setPower(uint8_t _power) {
  write8(SETPOWER, _power);
}

void ironmanLight::setColorLEDA(int ar, int ag, int ab)
{
  colorledDataCache[0] = ar & 0x7F;
  colorledDataCache[1] = ar >> 7;
  colorledDataCache[2] = ag & 0x7F;
  colorledDataCache[3] = ag >> 7;
  colorledDataCache[4] = ab & 0x7F;
  colorledDataCache[5] = ab >> 7;
  writecmddata(SETMODE, COLORLEDMODE , colorledDataCache, 12);
}

void ironmanLight::setColorLEDB(int br, int bg, int bb)
{
  colorledDataCache[6] = br & 0x7F;
  colorledDataCache[7] = br >> 7;
  colorledDataCache[8] = bg & 0x7F;
  colorledDataCache[9] = bg >> 7;
  colorledDataCache[10] = bb & 0x7F;
  colorledDataCache[11] = bb >> 7;
  writecmddata(SETMODE, COLORLEDMODE, colorledDataCache, 12);
}

void ironmanLight::setColorLEDA(int32_t _rgb)
{
  colorledDataCache[0] = _rgb >> 16 & 0x7F;
  colorledDataCache[1] = _rgb >> 23;
  colorledDataCache[2] = _rgb >> 8 & 0x7F;
  colorledDataCache[3] = _rgb >> 15;
  colorledDataCache[4] = _rgb & 0x7F;
  colorledDataCache[5] = _rgb >> 7;
  writecmddata(SETMODE, COLORLEDMODE, colorledDataCache, 12);
}

void ironmanLight::setColorLEDB(int32_t _rgb)
{
  colorledDataCache[6] = _rgb >> 16 & 0x7F;
  colorledDataCache[7] = _rgb >> 23;
  colorledDataCache[8] = _rgb >> 8 & 0x7F;
  colorledDataCache[9] = _rgb >> 15;
  colorledDataCache[10] = _rgb & 0x7F;
  colorledDataCache[11] = _rgb >> 7;
  writecmddata(SETMODE, COLORLEDMODE, colorledDataCache, 12);
}

void ironmanLight::setAllLED(int32_t _rgb)
{
  setColorLEDA(_rgb);
  setColorLEDB(_rgb);
}

void ironmanLight::setAllLED(int ar, int ag, int ab)
{
  setColorLEDA(ar, ag, ab);
  setColorLEDB(ar, ag, ab);
}

uint8_t ironmanLight::getReg8(uint8_t _reg)
{
  uint8_t result8, resultsta;
  resultsta = I2Cdev::readByte(_i2cAddr, _reg, &result8);
  if (resultsta)
    return result8;
  else
    return 0;
}

uint16_t ironmanLight::getReg7to8(uint8_t _reg) {
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

uint16_t ironmanLight::getID()
{
  return getReg7to8(LIGHTID);
}

uint8_t ironmanLight::getPower()
{
  return getReg8(LIGHTPOWER);
}

uint8_t ironmanLight::getHandware()
{
  return getReg8(LIGHTHANDWARE);
}

uint8_t ironmanLight::getSoftware()
{
  return getReg8(LIGHTSOFTWARE);
}

uint8_t ironmanLight::getErr()
{
  return getReg8(LIGHTERR);
}

uint8_t ironmanLight::getNrfEn()
{
  return getReg8(NRFENABLE);
}

void ironmanLight::getLine(uint8_t *_line, uint8_t _colorA, uint8_t _colorB)
{
  if (_colorA > 3 || _colorB > 3)
    return;
  memset(colorledline, 0, 12);
  if (_colorA != BLACK) {
    colorledline[2 * (_colorA - 1)] = 1;
  }
  if (_colorB != BLACK) {
    colorledline[6 + 2 * (_colorB - 1)] = 1;
  }
  writecmddata(SETMODE, LINEMODE, colorledline, 12);

  _line[0] = getReg7to8(GETLINEA);
  _line[1] = getReg7to8(GETLINEB);
}

uint8_t ironmanLight::getLineA(uint8_t _colorA) {
  aloneline[0] = _colorA;
  uint8_t linecache[2];
  getLine(linecache, aloneline[0], aloneline[1]);
  return linecache[0];
}

uint8_t ironmanLight::getLineB(uint8_t _colorB) {
  aloneline[1] = _colorB;
  uint8_t linecache[2];
  getLine(linecache, aloneline[0], aloneline[1]);
  return linecache[1];
}

void ironmanLight::getColorRaw(uint8_t *_color, uint8_t _ar, uint8_t _ag, uint8_t _ab, uint8_t __Br, uint8_t __Bg, uint8_t __Bb) {
  colorledcolor[0] = _ar;
  colorledcolor[2] = _ag;
  colorledcolor[4] = _ab;
  colorledcolor[6] = __Br;
  colorledcolor[8] = __Bg;
  colorledcolor[10] = __Bb;

  writecmddata(SETMODE, COLORMODE, colorledcolor, 12);

  for (uint8_t i = 0; i < SEND_LEN_COLOR; i++) {
    _color[i] = getReg7to8(GETCOLOR + 2 * i);
  }
}

uint8_t ironmanLight::isValid(uint8_t value)
{
  if (value >= 0 && value <= 255)
    return 1;
  return 0;
}

uint8_t ironmanLight::GetMax(uint8_t __R , uint8_t __G , uint8_t __B)
{
  Max = __G;
  if (__R > __G)Max = __R;
  if (Max < __B)Max = __B;
  return Max;
}

uint8_t ironmanLight::GetMin(uint8_t __R , uint8_t __G , uint8_t __B)
{
  Min = __G;
  if (__R < __G) Min = __R;
  if (Min > __B) Min = __B;
  return Min ;
}

boolean ironmanLight::RGBtoHSV(uint8_t __R , uint8_t __G , uint8_t __B , float  HSV[3])
{
  if (isValid(__R) && isValid(__G) && isValid(__B))
  {
    uint8_t Max_value =  GetMax(__R , __G , __B);
    uint8_t Min_value  = GetMin(__R , __G , __B )  ;

    float H ;
    //compute H
    if (Max_value == Min_value)
      H = 0;
    else
    {
      if (Max_value == __G)
        H =  60.0 * (__B - __R) / (Max_value - Min_value) + 120;
      if (Max_value == __B)
        H =  60.0 * (__R - __G) / (Max_value - Min_value) + 240;
      if (Max_value == __R  &&  __G >= __B)
        H = 60.0 * (__G - __B) / (Max_value - Min_value);
      if (Max_value == __R  &&  __G < __B)
        H  = 60.0 * (__G - __B) / (Max_value - Min_value) + 360;
    }
    //end compute H
    //GOTO: ;
    float S;
    //compute S
    if (Max_value == 0)
      S = 0;
    else
      S = 1.0 * (Max_value - Min_value) / Max_value;
    //end compute S
    float V = Max_value / 255.0;

    HSV[0] = H;
    HSV[1]  = S;
    HSV[2] = V;
    return true;
  }
  else
  {
    return false;
  }
}

uint8_t ironmanLight::getColorA()
{
  ledcolor[0] = 1;
  ledcolor[1] = 1;
  ledcolor[2] = 1;
  getColorRaw(cachecolor, ledcolor[0], ledcolor[1], ledcolor[2], ledcolor[3], ledcolor[4], ledcolor[5]);
  if (cachecolor[0] == 0 || cachecolor[1] == 0 || cachecolor[2] == 0) {
    return 0;
  }

  RGBtoHSV(cachecolor[0], cachecolor[1], cachecolor[2], hsvcolorA);
  if (hsvcolorA[1] <= SATURATED) {
    if (hsvcolorA[2] > BRIGHT)
      aloneColor[0] = 7;
    else
      aloneColor[0] = 8;
  } else {
    if (hsvcolorA[1] > RED_S && ((hsvcolorA[0] > RED_MAX && hsvcolorA[0] <= 360) || (hsvcolorA[0] >= 0 && hsvcolorA[0] < RED_MIN))) {
      aloneColor[0] = 1;
    } else if (hsvcolorA[1] > GREEN_S && hsvcolorA[0] > GREEN_MIN && hsvcolorA[0] < GREEN_MAX) {
      aloneColor[0] = 2;
    } else if (hsvcolorA[1] > BLUE_S && hsvcolorA[0] > BLUE_MIN && hsvcolorA[0] < BLUE_MAX) {
      aloneColor[0] = 3;
    } else if (hsvcolorA[1] > YELLOW_S && hsvcolorA[0] > YELLOW_MIN && hsvcolorA[0] < YELLOW_MAX) {
      aloneColor[0] = 4;
    } else if (hsvcolorA[1] > PURPLE_S && hsvcolorA[0] > PURPLE_MIN && hsvcolorA[0] < PURPLE_MAX) {
      aloneColor[0] = 5;
    } else if (hsvcolorA[1] > CYAN_S && hsvcolorA[0] > CYAN_MIN && hsvcolorA[0] < CYAN_MAX) {
      aloneColor[0] = 6;
    }
  }
  return aloneColor[0];
}

uint8_t ironmanLight::getColorB()
{
  ledcolor[3] = 1;
  ledcolor[4] = 1;
  ledcolor[5] = 1;
  getColorRaw(cachecolor, ledcolor[0], ledcolor[1], ledcolor[2], ledcolor[3], ledcolor[4], ledcolor[5]);

  if (cachecolor[3] == 0 || cachecolor[4] == 0 || cachecolor[5] == 0) {
    return 0;
  }

  RGBtoHSV(cachecolor[3], cachecolor[4], cachecolor[5], hsvcolorB);
  if (hsvcolorB[1] <= SATURATED) {
    if (hsvcolorB[2] > BRIGHT)
      aloneColor[1] = 7;
    else
      aloneColor[1] = 8;
  } else {
    if (hsvcolorB[1] > RED_S && ((hsvcolorB[0] > RED_MAX && hsvcolorB[0] <= 360) || (hsvcolorB[0] >= 0 && hsvcolorB[0] < RED_MIN))) {
      aloneColor[1] = 1;
    } else if (hsvcolorB[1] > GREEN_S && hsvcolorB[0] > GREEN_MIN && hsvcolorB[0] < GREEN_MAX) {
      aloneColor[1] = 2;
    } else if (hsvcolorB[1] > BLUE_S && hsvcolorB[0] > BLUE_MIN && hsvcolorB[0] < BLUE_MAX) {
      aloneColor[1] = 3;
    } else if (hsvcolorB[1] > YELLOW_S && hsvcolorB[0] > YELLOW_MIN && hsvcolorB[0] < YELLOW_MAX) {
      aloneColor[1] = 4;
    } else if (hsvcolorB[1] > PURPLE_S && hsvcolorB[0] > PURPLE_MIN && hsvcolorB[0] < PURPLE_MAX) {
      aloneColor[1] = 5;
    } else if (hsvcolorB[1] > CYAN_S && hsvcolorB[0] > CYAN_MIN && hsvcolorB[0] < CYAN_MAX) {
      aloneColor[1] = 6;
    }
  }
  return aloneColor[1];
}

void ironmanLight::getColor(uint8_t *_array)
{
  getColorRaw(cachecolor);
  if (cachecolor[0] == 0 || cachecolor[1] == 0 || cachecolor[2] == 0 || cachecolor[3] == 0 || cachecolor[4] == 0 || cachecolor[5] == 0) {
    _array[0] = 0;
    _array[1] = 0;
    return;
  }
  RGBtoHSV(cachecolor[0], cachecolor[1], cachecolor[2], hsvcolorA);
  RGBtoHSV(cachecolor[3], cachecolor[4], cachecolor[5], hsvcolorB);
#ifdef HSLCOLOR
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print(cachecolor[i]);
    Serial.print(",");
  }
  Serial.print("  ");

  for (uint8_t i = 0; i < 3; i++) {
    Serial.print(hsvcolorA[i]);
    Serial.print(",");
  }
  Serial.print("  ");
  for (uint8_t i = 0; i < 3; i++) {
    Serial.print(hsvcolorB[i]);
    Serial.print(",");
  }
  Serial.print("  ");
#endif

  if (hsvcolorA[1] <= SATURATED) {
    if (hsvcolorA[2] > BRIGHT)
      _array[0] = 7;
    else
      _array[0] = 8;
  } else {
    if (hsvcolorA[1] > RED_S && ((hsvcolorA[0] > RED_MAX && hsvcolorA[0] <= 360) || (hsvcolorA[0] >= 0 && hsvcolorA[0] < RED_MIN))) {
      _array[0] = 1;
    } else if (hsvcolorA[1] > GREEN_S && hsvcolorA[0] > GREEN_MIN && hsvcolorA[0] < GREEN_MAX) {
      _array[0] = 2;
    } else if (hsvcolorA[1] > BLUE_S && hsvcolorA[0] > BLUE_MIN && hsvcolorA[0] < BLUE_MAX) {
      _array[0] = 3;
    } else if (hsvcolorA[1] > YELLOW_S && hsvcolorA[0] > YELLOW_MIN && hsvcolorA[0] < YELLOW_MAX) {
      _array[0] = 4;
    } else if (hsvcolorA[1] > PURPLE_S && hsvcolorA[0] > PURPLE_MIN && hsvcolorA[0] < PURPLE_MAX) {
      _array[0] = 5;
    } else if (hsvcolorA[1] > CYAN_S && hsvcolorA[0] > CYAN_MIN && hsvcolorA[0] < CYAN_MAX) {
      _array[0] = 6;
    }
  }

  if (hsvcolorB[1] <= SATURATED) {
    if (hsvcolorB[2] > BRIGHT)
      _array[1] = 7;
    else
      _array[1] = 8;
  } else {
    if (hsvcolorB[1] > RED_S && ((hsvcolorB[0] > RED_MAX && hsvcolorB[0] <= 360) || (hsvcolorB[0] >= 0 && hsvcolorB[0] < RED_MIN))) {
      _array[1] = 1;
    } else if (hsvcolorB[1] > GREEN_S && hsvcolorB[0] > GREEN_MIN && hsvcolorB[0] < GREEN_MAX) {
      _array[1] = 2;
    } else if (hsvcolorB[1] > BLUE_S && hsvcolorB[0] > BLUE_MIN && hsvcolorB[0] < BLUE_MAX) {
      _array[1] = 3;
    } else if (hsvcolorB[1] > YELLOW_S && hsvcolorB[0] > YELLOW_MIN && hsvcolorB[0] < YELLOW_MAX) {
      _array[1] = 4;
    } else if (hsvcolorB[1] > PURPLE_S && hsvcolorB[0] > PURPLE_MIN && hsvcolorB[0] < PURPLE_MAX) {
      _array[1] = 5;
    } else if (hsvcolorB[1] > CYAN_S && hsvcolorB[0] > CYAN_MIN && hsvcolorB[0] < CYAN_MAX) {
      _array[1] = 6;
    }
  }
}
