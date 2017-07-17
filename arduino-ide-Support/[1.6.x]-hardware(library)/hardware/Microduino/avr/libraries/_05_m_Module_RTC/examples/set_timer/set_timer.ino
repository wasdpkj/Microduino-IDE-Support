// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* RTC定时器例程
*/

#include <Microduino_RTC.h>


RTC rtc;
/* 设置RTC启动时间
 * 年, 月, 星期, 日, 时, 分, 秒 */
DateTime dateTime = {2016, 6, 3, 1, 15, 30, 40};

//中断标志位
volatile uint8_t timerFlag=0;
uint32_t timer=millis();

//中断函数
void blink(){
  timerFlag=1;
}

void setup()
{
  pinMode(2, INPUT);           // 设置中断引脚
  Serial.begin(9600);
  //清除所有寄存器
  rtc.begin();
  rtc.clearAll();
  rtc.setDateTime(dateTime);
   
  //设置定时器频率 TIM_1_60HZ/TIM_1HZ/TIM_64HZ/TIM_4096HZ
  rtc.setTimerFreq(TIM_1HZ);
  //设置定时器计数值   定时器计时时间 = 计数值/定时器频率
  rtc.setTimerCnt(2);
  rtc.enableTimerInt();
  Serial.println("debug set timer");

  //设置外部中断，中断号0， 下降沿触发
  attachInterrupt(0, blink, FALLING);

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

  if (timerFlag==1){
    Serial.print("blink!\r\n");
	//清除中断
    rtc.clearTimerInt();   
    detachInterrupt(0);
    timerFlag=0;
    attachInterrupt(0, blink, FALLING);
  }
}
