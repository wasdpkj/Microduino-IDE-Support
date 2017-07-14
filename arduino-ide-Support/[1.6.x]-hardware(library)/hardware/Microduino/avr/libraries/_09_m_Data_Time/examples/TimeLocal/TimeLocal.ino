#include <TimeLib.h>

time_t prevDisplay = 0;
uint32_t timer = millis();

void setup()
{
  Serial.begin(115200);
  setTime(14, 06, 55, 10, 7, 2017);//设置时间（格式：时，分，秒，日，月，年）
  Serial.println("Calibration complete!");
  Serial.println();
}

void loop()
{
  //如果本地时间发生变化，则更新串口显示
  if (now() != prevDisplay) {
    prevDisplay = now();
    serialClockDisplay(year(), month(), day(), hour(), minute(), second());
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