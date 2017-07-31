/*
  =================================================================================
                              | What is: Piggy Bank? |
  ---------------------------------------------------------------------------------
  Saving money doesn't have to be a chore! Every time you drop your coins into the
  Piggy Bank, a tune and lightshow will follow. Doesn't it feel good to be a small
  step closer to getting that next thing you wanted?

  =================================================================================
                                    | You will |
  ---------------------------------------------------------------------------------
  Use sensors and lights to make an everyday object way more interesting.
  =================================================================================
                            | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)

  LINE_MAX: Line Finder minimum activation value.
  BRIGHT_MAX: Max LED brightness.

  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                               | Brainstorming Ideas |
  ---------------------------------------------------------------------------------
  1. Can you add another coin slot/Line Finder?
     One slot could be for quarters only and the other slot is for dimes.
  2. Can you modify the code to play a special song when you save up a certain amount?
  3. Can you add a motor in the Piggy Bank?
     Not sure why you would do that, but it'd be cool to hear the coins jingle around!

    Note: Some of the above may require significant modification of the code.
  =================================================================================
                                | Port Connections |
  ---------------------------------------------------------------------------------
  ColorLED: D12
  Buzzer: D10
  Line Finder: A0
  =================================================================================
                                   | Questions? |
  ---------------------------------------------------------------------------------
  Visit us at http://www.microduino.cc/ and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/
#include <Microduino_Key.h>
#include "colorLed.h"
#include "music.h"
#include "userDef.h"

AnalogKey keyCoin(PIN_LINEA);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyCoin.begin(INPUT);
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("LINE Val:");
  Serial.println(analogRead(PIN_LINEA));//Intensity of darkness detected by Line Finder.
#endif
  if (keyCoin.readEvent(0, LINE_MAX) == SHORT_PRESS)//Check if a coin was dropped.
    playIndex = 0;//Resume music.

  if (!playSound(1))//Play music.
  {
    noTone(PIN_BUZZER);
    ledBreath(COLOR_GREEN, 15);
    //setAllLed(COLOR_NONE);
  }
}
