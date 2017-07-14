#include "Microduino_AM2321.h"
#include <Wire.h>

static inline uint16_t crc16(uint8_t *ptr, uint8_t len) {
    uint16_t crc = 0xFFFF; 
    while(len--) {
        crc ^= *ptr++; 
        for(uint8_t i = 0; i < 8; i++) {
            if(crc & 0x01) {
                crc >>= 1;
                crc  ^= 0xA001; 
            }
            else {
                crc >>= 1;
            } 
        }
    }
    return crc; 
}


bool Tem_Hum::readRaw(uint8_t cmd, uint8_t readAddr, uint8_t readLen, uint8_t *buf) {
    //
    // Wakeup
    //
    Wire.beginTransmission(devAddr);
    Wire.endTransmission();

    //
    // Read Command
    //
    Wire.beginTransmission(devAddr);
    Wire.write(cmd);
    Wire.write(readAddr);
    Wire.write(readLen);
    Wire.endTransmission();

    //
    // Waiting
    //
    delayMicroseconds(1600); //>1.5ms

    //
    // Read
    //
    Wire.requestFrom(devAddr, (uint8_t)(readLen + 4)); // COMMAND + REGCOUNT + DATA + CRCLSB + CRCMSB
    delayMicroseconds(60);
    uint32_t t1 = millis();
    while (Wire.available() < readLen + 4)
    {
        if(millis() - t1 > 1000)
        {
          Wire.endTransmission(); 
          return false;   
      }
  }
  for(uint8_t i=0; i < readLen + 2; i++)
    buf[i] = Wire.read();
uint16_t crc = 0;
    crc  = Wire.read();     //CRC LSB
    crc |= Wire.read() << 8;//CRC MSB

    Wire.endTransmission();
    if (crc == crc16(buf, readLen + 2))
        return true;
    return false;
}



Tem_Hum::Tem_Hum() {
  devAddr = I2C_ADDR_Tem_Hum;
}


bool Tem_Hum::begin() {
    Wire.begin();
    Wire.beginTransmission(devAddr);
    if(Wire.endTransmission()){
        return false;
    }
    return true;    
}

uint32_t Tem_Hum::getUid() {
  uint8_t buf[6];
  uint32_t uid = 0;
  if(readRaw(PARAM_Tem_Hum_READ, REG_Tem_Hum_DEVICE_ID_BIT_24_31, 4, buf)){ 
    uid  = (uint32_t)buf[2] << 24;
    uid += (uint32_t)buf[3] << 16;
    uid += (uint32_t)buf[4] << 8;
    uid += buf[5];  
}
return uid;
}


float Tem_Hum::getTemperature() {
  uint8_t buf[4];
  uint16_t temp = 0;
  if(readRaw(PARAM_Tem_Hum_READ, REG_Tem_Hum_TEMPERATURE_MSB, 2, buf)){ 
    temp = ((uint16_t)buf[2]&0x0F) << 8;
    temp += buf[3];
}
if((buf[4]&0x80) == 0x80)
    return -temp/10.0;
return temp/10.0;
}


float Tem_Hum::getHumidity() {
  uint8_t buf[4];
  uint16_t humi = 0;
  if(readRaw(PARAM_Tem_Hum_READ, REG_Tem_Hum_HUMIDITY_MSB, 2, buf)){  
    humi = buf[2] << 8;
    humi += buf[3];
}
return humi/10.0;
}

//
// END OF FILE
//
