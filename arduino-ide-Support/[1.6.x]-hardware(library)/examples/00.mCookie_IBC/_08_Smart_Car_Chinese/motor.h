#include <Microduino_Motor.h>
#include "colorLed.h"
#include "music.h"
#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define motor_pin0A 8
#define motor_pin0B 6
#define motor_pin1A 7
#define motor_pin1B 5
#else
#define motor_pin0A 6
#define motor_pin0B 8
#define motor_pin1A 5
#define motor_pin1B 7
#endif

#define CMD_OK     0
#define CMD_UP     1
#define CMD_DOWN   2
#define CMD_LEFT   3
#define CMD_RIGHT  4

#define RIGHT  1
#define LEFT   0

Motor MotorLeft(motor_pin0A, motor_pin0B);
Motor MotorRight(motor_pin1A, motor_pin1B);

bool leftFlag = false;
bool rightFlag = false;
int16_t leftSpeed = 0;
int16_t rightSpeed = 0;
uint32_t micTimer = 0;
uint32_t trackTimer = 0;


void motorUpdate(int16_t speed1, int16_t speed2)
{
    MotorLeft.Driver(speed1);
    MotorRight.Driver(speed2);
}


void motorCoast()
{
    leftSpeed = 0;
    rightSpeed = 0;
}

void remoteControl(uint8_t remoteCmd)
{
    switch(remoteCmd)
    {
        case CMD_UP:
           setAllLed(COLOR_NONE);
           leftSpeed= MAX_THROTTLE;
           rightSpeed = MAX_THROTTLE;
           break;
        case CMD_DOWN:
           setAllLed(COLOR_RED);
           if (leftSpeed > 0 || rightSpeed > 0)
           {
              leftSpeed = 0;
              rightSpeed = 0;
           }
           else if(leftSpeed == 0 && rightSpeed ==0)
           {
              tone(PIN_BUZZER, TONE_FA);
              delay(500);
              noTone(PIN_BUZZER);
              delay(500);
              tone(PIN_BUZZER, TONE_FA);
              delay(400);
              noTone(PIN_BUZZER);
              delay(400);
              tone(PIN_BUZZER, TONE_FA);
              delay(300);
              noTone(PIN_BUZZER);
              delay(300);
              tone(PIN_BUZZER, TONE_SI);
              delay(200);
              noTone(PIN_BUZZER);
              delay(200);
              setAllLed(COLOR_RED);
              leftSpeed = -MAX_THROTTLE;
              rightSpeed = -MAX_THROTTLE;
           }
          break;
      case CMD_LEFT:
          if(leftSpeed < 0)
          {
              leftSpeed = -MAX_THROTTLE + STEER;
              rightSpeed = -MAX_THROTTLE - STEER;
              setLed(COLOR_RED, RIGHT);
          }
          else
          {
              leftSpeed = MAX_THROTTLE - STEER; 
              rightSpeed = MAX_THROTTLE + STEER; 
              setLed(COLOR_NONE, RIGHT);
          }
          ledBlink(500, COLOR_YELLOW, LEFT);
          break;
      case CMD_RIGHT:
          if(rightSpeed < 0)
          {
              rightSpeed = -MAX_THROTTLE + STEER;
              leftSpeed = -MAX_THROTTLE - STEER;
              setLed(COLOR_RED, LEFT);
          }
          else
          {
              rightSpeed = MAX_THROTTLE - STEER; 
              leftSpeed = MAX_THROTTLE + STEER;
              setLed(COLOR_NONE, LEFT);
          }
          ledBlink(500,COLOR_YELLOW, RIGHT);
          break;
      case CMD_OK:
          if(leftSpeed != 0 || rightSpeed != 0)
          {
            setAllLed(COLOR_NONE);
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
   if (trackVal1 < TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
   {
       leftSpeed = MAX_THROTTLE;
       rightSpeed = MAX_THROTTLE;
   }
   else if (trackVal1 < TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
   {
       leftSpeed = MAX_THROTTLE + STEER;
       rightSpeed = MAX_THROTTLE - STEER;
   }
   else if (trackVal1 > TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
   {
       leftSpeed = MAX_THROTTLE - STEER;
       rightSpeed = MAX_THROTTLE + STEER;
   }
   else if (trackVal1 > TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
   {
       leftSpeed = 0;
       rightSpeed = 0;
   }
}


void micControl(uint16_t micVal, uint16_t trackVal1, uint16_t trackVal2)
{
  if (micVal > 100 && micTimer == 0)  //If there is noise, set countdown for 6 seconds. 
  {
     micTimer = millis() + 6000;
  }
  else if(millis() < micTimer && micTimer > 0)  
  {
      if((trackVal1 < TRACK_THRESHOLD || trackVal2 < TRACK_THRESHOLD) && trackTimer == 0)
      {
          leftFlag = trackVal1 < DOCK_MAX;
          rightFlag = trackVal2 < DOCK_MAX;
          trackTimer = millis();
      }
      else if(millis() - trackTimer < 600)
      {
          setAllLed(COLOR_RED);
          leftSpeed = -MAX_THROTTLE;
          rightSpeed = -MAX_THROTTLE;
      }
      else if(millis() - trackTimer < 1000)
      {
          if(trackVal1 < TRACK_THRESHOLD || trackVal2 < TRACK_THRESHOLD)
          {
              trackTimer = 0;  
          }
          else if(leftFlag)  
          {  
              setLed(COLOR_NONE, LEFT);
              setLed(COLOR_YELLOW, RIGHT);
              leftSpeed = -MAX_THROTTLE + STEER*2;
              rightSpeed = -MAX_THROTTLE - STEER*2;
          }
          else if(rightFlag)
          {
              setLed(COLOR_YELLOW, LEFT);
              setLed(COLOR_NONE, RIGHT);
              leftSpeed = -MAX_THROTTLE - STEER * 2;
              rightSpeed = -MAX_THROTTLE + STEER * 2;
          }
      }
      else
      {
          setAllLed(COLOR_NONE);
          trackTimer = 0;
          leftSpeed = MAX_THROTTLE;
          rightSpeed = MAX_THROTTLE;
      }    
  }
  else
  {
     setAllLed(0);
     leftSpeed = 0;
     rightSpeed = 0;
     micTimer = 0;
  }
}
