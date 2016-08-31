/*
  =================================================================================                         
                              | What is: Nightlight? |
  ---------------------------------------------------------------------------------
  Scared of the dark? Fear no more.
  This is an automatic nightlight that does its own thing!

  When the surroundings are bright: 
  The nightlight will be off.
  
  When the surroundings are dark: 
  The nightlight will turn on when there is noise.

  Pressing the Crash Sensor at any time (doesn't matter if it's light or dark outside)
  will also turn on the nightlight. The light will remain on for 10 seconds.
  
  =================================================================================
                                    | You will |
  ---------------------------------------------------------------------------------
  Use sensors to automate things in a fun and unique way.
  =================================================================================
                            | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)
  
  VOICE_MIN: Minimum noise level to activate the LED.
  LIGHT_MAX: Sets when the Nightlight will "think" it's night time.
  TIME_RUN: How long the LED will be on.
  BRIGHT_MAX: Max LED brightness.
  
  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                               | Brainstorming Ideas |
  ---------------------------------------------------------------------------------
  1. What else can you use to control the Nightlight? 
     A built-clock? For example, it will start working everyday at 6pm.
  2. Can you add a Motor and attach it to the LEDs?
     You'd be sure to scare your parents in the middle of the night when they get up 
     to use the restroom!
  3. Can you add a Buzzer so it works like an alarm detector?
     Again, this would probably scare you parents, too. :)
     
    Note: Some of the above may require significant modification of the code.  
  =================================================================================
                                | Port Connections |
  ---------------------------------------------------------------------------------
  Light Sensor: A2
  Microphone Sensor: A6
  ColorLED: D12
  Crash Sensor: D6
  =================================================================================
                                   | Questions? |
  ---------------------------------------------------------------------------------
  Visit us at www.microduino.cc and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/

#include <Microduino_Key.h>
#include"colorLed.h"
#include"userDef.h"

Key keyButton(PIN_KEY, INPUT);
Key keyMic(PIN_MIC, INPUT);
Key keyLight(PIN_LIGHT, INPUT);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("LIGHT Val:");
  Serial.print(analogRead(PIN_LIGHT));
  Serial.print("\tMIC Val:");
  Serial.println(analogRead(PIN_MIC));
#endif

  if (keyLight.read(LIGHT_MIN, LIGHT_MAX) == LONG_PRESS && keyMic.read(VOICE_MIN, VOICE_MAX) == SHORT_PRESS
      || keyButton.read() == SHORT_PRESS)
  {
    uint32_t ledonTime = millis();
    while (millis() - ledonTime < TIME_ON)
      //rainbow(10);
      //ledBreath(COLOR_PURPLE, 10);
      setAllLed(COLOR_WARM);
  }
  else
    setAllLed(COLOR_NONE);
  delay(15);
}
