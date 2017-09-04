/*
  =================================================================================
                             | What is: Music Box? |
  ---------------------------------------------------------------------------------
  Use a card with black bars and swipe into the Music Box to play its corresponding
  song. One black bar is Song #1, two black bars is Song #2, and so on. There are a
  total of 11 different songs in this program, so you can make your own card and
  draw anywhere from 1 to 11 black bars to play 11 different songs!

  Feel free to add your own tunes into the music.h file.

  If it is not working correctly, try swiping a little slower.

  =================================================================================
                                   | You will |
  ---------------------------------------------------------------------------------
  Use the Line Finder Sensor like a barcode scanner to play fun musical tunes.
  =================================================================================
                           | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)

  DOCK_MAX: Line Finder minimum activation value.
  BRIGHT_MAX: Max LED brightness.

  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                              | Brainstorming Ideas |
  ---------------------------------------------------------------------------------
  1. Can you modify the songs in "music.h" or even add your own?
  2. Can you add two Crash Sensors instead to control the music with buttons?
     One would be PREVIOUS SONG and the other would be NEXT SONG.

    Note: Some of the above may require significant modification of the code.
  =================================================================================
                               | Port Connections |
  ---------------------------------------------------------------------------------
  Buzzer: D10
  ColorLED：D12
  Line Finder：A0
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

AnalogKey keyLine(PIN_LINEA);

uint32_t cardTimer = 0;
uint8_t cardNum = 0;
uint8_t playNum = 0;

void getCard()
{
  if (keyLine.readEvent(0, DOCK_MAX) == SHORT_PRESS)
  {
    setAllColor(COLOR_BLUE);
    cardNum++;
    cardTimer = millis();
  }
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyLine.begin(INPUT);
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("LINE Val:");
  Serial.println(analogRead(PIN_LINEA));//Intensity of darkness detected by Line Finder.
#endif
  getCard();
  if (millis() - cardTimer > 1000 && cardTimer > 0) //You have one second to scan the whole card.
  {
    noTone(PIN_BUZZER);
    setAllColor(COLOR_NONE);
    playNum = constrain(cardNum - 1, 0, SONG_SUM - 1); //Plays the corresponding song according to number of black bars scanned.
    cardNum = 0;
    playIndex = 0;
    cardTimer = 0;
    allLedBlinkNum(playNum, COLOR_WARM, 500);
  }
  if (!playSound(playNum))//Play music.
  {
    noTone(PIN_BUZZER);
    ledBreath(COLOR_GREEN, 15);
  }
  delay(15);
}
