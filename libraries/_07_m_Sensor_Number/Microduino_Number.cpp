#include <Arduino.h>
#include "Microduino_Number.h"

#if defined (ESP32)
Number::Number(uint8_t n, HardwareSerial *ser, int _rx, int _tx) {
  common_init();
  numHwSerial = ser;
  numLED = n;
  numLEDDir = 0;
  if ((numData = (uint8_t *)malloc(numLED * 2))) {
    memset(numData, 0, numLED * 2);
  }
  pinRX = _rx;
  pinTX = _tx;
}
#elif defined (__AVR__)
Number::Number(uint8_t n, SoftwareSerial *ser) {
  common_init();
  numSwSerial = ser;
  numLED = n;
  numLEDDir = 0;
  if ((numData = (uint8_t *)malloc(numLED * 2))) {
    memset(numData, 0, numLED * 2);
  }
}

Number::Number(uint8_t n, HardwareSerial *ser) {
  common_init();
  numHwSerial = ser;
  numLED = n;
  numLEDDir = 0;
  if ((numData = (uint8_t *)malloc(numLED * 2))) {
    memset(numData, 0, numLED * 2);
  }
}
#endif

void Number::common_init(void) {
#if defined (__AVR__)
  numSwSerial = NULL;
#endif
  numHwSerial = NULL;
}

void Number::begin() {
#if defined (__AVR__)
  if (numSwSerial)
    numSwSerial->begin(9600);
  else
    numHwSerial->begin(9600);
#elif defined (ESP32)
  numHwSerial->begin(9600, SERIAL_8N1, pinRX, pinTX);
#endif
  delay(100);
}


void Number::direction(uint8_t n) {
  numLEDDir = n;
}

byte Number::getCheckBcc(byte length, byte mydata[]) {
  byte checkbcc = 0;
  checkbcc ^= length;
  for (uint8_t i = 0; i < length; i++)
    checkbcc ^= mydata[i];
  return checkbcc;
}

void Number::Serial_Send(uint8_t* Data, uint8_t len)
{
  if (numHwSerial)
  {
    numHwSerial->write(HEADA);
    numHwSerial->write(HEADB);
    numHwSerial->write(len);
    numHwSerial->write(Data, len);
    numHwSerial->write(getCheckBcc(len, Data));
  }
#if defined (__AVR__)
  else
  {
    numSwSerial->write(HEADA);
    numSwSerial->write(HEADB);
    numSwSerial->write(len);
    numSwSerial->write(Data, len);
    numSwSerial->write(getCheckBcc(len, Data));
  }
#endif
}

void Number::setSeg(uint8_t _num, byte _data, byte _light)
{
  if (_num >= numLED)
    return;
  numData[2 * _num] &= 0x80;
  numData[2 * _num] |= _data & 0x7f;
  numData[2 * _num + 1] = _light;
}

void Number::setNumber(uint8_t _num, uint8_t _data, byte _light)
{
  if (_data < 0 || _data > 10)
    return;
  if (numLEDDir == POSITIVE)
    setSeg(_num, Positive_data[_data], _light);
  else if (numLEDDir == NEGATIVE)
    setSeg(_num, Negative_data[_data], _light);
}

void Number::setPoint(uint8_t _num, boolean _point)
{
  if (_num >= numLED)
    return;
  if (_point)
    numData[2 * _num] |= 0x80;
  else
    numData[2 * _num] &= 0x7f;
}

void Number::show()
{
  byte Tx_data[2];
  for (uint8_t i = numLED; i > 0; i--)
  {
    Tx_data[0] = numData[2 * (i - 1)];
    Tx_data[1] = numData[2 * (i - 1) + 1];
    Serial_Send(Tx_data, 2);
  }
  if (numHwSerial)
  {
    numHwSerial->write(HEADA);
    numHwSerial->write(SHOWC);
    numHwSerial->write(numLED);
  }
#if defined (__AVR__)
  else
  {
    numSwSerial->write(HEADA);
    numSwSerial->write(SHOWC);
    numSwSerial->write(numLED);
  }
#endif
}

void Number::turnOff(int8_t _num)
{
  if (_num == ALL)
  {
    for (int i = 0; i < numLED; i++)
    {
      setPoint(i, false);
      setSeg(i, 0x00, 50);
    }
  }
  else
  {
    setPoint(_num, false);
    setSeg(_num, 0x00, 50);
  }
  show();
}