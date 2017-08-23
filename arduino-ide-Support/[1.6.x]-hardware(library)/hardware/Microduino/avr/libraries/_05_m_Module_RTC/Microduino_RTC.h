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
#ifndef _MICRDOUINO_RTC_H_
#define _MICRDOUINO_RTC_H_

#include <Arduino.h>
#include <Wire.h>

/* the read and write values for pcf8563 rtcc */
/* these are adjusted for arduino */
#define RTCC_ADDR 	0x51

/* register addresses in the rtc */
#define RTCC_STAT1_ADDR			0x00
#define RTCC_STAT2_ADDR			0x01
#define RTCC_SEC_ADDR  			0x02
#define RTCC_MIN_ADDR 			0x03
#define RTCC_HR_ADDR 			0x04
#define RTCC_DAY_ADDR 			0x05
#define RTCC_WEEKDAY_ADDR		0x06
#define RTCC_MONTH_ADDR 		0x07
#define RTCC_YEAR_ADDR 			0x08
#define RTCC_ALRM_MIN_ADDR 	    0x09
#define RTCC_ALRM_HR_ADDR		0x0A
#define RTCC_ALRM_DAY_ADDR		0x0B
#define RTCC_ALRM_WDY_ADDR		0x0C
#define RTCC_SQW_ADDR 	        0x0D
#define RTCC_TIMER_FRE_ADDR		0x0E
#define RTCC_TIMER_CNT_ADDR		0x0F

/* setting the alarm flag to 0 enables the alarm.
 * set it to 1 to disable the alarm for that value.
 */
#define RTCC_ALARM				0x80
#define RTCC_TIMER				0x80
#define RTCC_STATUS_TIE			0x01
#define RTCC_STATUS_AIE 		0x02
#define RTCC_STATUS_TF			0x04
#define RTCC_STATUS_AF 			0x08 // 0x08 : not 0x04!!!!
/* optional val for no alarm setting */

#define EN_NONE					0x00
#define EN_MINUTE				0x01
#define EN_HOUR					0x02
#define EN_DAY					0x04
#define EN_WEEKDAY				0x08

#define RTCC_CENTURY_MASK 		0x80

/* square wave contants */
#define SQW_DISABLE     	B00000000
#define SQW_32KHZ       	B10000000
#define SQW_1024HZ      	B10000001
#define SQW_32HZ        	B10000010
#define SQW_1HZ         	B10000011

#define TIM_DISABLE			B00000011
#define TIM_4096HZ			B10000000
#define TIM_64HZ			B10000001
#define TIM_1HZ				B10000010
#define TIM_1_60HZ			B10000011


typedef struct{
    uint16_t year; 
	uint8_t month, weekday, day, hour, minute, second;
}DateTime;


/* arduino class */
class RTC {
	public:
		RTC();
		
		bool begin();		/* zero out all values, disable all alarms */
		void clearStatus();	/* set both status bytes to zero */
		void clearAll();	

		void setTime(uint8_t _hour, uint8_t _minute, uint8_t _second);
		void getTime(uint8_t *_hour, uint8_t *_minute, uint8_t *_second);    /* get time vars + 2 status bytes to local vars */
		void setDate(uint16_t _year, uint8_t _month, uint8_t _weekday, uint8_t _day);
		void getDate(uint16_t *_year, uint8_t *_month, uint8_t *_weekday, uint8_t *_day);			/* get date vals to local vars */
		void setDateTime(DateTime _dateTime);
		void getDateTime(DateTime *_dateTime);
		
		void setAlarm(uint8_t _minute, uint8_t _hour, uint8_t _day, uint8_t _weekday, uint8_t _enable); /* set alarm vals, 99=ignore */
		void disableAlarm();
		
		void setSquareWave(uint8_t frequency);
		void disableSquareWave();
		void setTimerFreq(uint8_t frequency);
		void disableTimer();
		void setTimerCnt(uint8_t count);
		
		void enableAlarmInt(); /* activate alarm flag and interrupt */
		void disableAlarmInt();	/* clear alarm flag and interrupt */
		void clearAlarmInt();  /* clear alarm flag but leave interrupt unchanged */
			
		void enableTimerInt();
		void disableTimerInt();
		void clearTimerInt();
		
		boolean isAlarmed();
		boolean isCountdown();
		
	private:
		uint8_t devAddr;
};

#endif
