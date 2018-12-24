#include <Arduino.h>
#include "BittyBuggy.h"

#if defined(ESP32)
BittyBuggy::BittyBuggy(HardwareSerial *ser, int _rx, int _tx)
{
  common_init();
  carHwSerial = ser;
  uartRxstep = STEP_WAIT_AA;
  pinRX = _rx;
  pinTX = _tx;
}
#elif defined(__AVR__)
BittyBuggy::BittyBuggy(SoftwareSerial *ser)
{
  common_init();
  carSwSerial = ser;
  uartRxstep = STEP_WAIT_AA;
}

BittyBuggy::BittyBuggy(HardwareSerial *ser)
{
  common_init();
  carHwSerial = ser;
  uartRxstep = STEP_WAIT_AA;
}
#endif

void BittyBuggy::common_init(void)
{
#if defined(__AVR__)
  carSwSerial = NULL;
#endif
  carHwSerial = NULL;
}

boolean BittyBuggy::begin(uint32_t _baud)
{
#if defined(ESP32)
  carHwSerial->begin(_baud, SERIAL_8N1, pinRX, pinTX);
#elif defined(__AVR__)
  if (carHwSerial)
  {
    carHwSerial->begin(_baud);
  }
  else if (carSwSerial)
  {
    carSwSerial->begin(_baud);
  }
#endif
  delay(100);
  if (connectionErr())
  {
    return false;
  }
  setAllLED(0);
  setSpeed(0, 0);
  noTone();
  return true;
}

uint8_t BittyBuggy::getChecksum(uint8_t _cmd, uint8_t *_data, uint8_t _len)
{
  uint8_t checksum = 0;
  checksum ^= _cmd;
  checksum ^= _len;
  for (uint8_t i = 0; i < _len; i++)
    checksum ^= _data[i];
  return checksum;
}

boolean BittyBuggy::parse(uint8_t _inByte)
{
  if (uartRxstep == STEP_WAIT_AA)
  { //等待接收0x7A
    if (_inByte == HEADA)
      uartRxstep = STEP_WAIT_BB;
  }
  else if (uartRxstep == STEP_WAIT_BB)
  {
    if (_inByte == HEADB)
    { //等待接收0x7B
      uartRxstep = STEP_SET_CMD;
    }
    else if (_inByte == HEADC)
    { //等待接收0x7C
      uartRxstep = STEP_WAIT_CMD;
    }
    else
    {
      uartRxstep = STEP_WAIT_AA;
    }
  }
  else if (uartRxstep == STEP_WAIT_CMD)
  { //等待接收get命令
    datacmd = _inByte;
    uartRxstep = STEP_WAIT_LEN;
  }
  else if (uartRxstep == STEP_WAIT_LEN)
  { //等待接收get长度
    datalen = _inByte;
    uartRxindex = 0;
    uartRxstep = STEP_WAIT_DATA;
  }
  else if (uartRxstep == STEP_WAIT_DATA)
  { //等待接收get数据
    uartdata[uartRxindex++] = _inByte;
    if (uartRxindex >= datalen)
    {
      uartRxstep = STEP_WAIT_BCC;
    }
  }
  else if (uartRxstep == STEP_WAIT_BCC)
  { //等待校验get数据
    uartRxstep = STEP_WAIT_AA;
    if (getChecksum(datacmd, uartdata, datalen) == _inByte)
    {
      return true;
    }
  }
  else if (uartRxstep == STEP_SET_CMD)
  { //等待set命令
    if (_inByte == MOTORCMD || _inByte == BUZZERCMD || _inByte == LINEFINDERCMD || _inByte == COLORDISCERNCMD || _inByte == SETCOLORCMD || _inByte == SETBAUDCMD)
    {
      uartRxstep = STEP_SET_RES;
    }
    else
    {
      uartRxstep = STEP_WAIT_AA;
    }
  }
  else if (uartRxstep == STEP_SET_RES)
  { //判断set结果
    uartRxstep = STEP_WAIT_AA;
    if (_inByte)
    {
      return true;
    }
  }
  return false;
}

