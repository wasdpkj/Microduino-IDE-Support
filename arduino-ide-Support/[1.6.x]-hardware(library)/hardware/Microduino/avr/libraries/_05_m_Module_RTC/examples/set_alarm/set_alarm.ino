#include <Microduino_RTC.h>

/* get a real time clock object */
RTC rtc;
/* set a time to start with.
 * year, month, weekday, day, hour, minute, second */
DateTime dateTime = {2016, 6, 3, 1, 15, 30, 40};
/* a flag for the interrupt */
volatile uint8_t alarmFlag=0;
uint32_t timer=millis();

/* the interrupt service routine */
void blink(){
  alarmFlag=1;
}

void setup()
{
  pinMode(2, INPUT);           // set pin to input
  Serial.begin(9600);
  /* clear out all the registers */
  rtc.begin();
  rtc.clearAll();
  rtc.setDateTime(dateTime);
   
  rtc.setAlarm(31, 0, 0, 0, EN_MINUTE);
  rtc.enableAlarmInt();
  Serial.println("debug set alarm");

  /* setup int on pin 2 of arduino */
  attachInterrupt(0, blink, FALLING);
}

void loop()
{
  if(millis()-timer>1000)
  {
    /* each sec update the display */
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
    rtc.clearAlarmInt();   
    detachInterrupt(0);
    alarmFlag=0;
    attachInterrupt(0, blink, FALLING);
  }
}
