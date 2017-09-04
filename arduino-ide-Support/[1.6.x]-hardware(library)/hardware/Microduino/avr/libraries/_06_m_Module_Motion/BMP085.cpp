// I2Cdev library collection - BMP085 I2C device class
// Based on register information stored in the I2Cdevlib internal database
//

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Shenyang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "BMP085.h"
#include <I2Cdev.h>
#include <Wire.h>

/**
 * Specific address constructor.
 * @param address Specific device address
 * @see BMP085_DEFAULT_ADDRESS
 */
BMP085::BMP085(uint8_t _addr) {
    devAddr = _addr;
}

/**
 * Verify the device is connected and available.
 */
bool BMP085::testConnection() {
    // test for a response, though this is very basic
	uint8_t buffer;
    return I2Cdev::readByte(devAddr, BMP085_RA_AC1_H, &buffer) == 1;
}


bool BMP085::begin(uint8_t _mode){
	Wire.begin();
	if(!testConnection())
		return false;
    loadCalibration();
	devStatus = STATUS_START_TEM;
	measureMode = _mode;
	return true;
}


/* calibration register methods */

void BMP085::loadCalibration() {
    uint8_t buf2[22];
    I2Cdev::readBytes(devAddr, BMP085_RA_AC1_H, 22, buf2);
    ac1 = ((int16_t)buf2[0] << 8) + buf2[1];
    ac2 = ((int16_t)buf2[2] << 8) + buf2[3];
    ac3 = ((int16_t)buf2[4] << 8) + buf2[5];
    ac4 = ((uint16_t)buf2[6] << 8) + buf2[7];
    ac5 = ((uint16_t)buf2[8] << 8) + buf2[9];
    ac6 = ((uint16_t)buf2[10] << 8) + buf2[11];
    b1 = ((int16_t)buf2[12] << 8) + buf2[13];
    b2 = ((int16_t)buf2[14] << 8) + buf2[15];
    mb = ((int16_t)buf2[16] << 8) + buf2[17];
    mc = ((int16_t)buf2[18] << 8) + buf2[19];
    md = ((int16_t)buf2[20] << 8) + buf2[21];
    calibrationLoaded = true;
}