boolean BittyBuggy::available()
{
  uint8_t inChar = 0x00;
  if (carHwSerial)
  {
    if (carHwSerial->available() > 0)
    {
      inChar = carHwSerial->read();
      if (parse(inChar))
      {
        return true;
      }
    }
  }
#if defined(__AVR__)
  else
  {
    if (carSwSerial->available() > 0)
    {
      inChar = carSwSerial->read();
      if (parse(inChar))
      {
        return true;
      }
    }
    return false;
  }
#endif
}

void BittyBuggy::readBytes(uint8_t *_data, uint8_t _len)
{
  memcpy(_data, uartdata, _len);
}

void BittyBuggy::readWords(int16_t *_data, uint8_t _len)
{
  readBytes((uint8_t *)_data, _len * 2);
}

void BittyBuggy::write(uint8_t cmd, uint8_t *_data, uint8_t _len)
{
  if (carHwSerial)
  {
    carHwSerial->write(HEADA);
    carHwSerial->write(HEADB);
    carHwSerial->write(cmd);
    carHwSerial->write(_len);
    carHwSerial->write(_data, _len);
    carHwSerial->write(getChecksum(cmd, _data, _len));
  }
#if defined(__AVR__)
  else
  {
    carSwSerial->write(HEADA);
    carSwSerial->write(HEADB);
    carSwSerial->write(cmd);
    carSwSerial->write(_len);
    carSwSerial->write(_data, _len);
    carSwSerial->write(getChecksum(cmd, _data, _len));
  }
#endif
}

void BittyBuggy::getData(uint8_t _cmd)
{
  if (carHwSerial)
  {
    carHwSerial->write(HEADA);
    carHwSerial->write(HEADC);
    carHwSerial->write(_cmd);
  }
#if defined(__AVR__)
  else
  {
    carSwSerial->write(HEADA);
    carSwSerial->write(HEADC);
    carSwSerial->write(_cmd);
  }
#endif
}

void BittyBuggy::setColorLED(uint8_t _cmd, uint8_t ar, uint8_t ag, uint8_t ab, uint8_t br, uint8_t bg, uint8_t bb)
{
  if (ar > 255)
    ar = 255;
  if (ag > 255)
    ag = 255;
  if (ab > 255)
    ab = 255;
  if (br > 255)
    br = 255;
  if (bg > 255)
    bg = 255;
  if (bb > 255)
    bb = 255;
  colorledData[0] = ar;
  colorledData[1] = ag;
  colorledData[2] = ab;
  colorledData[3] = br;
  colorledData[4] = bg;
  colorledData[5] = bb;
  last_time = millis();
  write(_cmd, colorledData, 6);
  while (!available())
  {
    connectionSta = 0;
    if (millis() - last_time > TIMEOUT)
    {
      connectionSta = _cmd;
      last_time = millis();
      break;
    }
  }
}

void BittyBuggy::setColorLEDA(uint8_t ar, uint8_t ag, uint8_t ab)
{
  colorledDataCache[0] = ar;
  colorledDataCache[1] = ag;
  colorledDataCache[2] = ab;
  setColorLED(SETCOLORCMD, ar, ag, ab, colorledDataCache[3], colorledDataCache[4], colorledDataCache[5]);
}

void BittyBuggy::setColorLEDB(uint8_t br, uint8_t bg, uint8_t bb)
{
  colorledDataCache[3] = br;
  colorledDataCache[4] = bg;
  colorledDataCache[5] = bb;
  setColorLED(SETCOLORCMD, colorledDataCache[0], colorledDataCache[1], colorledDataCache[2], br, bg, bb);
}

void BittyBuggy::setColorLEDA(uint32_t _rgb)
{
  colorledDataCache[0] = _rgb >> 16;
  colorledDataCache[1] = _rgb >> 8 & 0xFF;
  colorledDataCache[2] = _rgb & 0xFF;
  setColorLED(SETCOLORCMD, colorledDataCache[0], colorledDataCache[1], colorledDataCache[2], colorledDataCache[3], colorledDataCache[4], colorledDataCache[5]);
}

void BittyBuggy::setColorLEDB(uint32_t _rgb)
{
  colorledDataCache[3] = _rgb >> 16;
  colorledDataCache[4] = _rgb >> 8 & 0xFF;
  colorledDataCache[5] = _rgb & 0xFF;
  setColorLED(SETCOLORCMD, colorledDataCache[0], colorledDataCache[1], colorledDataCache[2], colorledDataCache[3], colorledDataCache[4], colorledDataCache[5]);
}

