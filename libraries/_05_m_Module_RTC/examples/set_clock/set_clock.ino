// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* RTC设置时间例程
*/


#include <Microduino_RTC.h>


RTC rtc;

/* 设置RTC启动时间
 * 年, 月, 星期, 日, 时, 分, 秒 */
DateTime dateTime = {2016, 6, 3, 1, 15, 30, 40};

uint16_t tYear;
uint8_t tMonth, tWeekday, tDay, tHour, tMinute, tSecond; 

void setup()
{
  Serial.begin(9600);
  //清除所有寄存器
  rtc.begin();
  rtc.clearAll();
  //设置启动时间
  rtc.setDateTime(dateTime);
}

void loop()
{
  //通过getDataTime获取时间
  rtc.getDateTime(&dateTime);
  Serial.println("CODE_1:");
  Serial.print(dateTime.year);
  Serial.print("/");
  Serial.print(dateTime.month);
  Serial.print("/");
  Serial.print(dateTime.day);
  Serial.print("     ");
  Serial.print(dateTime.hour);
  Serial.print(":");
  Serial.print(dateTime.minute);
  Serial.print(":");
  Serial.print(dateTime.second);
  Serial.print("\r\n");

  //通过getXX() 获取具体时间
  rtc.getDate(&tYear, &tMonth, &tWeekday, &tDay);
  rtc.getTime(&tHour, &tMinute, &tSecond);
  Serial.println("CODE_2:");
  Serial.print(tYear);
  Serial.print("/");
  Serial.print(tMonth);
  Serial.print("/");
  Serial.print(tDay);
  Serial.print("     ");
  Serial.print(tHour);
  Serial.print(":");
  Serial.print(tMinute);
  Serial.print(":");
  Serial.print(tSecond);
  Serial.print("\r\n");

  delay(1000);
  Serial.print("\r\n");
}