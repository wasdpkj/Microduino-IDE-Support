/*
  =================================================================================                         
                              | What is: Smart Car? |
  ---------------------------------------------------------------------------------                         
  Make a Smart Car that can be a lot of things! Police car, truck, mobile home, etc.
  You can control it with a Remote Control...or not. Let your imagination run free!
    
  =================================================================================
                                  | How to Play |
  ---------------------------------------------------------------------------------
  Use the Remote Control to select one of 3 modes:
  
  Mode A: Remote Control Mode
          - Use the Remote Control's arrow keys to drive the car. Forward, backward,
           left, and right.
          - When turning, buzzing and signal lights will indicate turn direction.
          
  Mode B: Line Finder Mode
          - Draw a path on the floor with black tape and place the Smart Car on the
            path. The car will begin to follow the path.
            
  Mode C: Sound Control + Obstacle Avoidance Mode
          - When there's noise (like clapping your hands), the car will begin moving 
            straight until it encounters an obstacle. When blocked, it will buzz and 
            light up, then proceed to turn until it is no longer blocked. It will 
            stop moving after 6 seconds of silence.
  =================================================================================
                                    | You will |
  ---------------------------------------------------------------------------------
  Use everything you have learned to create something awesome!
  =================================================================================
                           | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)
  
  BRIGHT_MAX: Max LED brightness.
  MAX_THROTTLE: How quickly the Smart Car moves.
  STEER: How quickly the Smart Car turns.
  
  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                              | Brainstorming Ideas |
  ---------------------------------------------------------------------------------                           
  1. Can you build the Smart Car into a police car, tank, ice cream truck?
  2. Can you put Crash Sensors in front of the car so that when it crashes into a 
     wall, it'll play some type of song?
  3. Can you modify the music.h file to add a fun tunes?
     Perhaps WHEE-OOH WHEE-OOH WHEE-OOH of ambulances or an ice cream truck tune.
  4. Now that you know how to use Motors. You could make a plane using the building
     blocks and put a Motor on each wing to create an electronic model!
       
     Note: Some of the above may require significant modification of the code.  
  =================================================================================
                               | Port Connections |
  ---------------------------------------------------------------------------------
  Buzzer: D10
  ColorLED: D12
  Line Finder A: A0
  Line Finder B: A2
  Microphone Sensor: A6
  Infrared Receiver: D4
  =================================================================================
                                  | Questions? |
  ---------------------------------------------------------------------------------                                 
  Visit us at http://www.microduino.cc/ and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/

#include "IRControl.h"
#include "motor.h"
#include "userDef.h"

uint8_t remoteCmd;
uint16_t lineValA;
uint16_t lineValB;
uint16_t micVal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LINEA, INPUT);
  pinMode(PIN_LINEB, INPUT);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  irrecv.enableIRIn();
  MotorLeft.Fix(1.1);
  MotorRight.Fix(1);

  for (int i = 9; i >= 0; i--)
  {
    setAllLed(i);
    delay(200);
  }
#if DEBUG
  Serial.println("**************START************");
#endif  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)) {//If there is an infrared signal,
     if (results.value != 0xFFFFFFFF && results.value != 0)//Remove gibberish
        remoteCmd = recvRemote(results.value);
     irrecv.enableIRIn();
     irrecv.resume(); 
  }
  micVal = analogRead(PIN_MIC);
  lineValA = analogRead(PIN_LINEA);
  lineValB = analogRead(PIN_LINEB);
#if DEBUG
   Serial.print("MIC Val:");
   Serial.print(micVal);
   Serial.print("\tLINEA Val:");
   Serial.print(lineValA);
   Serial.print("\tLINEB Val:");
   Serial.println(lineValB);
#endif

  switch(mode)
  {
      case MODE_A:
         remoteControl(remoteCmd);
         break;
      case MODE_B:
         trackControl(lineValA, lineValB);
         break;
      case MODE_C:
         micControl(micVal, lineValA, lineValB);
         break;   
      default:
         motorCoast();
         break;   
  }
  motorUpdate(leftSpeed, rightSpeed);
}


//------------Infrared Receiver Function-------------//
uint8_t recvRemote(uint32_t recvCode)
{
    uint8_t cmd = CMD_OK;
    switch(recvCode){
       case BUTTON_A:
          motorCoast();
          setAllLed(COLOR_RED);
          tone(PIN_BUZZER, TONE_DO);
          delay(300); 
          setAllLed(COLOR_NONE);
          noTone(PIN_BUZZER);
          mode = MODE_A;
          break;
       case BUTTON_B:
          motorCoast();
          setAllLed(COLOR_RED);
          tone(PIN_BUZZER, TONE_DO);
          delay(300);
          setAllLed(COLOR_GREEN);
          tone(PIN_BUZZER, TONE_RE);
          delay(300);
          setAllLed(COLOR_NONE);
          noTone(PIN_BUZZER);
          mode = MODE_B;
          break;
       case BUTTON_C:
          motorCoast();
          setAllLed(COLOR_RED);
          tone(PIN_BUZZER, TONE_DO);
          delay(300);
          setAllLed(COLOR_GREEN);
          tone(PIN_BUZZER, TONE_RE);
          delay(300);
          setAllLed(COLOR_BLUE);
          tone(PIN_BUZZER, TONE_MI);
          delay(300);
          setAllLed(COLOR_NONE);
          noTone(PIN_BUZZER);
          mode = MODE_C;
          break;
       case BUTTON_UP:
          cmd = CMD_UP;
          break;
       case BUTTON_DOWN:
          cmd = CMD_DOWN;
          break;
       case BUTTON_LEFT:
          cmd = CMD_LEFT;
          break;
       case BUTTON_RIGHT:
          cmd = CMD_RIGHT;
          break;
       case BUTTON_OK:
          cmd = CMD_OK;
          break;
       default:
          break;
   }
   return cmd;
}
