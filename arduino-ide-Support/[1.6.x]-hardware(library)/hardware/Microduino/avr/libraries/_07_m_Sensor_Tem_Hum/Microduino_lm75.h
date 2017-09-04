/*
 Copyright © 2014 José Luis Zabalza  License LGPLv3+: GNU
 LGPL version 3 or later <http://www.gnu.org/copyleft/lgpl.html>.
 This is free software: you are free to change and redistribute it.
 There is NO WARRANTY, to the extent permitted by law.
*/

#ifndef _MICRODUINO_LM75_H_
#define _MICRODUINO_LM75_H_


#include <inttypes.h>

#define Tem_D1_ADDR_0 0x48
#define Tem_D1_ADDR_1 0x49
#define Tem_D1_ADDR_2 0x4A
#define Tem_D1_ADDR_3 0x4B
#define Tem_D1_ADDR_4 0x4C
#define Tem_D1_ADDR_5 0x4D
#define Tem_D1_ADDR_6 0x4E
#define Tem_D1_ADDR_7 0x4F

#define Tem_D1_ADDR_DEFAULT Tem_D1_ADDR_0

class Tem_D1 {
  
public: 
  
    typedef enum {nine_bits = 0, ten_bits, eleven_bits, twelve_bits } Resolution;
    typedef enum {comparator_mode=0, interrupt_mode } TermostatMode;
    typedef enum {one_samples = 0, two_samples, four_samples, six_samples } TermostatFaultTolerance;
    typedef enum {active_low = 0, active_high } OSPolarity;

    Tem_D1(uint8_t _addr = Tem_D1_ADDR_DEFAULT);

  bool begin(Resolution resolution = nine_bits);

// Temperature and temperature ranges in degrees centigrade
    float getTemperature();

    void setTHyst(float newTHyst);
    void setTOS(float newTOS);

    float getTHyst(void);
    float getTOS(void);

    TermostatMode getTermostatMode();
    void setTermostatMode(TermostatMode newMode);

    TermostatFaultTolerance getTermostatFaultTolerance();
    void setTermostatFaultTolerance(TermostatFaultTolerance newFaultTolerance);

    Resolution getResolution();
    void setResolution(Resolution newResolution);

    OSPolarity getOSPolarity();
    void setOSPolarity(OSPolarity newOSPolarity);

private:

  typedef enum {Reg_Temp = 0, Reg_Config, Reg_THyst, Reg_Tos  } LM75Register;
  
  typedef union{
    
    struct{
      uint8_t shutdown:1;
      uint8_t termostatMode:1;
      uint8_t termostatOutputPolarity:1;
      uint8_t termostatFaultTolerance:2;
      uint8_t resolution:2;
      uint8_t reserved:1;
    } mbits;

    uint8_t mbyte;
  } CfgRegister;

    uint8_t devAddr;
  
  uint8_t getReg8(LM75Register _reg);
  int16_t getReg16(LM75Register _reg);
  
};

#endif

