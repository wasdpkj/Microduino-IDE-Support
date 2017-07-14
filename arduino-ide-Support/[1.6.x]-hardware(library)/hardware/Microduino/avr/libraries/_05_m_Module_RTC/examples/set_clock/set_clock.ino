#include <Microduino_RTC.h>

//init the real time clock
RTC rtc;
//year, month, weekday, day, hour, minute, second
DateTime dateTime = {2016, 6, 3, 1, 15, 30, 40};

uint16_t tYear;
uint8_t tMonth, tWeekday, tDay, tHour, tMinute, tSecond; 

void setup()
{
  Serial.begin(9600);
  //clear out the registers
  rtc.begin();
  rtc.clearAll();
  //set a time to start with.
  rtc.setDateTime(dateTime);
}

void loop()
{
  //both format functions call the internal getTime() so that the 
  //formatted strings are at the current time/date.
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