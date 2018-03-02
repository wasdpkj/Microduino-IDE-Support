#include "mBattery_Pmu.h"
#include "I2Cdev.h"

MbaPmu::MbaPmu(){
   
}

uint8_t MbaPmu::begin(){
  Wire.begin();
  return getVersion();  
}

uint8_t MbaPmu::getVersion(){
  uint8_t version = 0;
  I2Cdev::readByte(I2C_ADDR_PMU, ADDR8_VERSION, &version);    
  return version;
}

uint8_t MbaPmu::isLow(){
  uint8_t low = 0;
  I2Cdev::readByte(I2C_ADDR_PMU, ADDR8_LOW, &low);  
  return low;  
}

void MbaPmu::reset(){   
  I2Cdev::writeByte(I2C_ADDR_PMU, ADDR8_RST, 0x01);     
}

float MbaPmu::getVoltage(){
  uint16_t batRaw = 0;
  I2Cdev::readWord(I2C_ADDR_PMU, ADDR16_BAT*2, &batRaw);     
  return batRaw*3.3*2.0/1024.0;
}



