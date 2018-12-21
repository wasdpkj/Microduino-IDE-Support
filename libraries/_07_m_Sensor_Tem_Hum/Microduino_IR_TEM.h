/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  //For Microduino Sensor non-Contact Temperature sensor

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //���ڣ�2018.2
*********************************************************/
#ifndef _MICRODUINO_IR_TEM_H_
#define _MICRODUINO_IR_TEM_H_

#include <Arduino.h>
#include <Wire.h>

#define MLX90614_DEFAULT_ADDRESS 0x5A

#define MLX90614_REGISTER_TA      0x06
#define MLX90614_REGISTER_TOBJ1    0x07
#define MLX90614_REGISTER_TOBJ2   0x08
#define MLX90614_REGISTER_TOMAX   0x20
#define MLX90614_REGISTER_TOMIN   0x21
#define MLX90614_REGISTER_PWMCTRL 0x22
#define MLX90614_REGISTER_TARANGE 0x23
#define MLX90614_REGISTER_KE      0x24
#define MLX90614_REGISTER_CONFIG  0x25
#define MLX90614_REGISTER_ADDRESS 0x2E
#define MLX90614_REGISTER_ID0     0x3C
#define MLX90614_REGISTER_ID1     0x3D
#define MLX90614_REGISTER_ID2     0x3E
#define MLX90614_REGISTER_ID3     0x3F
//#define MLX90614_REGISTER_SLEEP   0xFF // Not really a register, but close enough

#define I2C_READ_TIMEOUT 1000

  typedef enum temperature_units {
  TEMP_RAW,
  TEMP_K,
  TEMP_C,
  TEMP_F
};

class IR_TEM {
  public:
    IR_TEM();
    bool begin(uint8_t address = MLX90614_DEFAULT_ADDRESS);
    void setUnit(temperature_units unit);
    uint8_t read(void);
    float object(void);
    float ambient(void);
    float readEmissivity(void);
    uint8_t setEmissivity(float emis);
    uint8_t setAddress(uint8_t newAdd);
    uint8_t readAddress(void);
    uint8_t readID(void);
    uint32_t getIDH(void);
    uint32_t getIDL(void);
  private:
    uint8_t _deviceAddress; // MLX90614's 7-bit I2C address
    temperature_units _defaultUnit; // Keeps track of configured temperature unit

    // These keep track of the raw temperature values read from the sensor:
    int16_t _rawAmbient, _rawObject, _rawObject2;

    uint16_t id[4]; // Keeps track of the 64-bit ID value

    // These functions individually read the object, object2, and ambient
    // temperature values from the MLX90614's RAM:
    uint8_t readObject(void);
    uint8_t readObject2(void);
    uint8_t readAmbient(void);

    float calcTemperature(int16_t rawTemp);

    int16_t calcRawTemp(float calcTemp);
    uint8_t writeEEPROM(byte reg, int16_t data);
    uint8_t I2CReadWord(byte reg, int16_t * dest);
    uint8_t I2CWriteWord(byte reg, int16_t data);

    // It's configured to calculate the CRC using a x^8+x^2+x^1+1 poly
    uint8_t crc8 (uint8_t inCrc, uint8_t inData);
};

#endif
