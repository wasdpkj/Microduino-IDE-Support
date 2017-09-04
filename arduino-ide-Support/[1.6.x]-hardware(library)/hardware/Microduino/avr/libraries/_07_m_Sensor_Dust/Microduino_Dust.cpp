// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Machine  maxing@microduino.cc
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// Microduino IDE Support：
// https://github.com/wasdpkj/Microduino-IDE-Support/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

// ==============
// E-mail:
// Machine
// maxing@microduino.cc


#include <Arduino.h>
#include "Microduino_Dust.h"


Dust::Dust(SoftwareSerial *ser) {
	commonInit();
	pmSwSerial = ser;
}

Dust::Dust(HardwareSerial *ser) {
	commonInit();
	pmHwSerial = ser;
}

void Dust::commonInit(void){
	pmSwSerial = NULL;
	pmHwSerial = NULL;
}


bool Dust::dataParse(uint8_t _input)
{
	switch(serialStep){
		case STEP_WAIT_AA:
			if(_input == STX_AA){
				serialStep = STEP_WAIT_DATA;  
				serialIndex = 0;
				serialCheckSum = 0;
			}
			break;
    
		case STEP_WAIT_DATA:
			serialData[serialIndex++] = _input;
			serialCheckSum += _input;
			if(serialIndex >= 4)
				serialStep = STEP_WAIT_SUM;       
			break;

		case STEP_WAIT_SUM:
			if(serialCheckSum == _input)
				serialStep = STEP_WAIT_FF;
			else
				serialStep = STEP_WAIT_AA;     
		break;
     
		case STEP_WAIT_FF:
			serialStep = STEP_WAIT_AA;  
			if(_input == ETX_FF)
				return 1;        
			break;
      
		default:
			serialStep = STEP_WAIT_AA;        
		break;  
	}
	return 0;  
}


bool Dust::available()
{
	uint32_t pmTimer = millis() + DUSTMAX_TIME;
	if(pmSwSerial)
	{
		pmSwSerial->begin(SERIAL_BAUD);
		pmSwSerial->flush();
		while(millis() < pmTimer)
		{
			if(pmSwSerial->available() > 0 && dataParse(pmSwSerial->read()))
			{
				pmSwSerial->end();
				return true;
			}
		}
		pmSwSerial->end();
	}
	else
	{
		pmHwSerial->begin(SERIAL_BAUD);
		pmHwSerial->flush();
		while(millis() < pmTimer)
		{
			if(pmHwSerial->available() > 0 && dataParse(pmHwSerial->read()))
			{
				pmHwSerial->end();
				return true;
			}
		}
		pmHwSerial->end();
	}
	return false;
}


float Dust::getPM25()
{
	return (serialData[0] * 256 + serialData[1]) * (5.0 / 1024) * 550; //计算PM2.5浓度，单位ug/m3		
}