void BittyBuggy::setAllLED(uint32_t _rgb)
{
  setColorLEDA(_rgb);
  setColorLEDB(_rgb);
}

void BittyBuggy::setAllLED(uint8_t ar, uint8_t ag, uint8_t ab)
{
  setColorLEDA(ar, ag, ab);
  setColorLEDB(ar, ag, ab);
}

void BittyBuggy::setSpeed(int16_t left, int16_t right)
{
  if (left != BRAKE)
  {
    if (left < -MAXSPEED)
      left = -MAXSPEED;
    if (left > MAXSPEED)
      left = MAXSPEED;
    if (left > 0)
      left = map(left, 1, MAXSPEED, MINSPEED, MAXSPEED);
    else if (left < 0)
      left = map(left, -1, -MAXSPEED, -MINSPEED, -MAXSPEED);
      motorspeed[0]=left;
    carmotorData[2] = (-left) & 0xFF;
    carmotorData[3] = (-left) >> 8;
  } else {
    carmotorData[2] = left & 0xFF;
    carmotorData[3] = left >> 8;
  }

  if (right != BRAKE)
  {
    if (right < -MAXSPEED)
      right = -MAXSPEED;
    if (right > MAXSPEED)
      right = MAXSPEED;
    if (right > 0)
      right = map(right, 1, MAXSPEED, MINSPEED, MAXSPEED);
    else if (right < 0)
      right = map(right, -1, -MAXSPEED, -MINSPEED, -MAXSPEED);
      motorspeed[1]=right;
  }
  carmotorData[0] = right & 0xFF;
  carmotorData[1] = right >> 8;

  last_time = millis();
  write(MOTORCMD, carmotorData, 4);
  while (!available())
  {
    connectionSta = 0;
    if (millis() - last_time > TIMEOUT)
    {
      connectionSta = MOTORCMD;
      last_time = millis();
      break;
    }
  }
}

void BittyBuggy::setSpeedA(int16_t left)
{
  motorspeed[0] = left;
  setSpeed(motorspeed[0], motorspeed[1]);
}

void BittyBuggy::setSpeedB(int16_t right)
{
  motorspeed[1] = right;
  setSpeed(motorspeed[0], motorspeed[1]);
}

void BittyBuggy::tone(int16_t _fre, int16_t _time)
{
  if (_fre < MINFRE || _fre > MAXFRE)
    _fre = 0;
  buzzerData[0] = _fre & 0xFF;
  buzzerData[1] = _fre >> 8;
  buzzerData[2] = _time & 0xFF;
  buzzerData[3] = _time >> 8;
  last_time = millis();
  write(BUZZERCMD, buzzerData, 4);
  while (!available())
  {
    connectionSta = 0;
    if (millis() - last_time > TIMEOUT)
    {
      connectionSta = BUZZERCMD;
      last_time = millis();
      break;
    }
  }
}

void BittyBuggy::tone(int16_t _fre)
{
  if (_fre < 20) {
    tone(0, 0);
  } else {
    tone(_fre, 0xFFFF);
  }
}

void BittyBuggy::noTone()
{
  tone(0, 0);
}

uint8_t BittyBuggy::getVersion()
{
  last_time = millis();
  getData(VERSIONCMD);
  while (!available())
  {
    connectionSta = 0;
    if (millis() - last_time > TIMEOUT)
    {
      connectionSta = VERSIONCMD;
      last_time = millis();
      break;
    }
  }

  if (!connectionSta)
  {
    readBytes(&version, VERSION_LEN);
  }
  else
  {
    version = 0;
  }

  return version;
}

void BittyBuggy::getLineAlone(uint8_t *_array, uint8_t _colorA, uint8_t _colorB)
{
  memset(colorledline, 0, 6);
  if (_colorA != BLACK)
  {
    colorledline[_colorA - 1] = 255;
  }
  if (_colorB != BLACK)
  {
    colorledline[3 + _colorB - 1] = 255;
  }

  setColorLED(LINEFINDERCMD, colorledline[0], colorledline[1], colorledline[2], colorledline[3], colorledline[4], colorledline[5]);

  last_time = millis();
  getData(GETLINECMD);
  while (!available())
  {
    connectionSta = 0;
    if (millis() - last_time > TIMEOUT)
    {
      connectionSta = GETLINECMD;
      last_time = millis();
      break;
    }
  }
  if (!connectionSta)
  {
    readBytes(_array, DATA_RXLINE_LEN);
  }
}

