// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc


#include <Arduino.h>

#include <I2Cdev.h>
#include "Microduino_RTC.h"

/************************ static functions ************************/

static inline uint8_t decToBcd(uint8_t val)
{
  return ( (val/10*16) + (val%10) );
}

static inline uint8_t bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

/************************ RTC ************************/

RTC::RTC(void)
{
	devAddr = RTCC_ADDR;	
}

bool RTC::begin()
{  	 
	Wire.begin();
	Wire.beginTransmission(devAddr);
	if (Wire.endTransmission())
		return false;
	return true;
}


void RTC::clearStatus()
{
	uint8_t buf[2] = {0, 0};
	I2Cdev::writeBytes(devAddr, RTCC_STAT1_ADDR, 2, buf);
}


void RTC::clearAll()
{
	clearStatus();	
	disableAlarm();
	disableSquareWave();
	disableTimer();	
}


void RTC::setTime(uint8_t _hour, uint8_t _minute, uint8_t _second)
{
	uint8_t buf[3];
	buf[0] = decToBcd(_second);
	buf[1] = decToBcd(_minute);
	buf[2] = decToBcd(_hour);
	I2Cdev::writeBytes(devAddr, RTCC_SEC_ADDR, 3, buf);
}


void RTC::getTime(uint8_t *_hour, uint8_t *_minute, uint8_t *_second)
{  
	uint8_t buf[3]; 
	I2Cdev::readBytes(devAddr, RTCC_SEC_ADDR, 3, buf);
 	*_second = bcdToDec(buf[0] & 0x7F);
	*_minute = bcdToDec(buf[1] & 0x7F);
	*_hour = bcdToDec(buf[2] & 0x3F);
}



void RTC::setDate(uint16_t _year, uint8_t _month, uint8_t _weekday, uint8_t _day)
{
    /* year val is 00 to 99, xx
        with the highest bit of month = century
        0=20xx
        1=19xx
        */
	uint8_t buf[4];
	buf[0] = decToBcd(_day);
	buf[1] = decToBcd(_weekday);	
	buf[2] = decToBcd(_month);
	if (_year < 2000){
        buf[2] |= RTCC_CENTURY_MASK;
    }else {
        buf[2] &= ~RTCC_CENTURY_MASK;
    }
	buf[3] = decToBcd((uint8_t)(_year%100));
	I2Cdev::writeBytes(devAddr, RTCC_DAY_ADDR, 4, buf);
}


void RTC::getDate(uint16_t *_year, uint8_t *_month, uint8_t *_weekday, uint8_t *_day)
{  
	uint8_t buf[4]; 
	I2Cdev::readBytes(devAddr, RTCC_DAY_ADDR, 4, buf);
	//0x3f = 0b00111111
	*_day = bcdToDec(buf[0] & 0x3F);
	//0x07 = 0b00000111
	*_weekday = bcdToDec(buf[1] & 0x07);
	//get raw month data byte and set month and century with it.
	*_year = bcdToDec(buf[3]);
	if (buf[2] & RTCC_CENTURY_MASK) {
		*_year += 1900;
	}else {
		*_year += 2000;
	}
	//0x1f = 0b00011111
	*_month = bcdToDec(buf[2] & 0x1F);
}


void RTC::setDateTime(DateTime _dateTime)
{
	uint8_t buf[7];
	buf[0] = decToBcd(_dateTime.second);
	buf[1] = decToBcd(_dateTime.minute);
	buf[2] = decToBcd(_dateTime.hour);
	buf[3] = decToBcd(_dateTime.day);
	buf[4] = decToBcd(_dateTime.weekday);	
	buf[5] = decToBcd(_dateTime.month);
	if (_dateTime.year < 2000){
        buf[5] |= RTCC_CENTURY_MASK;
    }else {
        buf[5] &= ~RTCC_CENTURY_MASK;
    }
	buf[6] = decToBcd((uint8_t)(_dateTime.year%100));
	I2Cdev::writeBytes(devAddr, RTCC_SEC_ADDR, 7, buf);	 
}


void RTC::getDateTime(DateTime *_dateTime)
{
	/* set the start byte , get the 2 status bytes */
	Wire.beginTransmission(devAddr);
 	Wire.write(RTCC_SEC_ADDR);
 	Wire.endTransmission();
  	
 	Wire.requestFrom(devAddr, 7u); //request 5 bytes
	
	uint8_t buf[7]; 
	I2Cdev::readBytes(devAddr, RTCC_SEC_ADDR, 7, buf);

 	_dateTime->second = bcdToDec(buf[0] & 0x7F);
	_dateTime->minute = bcdToDec(buf[1] & 0x7F);
	_dateTime->hour = bcdToDec(buf[2] & 0x3F);
	_dateTime->day = bcdToDec(buf[3] & 0x3F);
	_dateTime->weekday = bcdToDec(buf[4] & 0x07);
	_dateTime->year = bcdToDec(buf[6]);
	if (buf[5] & RTCC_CENTURY_MASK) {
		_dateTime->year += 1900;
	}else {
		_dateTime->year += 2000;
	}
	_dateTime->month = bcdToDec(buf[5] & 0x1F);
}



