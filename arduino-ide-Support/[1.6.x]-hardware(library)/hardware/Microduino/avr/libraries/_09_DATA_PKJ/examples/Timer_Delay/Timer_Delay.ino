#include <PKJ.h>

PKJ pkj;

#define pin_1 12
#define pin_2 11
#define pin_3 10
#define pin_4 9

boolean vol_1,vol_2,vol_3,vol_4;


void setup()
{
  Serial.begin(115200);
  
  pinMode(pin_1, OUTPUT);
  pinMode(pin_2, OUTPUT);
  pinMode(pin_3, OUTPUT);
  pinMode(pin_4, OUTPUT);

  pkj.Timer_Delay_Begin();
}

void loop()
{
  if(pkj.Timer_Delay(0,200))
    led1();
  if(pkj.Timer_Delay(1,500))
    led2();
  if(pkj.Timer_Delay(2,900))
    led3();
  if(pkj.Timer_Delay(3,1500))
    led4();

  digitalWrite(pin_1, vol_1);
  digitalWrite(pin_2, vol_2);
  digitalWrite(pin_3, vol_3);
  digitalWrite(pin_4, vol_4);
}


void led1()
{
  vol_1=!vol_1;
}

void led2()
{
  vol_2=!vol_2;
}

void led3()
{
  vol_3=!vol_3;
}

void led4()
{
  vol_4=!vol_4;
}