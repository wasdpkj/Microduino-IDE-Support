/*
  =================================================================================                         
                          | What is: 1 VS 1 Basketball? |
  ---------------------------------------------------------------------------------                         
  Play a heads up basketball game against your friend to see who's the king of 
  shooting hoops!
  
  =================================================================================
                                   | You will |
  ---------------------------------------------------------------------------------
  Use cool sensors to create a fun and interactive game.
  =================================================================================
                                 | How to Play |
  ---------------------------------------------------------------------------------                                 
  1. The Buzzer will countdown (BUZZ, BUZZ, BUZZ). The game begins when the LEDs
     turn from red to green.
  2. Start shooting your Basketball into your own hoop!
  3. Whenever someone scores:
     a. the Buzzer will sound. 
     b. the winning player's LED will flash green by number of points ahead.
     c. the losing player's LED will flash red by number of points behind.
     Example: Player A is ahead by 2 points. Player A's LED will flash green twice.
              Player B is behind by 2 points. Player B's LED will flash red twice.
  4. When a player is ahead by 5 points, he/she wins!
     a. The winner's LED will show a Rainbow Effect.
     b. The loser's LED will show a solid red.
     c. The Buzzer will sound one last time indicating the end of the game.
  5. Hold the Crash Sensor to play again!
  =================================================================================
                           | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)
  
  SCORE_MAX: Number of points you have to be ahead by to win!
  DOCK_MAX: Line Finder minimum activation value.
  BRIGHT_MAX: Max LED brightness.
  
  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                              | Brainstorming Ideas |
  ---------------------------------------------------------------------------------                           
  1. Can you code it so that you only score points when the LED is a certain color?
     Maybe you'd even LOSE POINTS if you shoot when the color is wrong.
  2. Can you add a Motor that spins above the hoops?
     You would have to time your shots right or else the ball will get knocked away!
  3. Can you modify the structure into a soccer goal instead?
     GOOOOOOOOOOOOOOOOOOOOOOOOOAL!
     
    Note: Some of the above may require significant modification of the code.  
  =================================================================================
                               | Port Connections |
  ---------------------------------------------------------------------------------
  Crash Sensor： D6
  Buzzer: D10
  ColorLED：D12
  Line Finders：A0 and A2  
  =================================================================================
                                  | Questions? |
  ---------------------------------------------------------------------------------                                 
  Visit us at www.microduino.cc and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/

#include <Microduino_Key.h>
#include "colorLed.h"
#include "userDef.h"

Key keyLineA(PIN_LINEA, INPUT);
Key keyLineB(PIN_LINEB, INPUT);
Key keyButton(PIN_KEY, INPUT);

uint8_t scoreA, scoreB, scoreCmp;
uint8_t colorNum;
uint32_t scoreTimer;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  soundInit();
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("LINEA Val:");
  Serial.print(analogRead(PIN_LINEA));//Line Finder A Value
  Serial.print("\tLINEB Val:");
  Serial.println(analogRead(PIN_LINEB));//Line Finder B Value
#endif
  gameReset();
  if (scoreCmp < SCORE_MAX)
  {
    updateScore();
    if (millis() - scoreTimer > 800 && scoreTimer > 0)
    {
      scoreTimer = 0;
      if (scoreCmp > 0 && scoreCmp < SCORE_MAX)
        ledBlink(scoreCmp , (scoreA > scoreB));
    }
  }
  else
  {
    if (scoreA > scoreB)
      gameOver(0, 1);
    else
      gameOver(1, 0);
  }
  delay(15);
}

//----------ColorLED and Buzzer--------------//
void soundInit()
{
  setAllLed(COLOR_RED);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_WARM);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_GREEN);
  tone(PIN_BUZZER, 1500);
  delay(500);
  noTone(PIN_BUZZER);
  setAllLed(COLOR_NONE);
}

//------------ColorLED during Game-------------//
void ledBlink(int num , bool scoreFlag)
{
  for (uint8_t i = 0; i < num; i++)
  {
    uint32_t bTimer = millis();
    while (millis() - bTimer < 300)
    {
      if (scoreFlag)
      {
        setLed(COLOR_RED, 0);
        setLed(COLOR_GREEN, 1);
      }
      else
      {
        setLed(COLOR_GREEN, 0);
        setLed(COLOR_RED, 1);
      }
      if (updateScore())
        return;
    }
    bTimer = millis();
    while (millis() - bTimer < 300)
    {
      setAllLed(COLOR_NONE);
      if (updateScore())
        return;
    }
  }
}

//------------Scoring-------------//
bool updateScore()
{
  if (keyLineA.read(0, DOCK_MAX) == SHORT_PRESS)
  {
    tone(PIN_BUZZER, 600, 300);
    setAllLed(COLOR_NONE);
    scoreA++;
    scoreCmp = abs(scoreA - scoreB);
    scoreTimer = millis();
    return true;
  }
  else if (keyLineB.read(0, DOCK_MAX) == SHORT_PRESS)
  {
    tone(PIN_BUZZER, 600, 300);
    setAllLed(COLOR_NONE);
    scoreB++;
    scoreCmp = abs(scoreA - scoreB);
    scoreTimer = millis();
    return true;
  }
  return false;
}

//------------End of Game-------------//
void gameOver(int m, int n)
{
  setLed(COLOR_RED, m);
  uint32_t bTimer = millis();
  while (millis() - bTimer < 300)
  {
    if (colorNum % 2)
      tone(PIN_BUZZER, 1000);
    else
      noTone(PIN_BUZZER);
    setLed(colorNum, n);
    gameReset();
  }
  colorNum++;
  if (colorNum > 9)
    colorNum = 1;
}

void gameReset()
{
  if (keyButton.read() == LONG_PRESS)
  {
    scoreA = 0;
    scoreB = 0;
    scoreCmp = 0;
    setAllLed(COLOR_NONE);
    soundInit();
  }
}