#ifdef BMP085_INCLUDE_INDIVIDUAL_CALIBRATION_ACCESS
    int16_t BMP085::getAC1() {
        if (calibrationLoaded) return ac1;
        I2Cdev::readBytes(devAddr, BMP085_RA_AC1_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getAC2() {
        if (calibrationLoaded) return ac2;
        I2Cdev::readBytes(devAddr, BMP085_RA_AC2_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getAC3() {
        if (calibrationLoaded) return ac3;
        I2Cdev::readBytes(devAddr, BMP085_RA_AC3_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    uint16_t BMP085::getAC4() {
        if (calibrationLoaded) return ac4;
        I2Cdev::readBytes(devAddr, BMP085_RA_AC4_H, 2, buffer);
        return ((uint16_t)buffer[1] << 8) + buffer[0];
    }

    uint16_t BMP085::getAC5() {
        if (calibrationLoaded) return ac5;
        I2Cdev::readBytes(devAddr, BMP085_RA_AC5_H, 2, buffer);
        return ((uint16_t)buffer[1] << 8) + buffer[0];
    }

    uint16_t BMP085::getAC6() {
        if (calibrationLoaded) return ac6;
        I2Cdev::readBytes(devAddr, BMP085_RA_AC6_H, 2, buffer);
        return ((uint16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getB1() {
        if (calibrationLoaded) return b1;
        I2Cdev::readBytes(devAddr, BMP085_RA_B1_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getB2() {
        if (calibrationLoaded) return b2;
        I2Cdev::readBytes(devAddr, BMP085_RA_B2_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getMB() {
        if (calibrationLoaded) return mb;
        I2Cdev::readBytes(devAddr, BMP085_RA_MB_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getMC() {
        if (calibrationLoaded) return mc;
        I2Cdev::readBytes(devAddr, BMP085_RA_MC_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }

    int16_t BMP085::getMD() {
        if (calibrationLoaded) return md;
        I2Cdev::readBytes(devAddr, BMP085_RA_MD_H, 2, buffer);
        return ((int16_t)buffer[1] << 8) + buffer[0];
    }
#endif

/* control register methods */

uint8_t BMP085::getControl() {
	uint8_t buffer;
    I2Cdev::readByte(devAddr, BMP085_RA_CONTROL, &buffer);
    return buffer;
}

void BMP085::setControl(uint8_t value) {
    I2Cdev::writeByte(devAddr, BMP085_RA_CONTROL, value);
}

/* measurement register methods */

uint16_t BMP085::readMeasurement2() {
	uint8_t buffer[2];
    I2Cdev::readBytes(devAddr, BMP085_RA_MSB, 2, buffer);
    return ((uint16_t)buffer[0] << 8) + buffer[1];
}

uint32_t BMP085::readMeasurement3() {
	uint8_t buffer[3];
    I2Cdev::readBytes(devAddr, BMP085_RA_MSB, 3, buffer);
    return ((uint32_t)buffer[0] << 16) + ((uint16_t)buffer[1] << 8) + buffer[2];
}

uint8_t BMP085::getMeasureDelayMilliseconds(uint8_t _mode) {
    if (measureMode == BMP085_MODE_TEMPERATURE) return 5;
    else if (measureMode == BMP085_MODE_PRESSURE_0) return 5;
    else if (measureMode == BMP085_MODE_PRESSURE_1) return 8;
    else if (measureMode == BMP085_MODE_PRESSURE_2) return 14;
    else if (measureMode == BMP085_MODE_PRESSURE_3) return 26;
    return 0; // invalid mode
}

uint16_t BMP085::getMeasureDelayMicroseconds(uint8_t _mode) {
    if (measureMode == BMP085_MODE_TEMPERATURE) return 4500;
    else if (measureMode == BMP085_MODE_PRESSURE_0) return 4500;
    else if (measureMode == BMP085_MODE_PRESSURE_1) return 7500;
    else if (measureMode == BMP085_MODE_PRESSURE_2) return 13500;
    else if (measureMode == BMP085_MODE_PRESSURE_3) return 25500;
    return 0; // invalid mode
}

uint16_t BMP085::readRawTemperature() {
    return readMeasurement2();
}

float BMP085::readTemperature() {
    /*
    Datasheet formula:
        UT = raw temperature
        X1 = (UT - AC6) * AC5 / 2^15
        X2 = MC * 2^11 / (X1 + MD)
        B5 = X1 + X2
        T = (B5 + 8) / 2^4
    */
    int32_t ut = readRawTemperature();
    int32_t x1 = ((ut - (int32_t)ac6) * (int32_t)ac5) >> 15;
    int32_t x2 = ((int32_t)mc << 11) / (x1 + md);
    b5 = x1 + x2;
    return (float)((b5 + 8) >> 4) / 10.0f;
}

uint32_t BMP085::readRawPressure() {
    if (measureMode & 0x34) return readMeasurement3() >> (8 - ((measureMode & 0xC0) >> 6));
    return 0; // wrong measurement mode for pressure request
}

float BMP085::readPressure() {
    /*
    Datasheet forumla
        UP = raw pressure
        B6 = B5 - 4000
        X1 = (B2 * (B6 * B6 / 2^12)) / 2^11
        X2 = AC2 * B6 / 2^11
        X3 = X1 + X2
        B3 = ((AC1 * 4 + X3) << oss + 2) / 4
        X1 = AC3 * B6 / 2^13
        X2 = (B1 * (B6 * B6 / 2^12)) / 2^16
        X3 = ((X1 + X2) + 2) / 2^2
        B4 = AC4 * (unsigned long)(X3 + 32768) / 2^15
        B7 = ((unsigned long)UP - B3) * (50000 >> oss)
        if (B7 < 0x80000000) { p = (B7 * 2) / B4 }
        else { p = (B7 / B4) * 2 }
        X1 = (p / 2^8) * (p / 2^8)
        X1 = (X1 * 3038) / 2^16
        X2 = (-7357 * p) / 2^16
        p = p + (X1 + X2 + 3791) / 2^4
    */
    uint32_t up = readRawPressure();
    uint8_t oss = (measureMode & 0xC0) >> 6;
    int32_t p;
    int32_t b6 = b5 - 4000;
    int32_t x1 = ((int32_t)b2 * ((b6 * b6) >> 12)) >> 11;
    int32_t x2 = ((int32_t)ac2 * b6) >> 11;
    int32_t x3 = x1 + x2;
    int32_t b3 = ((((int32_t)ac1 * 4 + x3) << oss) + 2) >> 2;
    x1 = ((int32_t)ac3 * b6) >> 13;
    x2 = ((int32_t)b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    uint32_t b4 = ((uint32_t)ac4 * (uint32_t)(x3 + 32768)) >> 15;
    uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> oss);
    if (b7 < 0x80000000) {
        p = (b7 << 1) / b4;
    } else {
        p = (b7 / b4) << 1;
    }
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    return p + ((x1 + x2 + (int32_t)3791) >> 4);
}


bool BMP085::available(){
	switch(devStatus){
		case STATUS_START_TEM:
			setControl(BMP085_MODE_TEMPERATURE);
			lastMillis = millis();
			delayTime = getMeasureDelayMilliseconds(BMP085_MODE_TEMPERATURE);
			devStatus = STATUS_GETTING_TEM;
			break;
		case STATUS_GETTING_TEM:
			if(millis() - lastMillis > delayTime){
				temperatureBuf = readTemperature();
				temperatureFlag = true;
				if(measureMode == BMP085_MODE_PRESSURE_0||
				   measureMode == BMP085_MODE_PRESSURE_1||
				   measureMode == BMP085_MODE_PRESSURE_2||
				   measureMode == BMP085_MODE_PRESSURE_3){
					setControl(measureMode);
					lastMillis = millis();
					delayTime = getMeasureDelayMilliseconds(measureMode);
					devStatus = STATUS_GETTING_PRE;
				}else if(measureMode == BMP085_MODE_TEMPERATURE){
					setControl(measureMode);
					lastMillis = millis();
					delayTime = getMeasureDelayMilliseconds(BMP085_MODE_TEMPERATURE);
				}
			}
		case STATUS_GETTING_PRE:
			if(millis() - lastMillis > delayTime){
				pressureBuf = readPressure();
				pressureFlag = true;
				setControl(BMP085_MODE_TEMPERATURE);
				lastMillis = millis();
				delayTime = getMeasureDelayMilliseconds(BMP085_MODE_TEMPERATURE);
				devStatus = STATUS_GETTING_TEM;
			}
	}
	return (measureMode == BMP085_MODE_TEMPERATURE)?  temperatureFlag : pressureFlag;
}

float BMP085::getTemperature(){
	if(temperatureFlag){
		temperatureFlag = false;
		return temperatureBuf;
	}
	return 0.0;
}

float BMP085::getTemperatureF() {
    return getTemperature() * 9.0f / 5.0f + 32;
}

float BMP085::getPressure(){
	if(pressureFlag){
		pressureFlag = false;
		return pressureBuf;
	}
	return 0.0;
}

float BMP085::getAltitude(float seaLevelPressure) {
	return 44330 * (1.0 - pow(pressureBuf / seaLevelPressure, 0.1903));
}
