// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* RTC闹钟例程
*/

#include <Microduino_RTC.h>


RTC rtc;

#define PIN_IRQ D2

/* 设置RTC启动时间
 * 年, 月, 星期, 日, 时, 分, 秒 */
DateTime dateTime = {2016, 6, 3, 1, 15, 30, 40};

//中断标志位
volatile uint8_t alarmFlag=0;
uint32_t timer=millis();

//中断函数
void blink(){
  alarmFlag=1;
}

void setup()
{
  pinMode(PIN_IRQ, INPUT);           // 设置中断引脚
  Serial.begin(9600);
  //清除所有寄存器
  rtc.begin();
  rtc.clearAll();
  rtc.setDateTime(dateTime);
   
  //设置分钟报警，每到31分的时候报警
  rtc.setAlarm(31, 0, 0, 0, EN_MINUTE);
  rtc.enableAlarmInt();
  Serial.println("debug set alarm");

  //设置外部中断，中断号0， 下降沿触发
  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), blink, FALLING);
}

void loop()
{
  if(millis()-timer>1000)
  {
    //打印当前时间
    rtc.getDateTime(&dateTime);    
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
    timer=millis();  
  }

  if (alarmFlag==1){
    Serial.print("Alarm!\r\n");
	//清除中断
    rtc.clearAlarmInt();   
    detachInterrupt(0);
    alarmFlag=0;
    attachInterrupt(0, blink, FALLING);
  }
}
