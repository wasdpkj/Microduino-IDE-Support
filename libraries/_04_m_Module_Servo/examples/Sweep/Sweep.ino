// Sweep
// by BARRAGAN <http://barraganstudio.com>
// This example code is in the public domain.

/*
  如果使用核心Microduino_ESP32，任意引脚都可以用来驱动舵机
  供电允许条件下最多可驱动8个舵机
  使用LEDC_PWM高8个通道驱动
  用数字引脚前要加D,例：D5
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// a maximum of eight servo objects can be created

#define servoPin  9

#if defined (ESP32)
#define servoPin  D9   //使用ESP32时，数字引脚号前要加D
#endif

int pos = 0;    // variable to store the servo position

void setup()
{
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
  for (pos = 0; pos < 180; pos += 1) // goes from 0 degrees to 180 degrees
  { // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 1; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
