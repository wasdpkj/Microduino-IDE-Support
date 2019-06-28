#ifndef __MBATTERY_PMU_H__
#define __MBATTERY_PMU_H__

#include <Arduino.h>
#include <inttypes.h>

#define ADDR8_VERSION    0
#define ADDR8_LOW        1
#define ADDR8_RST        2
#define ADDR16_BAT       2

class MbaPmu{
public:
  MbaPmu();
  uint8_t begin();
  uint8_t getVersion();
  uint8_t isLow();
  void reset();
  float getVoltage(float pmu_Voltage = 3.3,float resX = 2.0);
  
protected:
  uint8_t I2C_ADDR_PMU=0x52;  
};


#endif
