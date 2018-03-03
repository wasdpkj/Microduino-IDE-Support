#include <Microduino_Motor.h>
#include "colorLed.h"
#include "music.h"

#define CMD_OK     0
#define CMD_UP     1
#define CMD_DOWN   2
#define CMD_LEFT   3
#define CMD_RIGHT  4

#define RIGHT  1
#define LEFT   0

Motor MotorLeft(MOTOR0_PINA, MOTOR0_PINB);
Motor MotorRight(MOTOR1_PINA, MOTOR1_PINB);

bool leftFlag = false;
bool rightFlag = false;
int16_t leftSpeed = 0;
int16_t rightSpeed = 0;
uint32_t micTime = 0;
uint32_t trackTime = 0, trackTime1 = 0;
bool back;
bool fback = true;

void motorUpdate(int16_t speed1, int16_t speed2)
{
  MotorLeft.setSpeed(-speed1);
  MotorRight.setSpeed(speed2);
}


void motorCoast()
{
  leftSpeed = 0;
  rightSpeed = 0;
}

void remoteControl(uint8_t remoteCmd)
{
  switch (remoteCmd)
  {
    case CMD_UP:
      setAllColor(COLOR_COLD);
      leftSpeed = MAX_THROTTLE;
      rightSpeed = MAX_THROTTLE;
      break;
    case CMD_DOWN:
      setAllColor(COLOR_RED);
      if (leftSpeed > 0 || rightSpeed > 0)
      {
        leftSpeed = 0;
        rightSpeed = 0;
      }
      else if (leftSpeed == 0 && rightSpeed == 0)
      {
        setAllColor(COLOR_RED);
        leftSpeed = -MAX_THROTTLE;
        rightSpeed = -MAX_THROTTLE;
      }
      break;
    case CMD_LEFT:
      if (leftSpeed < 0)
      {
        leftSpeed = -MAX_THROTTLE + STEER;
        rightSpeed = -MAX_THROTTLE - STEER;
        setColor(COLOR_NONE, RIGHT);
      }
      else
      {
        leftSpeed = MAX_THROTTLE - STEER;
        rightSpeed = MAX_THROTTLE + STEER;
        setColor(COLOR_NONE, RIGHT);
      }
      ledBlink(500, COLOR_YELLOW, LEFT);
      break;
    case CMD_RIGHT:
      if (rightSpeed < 0)
      {
        rightSpeed = -MAX_THROTTLE + STEER;
        leftSpeed = -MAX_THROTTLE - STEER;
        setColor(COLOR_NONE, LEFT);
      }
      else
      {
        rightSpeed = MAX_THROTTLE - STEER;
        leftSpeed = MAX_THROTTLE + STEER;
        setColor(COLOR_NONE, LEFT);
      }
      ledBlink(500, COLOR_YELLOW, RIGHT);
      break;
    case CMD_OK:
      if (leftSpeed != 0 || rightSpeed != 0)
      {
        setAllColor(COLOR_NONE);
        leftSpeed = 0;
        rightSpeed = 0;
      }
      break;
    default:
      break;
  }
}

void trackControl(uint16_t trackVal1, uint16_t trackVal2)
{
  leftSpeed = ((1000 - trackVal1) / 15);
  rightSpeed = ((1000 - trackVal2) / 15);
  setAllColor(COLOR_COLD);
}

void micControl(long micVal, long trackVal1, long trackVal2) {
  if (micVal > 200 && micTime == 0)
    micTime = millis() + 6000;
  else if (millis() < micTime && micTime > 0)
  {
    trackVal1 = constrain(trackVal1, 800, 1023);
    trackVal1 = (map(trackVal1, 800, 1023, (-255), 255));
    trackVal2 = constrain(trackVal2, 850, 1023);
    trackVal2 = (map(trackVal2, 800, 1023, (-255), 255));
    leftSpeed = trackVal2;
    rightSpeed = trackVal1;
    if (trackVal1 < 40 && trackVal2 < 40) {
      if (back) {
        trackTime = millis();
        back = false;
      }
      trackTime1 = millis();
      if (trackTime1 - trackTime > 800) {
        setAllColor(COLOR_RED);
        motorUpdate(-150, -150);
        delay(1000);
        motorUpdate(150, -150);
        delay(300);
      }
    } else {
      setAllColor(COLOR_COLD);
      back = true;
      trackTime1 = trackTime;
    }
  } else {
    setAllColor(COLOR_NONE);
    leftSpeed = 0;
    rightSpeed = 0;
    micTime = 0;
  }
}