void BittyBuggy::getLine(uint8_t *_array, uint8_t _color)
{
  getLineAlone(_array, _color, _color);
}

uint8_t BittyBuggy::getLineA(uint8_t _color)
{
  setlineLED[0] = _color;
  getLineAlone(linefilter, setlineLED[0], setlineLED[1]);
  return linefilter[0];
}

uint8_t BittyBuggy::getLineB(uint8_t _color)
{
  setlineLED[1] = _color;
  getLineAlone(linefilter, setlineLED[0], setlineLED[1]);
  return linefilter[1];
}

void BittyBuggy::getLineLowpassFilter(uint8_t *_array, uint8_t _color, float _num)
{
  getLine(linefilter, _color);
  _array[0] += (linefilter[0] - _array[0]) * _num;
  _array[1] += (linefilter[1] - _array[1]) * _num;
}

void BittyBuggy::getLineAverageFilter(uint8_t *_array, uint8_t _color, uint8_t len)
{
  uint32_t filter_sum[2];
  for (uint8_t i = 0; i < len; i++)
  {
    getLine(linefilter, _color);
    filter_sum[0] += linefilter[0];
    filter_sum[1] += linefilter[1];
  }
  _array[0] = filter_sum[0] / len;
  _array[1] = filter_sum[1] / len;
}

void BittyBuggy::getColorRaw(uint8_t *_array, uint8_t _ar, uint8_t _ag, uint8_t _ab, uint8_t _br, uint8_t _bg, uint8_t _bb)
{
  setColorLED(COLORDISCERNCMD, _ar, _ag, _ab, _br, _bg, _bb);
  getData(GETCOLORCMD);
  while (!available())
  {
    connectionSta = 0;
    if (millis() - last_time > TIMEOUT)
    {
      connectionSta = GETCOLORCMD;
      last_time = millis();
      break;
    }
  }
  if (!connectionSta)
  {
    readBytes(_array, DATA_RXCOLOR_LEN);
  }
}

uint8_t BittyBuggy::isValid(uint8_t value)
{
  if (value >= 0 && value <= 255)
    return 1;
  return 0;
}

uint8_t BittyBuggy::GetMax(uint8_t __R , uint8_t __G , uint8_t __B)
{
  Max = __G;
  if (__R > __G)Max = __R;
  if (Max < __B)Max = __B;
  return Max;
}

uint8_t BittyBuggy::GetMin(uint8_t __R , uint8_t __G , uint8_t __B)
{
  Min = __G;
  if (__R < __G) Min = __R;
  if (Min > __B) Min = __B;
  return Min ;
}

boolean BittyBuggy::RGBtoHSV(uint8_t __R , uint8_t __G , uint8_t __B , float  HSV[3])
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

void BittyBuggy::getColor(uint8_t *_array)
{
  getColorRaw(cachecolor);
  if (!(cachecolor[0] && cachecolor[1] && cachecolor[2] && cachecolor[3] && cachecolor[4] && cachecolor[5])) {
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

uint8_t BittyBuggy::getColorA()
{
  ledcolor[0] = 255;
  ledcolor[1] = 255;
  ledcolor[2] = 255;
  getColorRaw(cachecolor, ledcolor[0], ledcolor[1], ledcolor[2], ledcolor[3], ledcolor[4], ledcolor[5]);
  if (!(cachecolor[0] && cachecolor[1] && cachecolor[2])) {
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

uint8_t BittyBuggy::getColorB()
{
  ledcolor[3] = 255;
  ledcolor[4] = 255;
  ledcolor[5] = 255;
  getColorRaw(cachecolor, ledcolor[0], ledcolor[1], ledcolor[2], ledcolor[3], ledcolor[4], ledcolor[5]);
  if (!(cachecolor[3] && cachecolor[4] && cachecolor[5])) {
    aloneColor[1] = 0;
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

uint8_t BittyBuggy::connectionErr()
{
  if (millis() - last_time > 10)
  {
    getVersion();
    last_time = millis();
  }
  return connectionSta;
}
