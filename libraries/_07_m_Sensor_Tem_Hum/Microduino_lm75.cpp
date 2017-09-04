/*
 Copyright © 2014 José Luis Zabalza  License LGPLv3+: GNU
 LGPL version 3 or later <http://www.gnu.org/copyleft/lgpl.html>.
 This is free software: you are free to change and redistribute it.
 There is NO WARRANTY, to the extent permitted by law.
*/


#include <Wire.h>
#include <I2Cdev.h>
#include "Microduino_lm75.h"
#include <Arduino.h>


//-------------------------------------------------------------------------------
Tem_D1::Tem_D1(uint8_t _addr)
{
    devAddr = _addr;
}

//-------------------------------------------------------------------------------
bool Tem_D1::begin(Resolution resolution)
{
  Wire.begin();
    Wire.beginTransmission(devAddr);
    if(Wire.endTransmission()){
        return false;
    }
    setResolution(resolution);
    return true;    
}

//-------------------------------------------------------------------------------
float Tem_D1::getTemperature()
{
    return (getReg16(Reg_Temp) / 256.0F);
}


//-------------------------------------------------------------------------------
uint8_t Tem_D1::getReg8(LM75Register _reg)
{
    uint8_t result8;

  I2Cdev::readByte(devAddr, _reg, &result8);

    return result8;
}


//-------------------------------------------------------------------------------
int16_t Tem_D1::getReg16(LM75Register _reg)
{
    int16_t result16;

  I2Cdev::readWord(devAddr, _reg, (uint16_t *)&result16);

    return result16;
}

//-------------------------------------------------------------------------------
void Tem_D1::setTHyst(float newTHyst)
{
    int16_t buf = int16_t(newTHyst * 256);
//    setReg(THyst_reg,int(newTHyst * 256));
  I2Cdev::writeWord(devAddr, Reg_THyst, *((uint16_t *)&buf));
}

//-------------------------------------------------------------------------------
void Tem_D1::setTOS(float newTOS)
{
  int16_t buf = int16_t(newTOS * 256);
//    setReg(TOS_reg,int(newTOS * 256) );
  I2Cdev::writeWord(devAddr, Reg_Tos, *((uint16_t *)&buf));
}

//-------------------------------------------------------------------------------
float Tem_D1::getTHyst(void)
{
    return (int(getReg16(Reg_THyst)) / 256.0F);
}

//-------------------------------------------------------------------------------
float Tem_D1::getTOS(void)
{
    return (int(getReg16(Reg_Tos)) / 256.0F);
}

//-------------------------------------------------------------------------------
Tem_D1::TermostatMode Tem_D1::getTermostatMode()
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);

    return(TermostatMode(regv.mbits.termostatMode));
}

//-------------------------------------------------------------------------------
void Tem_D1::setTermostatMode(Tem_D1::TermostatMode newMode)
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);
    regv.mbits.termostatMode = newMode;

//    setReg(config_reg,unsigned(regv.mbyte));
  I2Cdev::writeByte(devAddr, Reg_Config, uint8_t(regv.mbyte));
}

//-------------------------------------------------------------------------------
Tem_D1::TermostatFaultTolerance Tem_D1::getTermostatFaultTolerance()
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);

    return (TermostatFaultTolerance(regv.mbits.termostatFaultTolerance));
}

//-------------------------------------------------------------------------------
void Tem_D1::setTermostatFaultTolerance(TermostatFaultTolerance newFaultTolerance)
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);
    regv.mbits.termostatFaultTolerance = newFaultTolerance;

 //   setReg(config_reg,unsigned(regv.mbyte));
  I2Cdev::writeByte(devAddr, Reg_Config, uint8_t(regv.mbyte));
}

//-------------------------------------------------------------------------------
Tem_D1::Resolution Tem_D1::getResolution()
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);

    return (Resolution(regv.mbits.resolution));
}
//-------------------------------------------------------------------------------
void Tem_D1::setResolution(Resolution newResolution)
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);
    regv.mbits.resolution = newResolution;

 //   setReg(config_reg,unsigned(regv.mbyte));
  I2Cdev::writeByte(devAddr, Reg_Config, uint8_t(regv.mbyte));
}

//-------------------------------------------------------------------------------
Tem_D1::OSPolarity Tem_D1::getOSPolarity()
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);

    return (OSPolarity(regv.mbits.termostatOutputPolarity));
  
}

//-------------------------------------------------------------------------------
void Tem_D1::setOSPolarity(OSPolarity newOSPolarity)
{
    CfgRegister regv;

    regv.mbyte = getReg8(Reg_Config);
    regv.mbits.termostatOutputPolarity = newOSPolarity;

//   setReg(config_reg,unsigned(regv.mbyte));
  I2Cdev::writeByte(devAddr, Reg_Config, uint8_t(regv.mbyte));
}

