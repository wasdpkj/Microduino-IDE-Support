/*
  =================================================================================
                               | What is: Windmill? |
  ---------------------------------------------------------------------------------
  Blow into the Microphone to start the windmill! The LED will begin to display
  a rainbow pattern. After 10 seconds, the windmill and LED will stop.

  =================================================================================
                                    | You will |
  ---------------------------------------------------------------------------------
  Use the Microphone Sensor as a control device and add movement to your projects
  with the Motor.
  =================================================================================
                            | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)

  VOICE_MIN: Minimum noise level to activate the windmill and LED.
  MOTOR_SPEED_MAX: How fast the windmill spins.
  TIME_RUN: How long the windmill and LED will be on.
  BRIGHT_MAX: Max LED brightness.

  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                               | Brainstorming Ideas |
  ---------------------------------------------------------------------------------
  1. What else can you use to control the Windmill instead of the MIC Sensor?
     A button? Light Sensor? Remote control?
  2. Can you add another Motor to the other side for TWO MOTORS?!
     That'd look pretty weird.
  3. Can you modify the Windmill so it works and looks like a fan instead?
     Shouldn't be too hard. It would be a BREEZE. :)

    Note: Some of the above may require significant modification of the code.
  =================================================================================
                                | Port Connections |
  ---------------------------------------------------------------------------------
  Motor: Can connect to either port on the Motor module.
  Motor module： D5,D7,D6,D8
  MIC Sensor：A6
  ColorLED：D12
  =================================================================================
                                   | Questions? |
  ---------------------------------------------------------------------------------
  Visit us at http://www.microduino.cc/ and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/

#include <Microduino_Key.h>
#include"userDef.h"
#include"colorLed.h"
#include"motor.h"

AnalogKey keyMic(PIN_MIC);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyMic.begin(INPUT);
  MotorLeft.begin();
  MotorRight.begin();
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("MIC Val:");
  Serial.println(analogRead(PIN_MIC));
#endif
  if (keyMic.readEvent(VOICE_MIN, VOICE_MAX) == SHORT_PRESS)//If MIC Sensor detects noise, then the windmill and LED will activate.
  {
    uint32_t motorTimer = millis();
    while (millis() - motorTimer < TIME_RUN)
    {
      motorRun(MOTOR_SPEED_MAX, MOTOR_SPEED_MAX);//Active Motor.
      ledRainbow(10);//Activate rainbow LED effect.
    }
  }
  else
  {
    motorFree();//Turn off the Motor
    setAllLed(COLOR_NONE);//Turn off the LED
  }
}
