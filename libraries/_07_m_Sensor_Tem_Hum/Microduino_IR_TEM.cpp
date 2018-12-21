/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  //For Microduino Sensor non-Contact Temperature sensor

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //���ڣ�2018.2
*********************************************************/
#include "Microduino_IR_TEM.h"

IR_TEM::IR_TEM() {
  _deviceAddress = 0;
  _defaultUnit = TEMP_C;
  _rawObject = 0;
  _rawAmbient = 0;
  _rawObject2 = 0;
}

bool IR_TEM::begin(uint8_t address)
{
  _deviceAddress = address;
  delay(150);
  Wire.begin();
  Wire.beginTransmission(_deviceAddress);
  bool sta = (Wire.endTransmission() == 0);
  return sta;
}
void IR_TEM::setUnit(temperature_units unit)
{
  _defaultUnit = unit;
}

uint8_t IR_TEM::read()
{
  if (readObject() && readAmbient())
  {
	  /*
	Serial.print(_rawObject);
	Serial.print("   ||   ");
	Serial.println(_rawAmbient);
	*/
    return 1;
	
  }
  return 0;
}

float IR_TEM::ambient(void)
{
  return calcTemperature(_rawAmbient);
}

float IR_TEM::object(void)
{
  return calcTemperature(_rawObject);
}

uint8_t IR_TEM::readObject()
{
  int16_t rawObj;
  if (I2CReadWord(MLX90614_REGISTER_TOBJ1, &rawObj))
  {
    if (rawObj & 0x8000) //flag error?
    {
      return 0;
    }
    _rawObject = rawObj;
	delay(2);
    return 1;
  }
  return 0;
}

uint8_t IR_TEM::readObject2()
{
  int16_t rawObj;

  if (I2CReadWord(MLX90614_REGISTER_TOBJ2, &rawObj))
  {
    if (rawObj & 0x8000) //flag error
    {
      return 0;
    }
    _rawObject2 = rawObj;
    return 1;
  }
  return 0;
}

uint8_t IR_TEM::readAmbient()
{
  int16_t rawAmb;

  if (I2CReadWord(MLX90614_REGISTER_TA, &rawAmb))
  {
    _rawAmbient = rawAmb;
	delay(2);
    return 1;
  }
  return 0;
}

uint8_t IR_TEM::setEmissivity(float emis)
{
	uint8_t sta;
	float temp_emm;
  if ((emis > 1.0) || (emis < 0.1))
  {
    return 3;
  }
  temp_emm = readEmissivity();
  
  if(abs(temp_emm - emis)<1)
  {
	  return 2;
  }else{  
  uint16_t ke = uint16_t(65535.0 * emis);
  ke = constrain(ke, 0x2000, 0xFFFF);
  sta = writeEEPROM(MLX90614_REGISTER_KE, (int16_t)ke);
  delay(10); //EEP WRITE 5MS
  return sta;
  }
}

float IR_TEM::readEmissivity(void)
{
  int16_t ke;
  if (I2CReadWord(MLX90614_REGISTER_KE, &ke))
  {
	delay(10); //EEP READ 5MS
    return (((float)((uint16_t)ke)) / 65535.0);
  }
  return 0;
}

uint8_t IR_TEM::readAddress(void)
{
  int16_t tempAdd;

  if (I2CReadWord(MLX90614_REGISTER_ADDRESS, &tempAdd))
  {
    return (uint8_t) tempAdd;
  }
  return 0;
}

uint8_t IR_TEM::setAddress(uint8_t newAdd)
{
  int16_t tempAdd;

  if ((newAdd >= 0x80) || (newAdd == 0x00))
  {
    return 0;
  }

  if (I2CReadWord(MLX90614_REGISTER_ADDRESS, &tempAdd))
  {
    tempAdd &= 0xFF00;
    tempAdd |= newAdd;
    return writeEEPROM(MLX90614_REGISTER_ADDRESS, tempAdd);
  }
  return 0;
}

