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

#ifndef __MICRODUINO_DUST_H__
#define __MICRODUINO_DUST_H__

#include <SoftwareSerial.h>

#define STEP_WAIT_AA 	0  
#define STEP_WAIT_DATA 	1 
#define STEP_WAIT_SUM	2
#define STEP_WAIT_FF	3

#define STX_AA	 0xAA
#define ETX_FF	 0xFF

#define SERIAL_BAUD	2400

#define DUSTMAX_TIME	1000


//0,1,2,3,4,5,6,7,8,9
const byte Positive_data[11] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,0x00};

//0,1,2,3,4,5,6,7,8,9
const byte Negative_data[11] = {0x3f, 0x30, 0x5b, 0x79, 0x74, 0x6d, 0x6f, 0x38, 0x7f, 0x7d,0x00};

class Dust{
public:
	Dust(SoftwareSerial *ser);
	Dust(HardwareSerial *ser);
	
	bool available();
	float getPM25();

private:
	uint8_t serialStep = STEP_WAIT_AA;
	uint8_t serialIndex;
	uint8_t serialCheckSum;
	uint8_t serialData[4];

	SoftwareSerial *pmSwSerial;
	HardwareSerial *pmHwSerial;

	void commonInit(void);
	bool dataParse(uint8_t _input);
};

#endif