/* set the alarm values
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void RTC::setAlarm(uint8_t _minute, uint8_t _hour, uint8_t _day, uint8_t _weekday, uint8_t _enable)
{
    uint8_t buf[4];
	
	buf[0] = decToBcd(constrain(_minute, 0, 59))|RTCC_ALARM;
	buf[1] = decToBcd(constrain(_hour, 0, 23))|RTCC_ALARM;
	buf[2] = decToBcd(constrain(_day, 1, 31))|RTCC_ALARM;
	buf[3] = decToBcd(constrain(_weekday, 0, 6))|RTCC_ALARM;
	
	switch(_enable){
		case EN_MINUTE:
			buf[0] &= ~RTCC_ALARM;
			break;
		case EN_HOUR:
			buf[1] &= ~RTCC_ALARM;
			break;
		case EN_DAY:
			buf[2] &= ~RTCC_ALARM;
			break;
		case EN_WEEKDAY:
			buf[3] &= ~RTCC_ALARM;
			break;
		default:
			break;
	}

	I2Cdev::writeBytes(devAddr, RTCC_ALRM_MIN_ADDR, 4, buf);
}



void RTC::disableAlarm()
{
	setAlarm(0, 0, 0, 0, EN_NONE);	
}

/**
* Set the square wave pin output
*/
void RTC::setSquareWave(uint8_t _freq)
{
	I2Cdev::writeByte(devAddr, RTCC_SQW_ADDR, _freq);
}

void RTC::disableSquareWave()
{
    setSquareWave(SQW_DISABLE);
}


void RTC::setTimerFreq(uint8_t _freq)
{
	I2Cdev::writeByte(devAddr, RTCC_TIMER_FRE_ADDR, _freq);
}


void RTC::disableTimer()
{
	setTimerFreq(TIM_DISABLE);
}


void RTC::setTimerCnt(uint8_t _count)
{
	I2Cdev::writeByte(devAddr, RTCC_TIMER_CNT_ADDR, _count);
}


/* enable alarm interrupt
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void RTC::enableAlarmInt()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2);    	
	//set status2 AF val to zero
    status2 &= ~RTCC_STATUS_AF;
    //enable the interrupt
    status2 |= RTCC_STATUS_AIE;

    //enable the interrupt
	I2Cdev::writeByte(devAddr, RTCC_STAT2_ADDR, status2);	
}

void RTC::disableAlarmInt()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 
	//set status2 AF val to zero to reset alarm 
	status2 &= ~RTCC_STATUS_AF;
	//turn off the interrupt
	status2 &= ~RTCC_STATUS_AIE;
		
	I2Cdev::writeByte(devAddr, RTCC_STAT2_ADDR, status2);	 
}


/**
* Reset the alarm leaving interrupt unchanged
*/
void RTC::clearAlarmInt()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 
    //set status2 AF val to zero to reset alarm
    status2 &= ~RTCC_STATUS_AF;
	I2Cdev::writeByte(devAddr, RTCC_STAT2_ADDR, status2);
}


void RTC::enableTimerInt()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 
    //set status2 AF val to zero
    status2 &= ~RTCC_STATUS_TF;
    //enable the interrupt
    status2 |= RTCC_STATUS_TIE;

    //enable the interrupt
	I2Cdev::writeByte(devAddr, RTCC_STAT2_ADDR, status2);
}

void RTC::disableTimerInt()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 	
	//set status2 AF val to zero to reset alarm 
	status2 &= ~RTCC_STATUS_TF;
	//turn off the interrupt
	status2 &= ~RTCC_STATUS_TIE;
		
	I2Cdev::writeByte(devAddr, RTCC_STAT2_ADDR, status2); 
}


void RTC::clearTimerInt()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 	
    //set status2 AF val to zero to reset alarm
    status2 &= ~RTCC_STATUS_TF;
	
	I2Cdev::writeByte(devAddr, RTCC_STAT2_ADDR, status2); 
}

/*
* Returns true if AF is on
*
*/
boolean RTC::isAlarmed()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 
    return status2 & RTCC_STATUS_AF;
}


boolean RTC::isCountdown()
{
	uint8_t status2;
	I2Cdev::readByte(devAddr, RTCC_STAT2_ADDR, &status2); 
	return status2 & RTCC_STATUS_TF;
}

