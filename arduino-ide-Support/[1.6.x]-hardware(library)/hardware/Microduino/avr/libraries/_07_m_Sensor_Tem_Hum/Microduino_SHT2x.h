#ifndef _MICRODUINO_Tem_Hum_S2_H_
#define _MICRODUINO_Tem_Hum_S2_H_
/*
 * Tem_Hum_S2.h
 *
 * Author: Shenyang
 * Created: 2016/11/07
 */

#include <Arduino.h>
#include <math.h>

#define  Tem_Hum_S2_TEMP_HOLD     0b11100011
#define Tem_Hum_S2_HUMI_HOLD   0b11100101
#define Tem_Hum_S2_TEMP_UNHOLD 0b11110011
#define Tem_Hum_S2_HUMI_UNHOLD 0b11110101
#define Tem_Hum_S2_WRITE_USER  0b11100110
#define Tem_Hum_S2_READ_USER   0b11100111
#define Tem_Hum_S2_SOFT_RESET  0b11111110

#define Tem_Hum_S2_ADDR_DEFAULT  0x40


class Tem_Hum_S2 {
  public:

    typedef enum {
    RH_12_T_14 = 0b00000000, 
    RH_8_T_12  = 0b00000000, 
    RH_10_T_13 = 0b10000000, 
    RH_11_T_11 = 0b10000001 
  } Resolution;

    bool begin(Resolution resolution = RH_12_T_14);
    void reset(void);

    float getTemperature();
    float getHumidity();

    void setResolution(Resolution resolution);
    void heaterOn(void);
    void heaterOff(void);
    void otpReloadOff(void);
    void otpReloadOn(void);
    bool isEOD(void); 
  
  private:
 
    const uint8_t devAddr = Tem_Hum_S2_ADDR_DEFAULT;

    inline float convertStToTemperature(uint16_t st) {
      return -46.85 + 175.72 / 65536.0 * (float)st;
    }

    inline float convertSrhToHumidity(uint16_t srh) {
      return -6.0 + 125.0 / 65536.0 * (float)srh;
    }

    uint16_t readRaw(uint8_t _reg);
};

#endif