uint8_t IR_TEM::readID(void)  //64位
{
  for (int i = 0; i < 4; i++)
  {
    int16_t temp = 0;
    if (!I2CReadWord(MLX90614_REGISTER_ID0 + i, &temp))
      return 0;
    id[i] = (uint16_t)temp;
  }
  return 1;
}
//run过readID之后才可以获得高位低位
uint32_t IR_TEM::getIDH(void)
{
  return ((uint32_t)id[3] << 16) | id[2];
}

uint32_t IR_TEM::getIDL(void)
{
  return ((uint32_t)id[1] << 16) | id[0];
}

int16_t IR_TEM::calcRawTemp(float calcTemp)
{
  int16_t rawTemp;

  if (_defaultUnit == TEMP_RAW)
  {
    rawTemp = (int16_t) calcTemp;
  }
  else
  {
    float tempFloat;
    //单位转换
    if (_defaultUnit == TEMP_F)
    {
      tempFloat = (calcTemp - 32.0) * 5.0 / 9.0 + 273.15;
    }
    else if (_defaultUnit == TEMP_C)
    {
      tempFloat = calcTemp + 273.15;
    }
    else if (_defaultUnit == TEMP_K)
    {
      tempFloat = calcTemp;
    }
    // Then multiply by 0.02 degK / bit
    tempFloat *= 50;
    rawTemp = (int16_t) tempFloat;
  }
  return rawTemp;
}

float IR_TEM::calcTemperature(int16_t rawTemp)
{
  float retTemp;

  if (_defaultUnit == TEMP_RAW)
  {
    retTemp = (float) rawTemp;
  }
  else
  {
    retTemp = float(rawTemp) * 0.02;
    if (_defaultUnit != TEMP_K)
    {
      retTemp -= 273.15;
      if (_defaultUnit == TEMP_F)
      {
        retTemp = retTemp * 9.0 / 5.0 + 32;
      }
    }
  }
  if(retTemp < -30 || retTemp > 100)
  {
	  return 999;
  }
  return retTemp;
}

uint8_t IR_TEM::I2CReadWord(byte reg, int16_t * dest)
{
  int timeout = I2C_READ_TIMEOUT;

  Wire.beginTransmission(_deviceAddress);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(_deviceAddress, (uint8_t) 3);

  while ((Wire.available() < 3) && (timeout-- > 0))
  {
    delay(1);
  }
  if (timeout <= 0)
  {
    return 0;
  }

  uint8_t lsb = Wire.read();
  uint8_t msb = Wire.read();
  uint8_t pec = Wire.read();

  uint8_t crc = crc8(0, (_deviceAddress << 1));
  crc = crc8(crc, reg);
  crc = crc8(crc, (_deviceAddress << 1) + 1);
  crc = crc8(crc, lsb);
  crc = crc8(crc, msb);

  if (crc == pec)
  {
    *dest = (msb << 8) | lsb;
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t IR_TEM::writeEEPROM(byte reg, int16_t data)
{

  if (I2CWriteWord(reg, 0) != 0)
    return 0; // If the write failed, return 0
  delay(5); // Delay tErase

  uint8_t i2cRet = I2CWriteWord(reg, data);
  delay(5); // Delay tWrite

  if (i2cRet == 0)
    return 1;
  else
    return 0;
}

uint8_t IR_TEM::I2CWriteWord(byte reg, int16_t data)
{
  uint8_t crc;
  uint8_t lsb = data & 0x00FF;
  uint8_t msb = (data >> 8);

  crc = crc8(0, (_deviceAddress << 1));
  crc = crc8(crc, reg);
  crc = crc8(crc, lsb);
  crc = crc8(crc, msb);

  Wire.beginTransmission(_deviceAddress);
  Wire.write(reg);
  Wire.write(lsb);
  Wire.write(msb);
  Wire.write(crc);
  return Wire.endTransmission(true);
}

uint8_t IR_TEM::crc8 (uint8_t inCrc, uint8_t inData)
{
  uint8_t i;
  uint8_t data;
  data = inCrc ^ inData;
  for ( i = 0; i < 8; i++ )
  {
    if (( data & 0x80 ) != 0 )
    {
      data <<= 1;
      data ^= 0x07;
    }
    else
    {
      data <<= 1;
    }
  }
  return data;
}
