#pragma once

#include "Arduino.h"
#include "Wire.h"

#define SHT2x_LIB_VERSION (F("0.3.0"))

//  fields getStatus
#define SHT2x_STATUS_OPEN_CIRCUIT 0x00
#define SHT2x_STATUS_TEMPERATURE 0x01
#define SHT2x_STATUS_HUMIDITY 0x02
#define SHT2x_STATUS_CLOSED_CIRCUIT 0x03

//  error codes
//  kept in sync with SHT31 library
#define SHT2x_OK 0x00
#define SHT2x_ERR_WRITECMD 0x81
#define SHT2x_ERR_READBYTES 0x82
#define SHT2x_ERR_HEATER_OFF 0x83
#define SHT2x_ERR_NOT_CONNECT 0x84
#define SHT2x_ERR_CRC_TEMP 0x85
#define SHT2x_ERR_CRC_HUM 0x86
#define SHT2x_ERR_CRC_STATUS 0x87 // not used
#define SHT2x_ERR_HEATER_COOLDOWN 0x88
#define SHT2x_ERR_HEATER_ON 0x89
//  0.2.0
#define SHT2x_ERR_RESOLUTION 0x8A

class SHT2x
{
public:
  SHT2x();

#if defined(ESP8266) || defined(ESP32)
  bool begin(const int dataPin, const int clockPin);
#endif
  bool begin(TwoWire *wire = &Wire);

  //  check sensor is reachable over I2C
  bool isConnected();

  //  read must be called get getTemperature / getHumidity
  bool read();

  float getTemperature();
  float getHumidity();
  uint16_t getRawTemperature();
  uint16_t getRawHumidity();

  //  might take up to 15 milliseconds.
  bool reset();

  //  from datasheet HTU20
  //
  //  |  bits  | value  | meaning             |
  //  |:------:|:------:|:--------------------|
  //  |  00    |   0    | open circuit        |
  //  |  01    |   1    | temperature reading |
  //  |  10    |   2    | humidity reading    |
  //  |  11    |   3    | closed circuit      |
  //
  uint8_t getStatus();

  // lastRead is in milliSeconds since start
  uint32_t lastRead();

  //
  //  HEATER
  //
  //  do not use heater for long periods,
  //  use it for max 3 minutes to heat up
  //  and let it cool down at least 3 minutes.
  void setHeatTimeout(uint8_t seconds);
  uint8_t getHeatTimeout();

  bool heatOn();
  bool heatOff();
  bool isHeaterOn(); // is the sensor still heating up?

  bool setHeaterLevel(uint8_t level);  // level = 0..15
  bool getHeaterLevel(uint8_t &level); // 0..15

  int getError(); // clears error flag

  //
  //  Electronic Identification Code
  //
  //  Sensirion_Humidity_SHT2x_Electronic_Identification_Code_V1.1.pdf
  //  Electronic ID bytes
  uint32_t getEIDA();
  uint32_t getEIDB();
  uint8_t getFirmwareVersion();

  //
  //  RESOLUTION
  //
  //  experimental 0.2.0 - needs testing.
  //  table 8 SHT20 datasheet
  //  table 7 shows different timing per resolution
  //          every level is roughly factor 2 in time.
  //  RES     HUM       TEMP
  //   0      12 bit    14  bit
  //   1      08 bit    12  bit
  //   2      10 bit    13  bit
  //   3      11 bit    11  bit
  //   4..255 returns false
  bool setResolution(uint8_t res = 0);
  //  returns RES set (cached value)
  uint8_t getResolution();

  bool requestTemperature();
  bool requestHumidity();
  bool reqTempReady();
  bool reqHumReady();
  bool readTemperature();
  bool readHumidity();
  uint32_t lastRequest();

  bool batteryOK();

protected:
  uint8_t crc8(const uint8_t *data, uint8_t len);

  bool writeCmd(uint8_t cmd);
  bool writeCmd(uint8_t cmd, uint8_t value);
  bool readBytes(uint8_t n, uint8_t *val, uint8_t maxDuration);
  TwoWire *_wire;

  uint32_t _lastRead;

  //  for async interface
  uint32_t _lastRequest;
  //  0 = none  1 = temp  2 = hum
  uint8_t _requestType;

  uint8_t _heatTimeout; //  seconds
  uint32_t _heaterStart;
  uint32_t _heaterStop;
  bool _heaterOn;

  uint16_t _rawHumidity;
  uint16_t _rawTemperature;

  uint8_t _status;

  uint8_t _error;
  uint8_t _resolution;
};

class Tem_Hum_S2 : public SHT2x
{
public:
  Tem_Hum_S2();
};

class SHT20 : public SHT2x
{
public:
  SHT20();
};

class SHT21 : public SHT2x
{
public:
  SHT21();
};

class SHT25 : public SHT2x
{
public:
  SHT25();
};

class HTU20 : public SHT2x
{
public:
  HTU20();
};

class HTU21 : public SHT2x
{
public:
  HTU21();
};

class Si7013 : public SHT2x
{
public:
  Si7013();
};

class Si7020 : public SHT2x
{
public:
  Si7020();
};

class Si7021 : public SHT2x
{
public:
  Si7021();
};

class GY21 : public SHT2x
{
public:
  GY21();
};

//  -- END OF FILE --
