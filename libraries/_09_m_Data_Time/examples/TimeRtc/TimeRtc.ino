#include <Microduino_RTC.h>
#include <TimeLib.h>

//如果开启下面#define set_time的标志，则按照rtcTime中的时间设置RTC模块中的时间
#define set_time

DateTime rtcTime = {2017, 7, 12, 6, 15, 22, 50};//时间格式（年，月，日，时，分，秒）

RTC rtc;
time_t prevDisplay = 0;

void setup()
{
  Serial.begin(115200);
  rtc.begin();
  //如果之前开启了#define set_time的标志，则按照rtcTime中的时间设置RTC模块中的时间
#ifdef set_time
  rtc.clearAll();
  rtc.setDateTime(rtcTime);
#endif
  Serial.println("RTC-time: ");
  rtc.getDateTime(&rtcTime);//获取RTC时间
  serialClockDisplay(rtcTime.year, rtcTime.month, rtcTime.day, rtcTime.hour, rtcTime.minute, rtcTime.second);//打印RTC时间
  setTime(rtcTime.hour, rtcTime.minute, rtcTime.second, rtcTime.day, rtcTime.month, rtcTime.year);//使用RTC时间校准本地时间
}

void loop()
{
  if (timeStatus() != timeNotSet) {
    //如果本地时间发生变化，则更新串口显示
    if (now() != prevDisplay) {
      prevDisplay = now();
      Serial.println("Location-time: ");
      serialClockDisplay(year(), month(), day(), hour(), minute(), second());
    }
  }
}

//*****串口打印日期时间*****
void serialClockDisplay(int _year, int _month, int _day, int _hour, int _minute, int _second) {
  if (_year < 1000) {
    Serial.print("20");
  }
  Serial.print(_year, DEC);
  Serial.print('/');
  if (_month < 10) {
    Serial.print("0");
  }
  Serial.print(_month, DEC);
  Serial.print('/');
  if (_day < 10) {
    Serial.print("0");
  }
  Serial.print(_day, DEC);
  Serial.print("   ");
  Serial.print(_hour, DEC);
  Serial.print(':');
  if (_minute < 10) {
    Serial.print("0");
  }
  Serial.print(_minute, DEC);
  Serial.print(':');
  if (_second < 10) {
    Serial.print("0");
  }
  Serial.println(_second, DEC);
  Serial.println();
}