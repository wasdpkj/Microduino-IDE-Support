/*
  =================================================================================                         
                            | What is: Reflex Battle? |
  ---------------------------------------------------------------------------------                         
  Ever wonder whether you or your friend Bob has faster reflexes? Make this project
  and find out! Challenge your friends, parents, teachers...or even your dog. It'll
  be the showdown of the century.
    
  =================================================================================
                                  | How to Play |
  ---------------------------------------------------------------------------------
  1. Each player has a finger on a Crash Sensor.
  2. Important: Make sure to follow the connection diagram EXACTLY like in the 
     instruction booklet. Each Crash Sensor also corresponds specifically to one of
     the ColorLEDs.
  3. Hold down one of the buttons to begin the game!
  4. The ColorLEDs will flash and buzz RED, RED, GREEN. The game begins after the 
     last buzz.
  5. Press your Crash sensor when the colors match! The faster player receives a point.
     If you press the Crash sensor when the colors do not match, the other player receives a point.
       a. The LED will flash green for the faster player if the colors match.
       b. The LED will flash red for the faster player if the colors DO NOT match. 
       c. Then, the LED will flash white by the number of points a player is ahead.
    For example, if Player B is ahead by 3 points, his LED will flash white 3 times.
  6. The player who is 5 points ahead wins.
  7. Music will play once a player wins!
  
           NOTE: Hold down a button at any time to restart the game.
  =================================================================================
                                    | You will |
  ---------------------------------------------------------------------------------
  Use sensors to create a fun and interactive game.
  =================================================================================
                           | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change below values to your own liking. :)
  
  BRIGHT_MAX: Max LED brightness.
  LED_INTERVAL: Speed at which the colors change.
  SCORE_MAX: Victory condition.
  
  Remember to upload the code into the mBattery again after changing the values!
  =================================================================================
                              | Brainstorming Ideas |
  ---------------------------------------------------------------------------------                           
  1. Can you modify the code to change the type of competition?
     For example, "Who can press the button the most times in 10 seconds?"
  2. Can you modify the code so that you only press the Crash Sensor when the 
     ColorLEDs are DIFFERENT colors?
  3. Can you modify the code to create a Simon Says game?
     You could use two ColorLEDs to play a sequence of left and right. Then, you 
     have to repeat the sequence with two Crash Sensors! 
       
    Note: Some of the above may require significant modification of the code.  
  =================================================================================
                               | Port Connections |
  ---------------------------------------------------------------------------------
  Buzzer: D10
  ColorLED: D12
  Crash Sensor A: D6
  Crash Sensor B: D8 
  =================================================================================
                                  | Questions? |
  ---------------------------------------------------------------------------------                                 
  Visit us at www.microduino.cc and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/

#include <Microduino_Key.h>
#include "colorLed.h"
#include "music.h"
#include "userDef.h"

Key keyA(PIN_KEYA, INPUT);
Key keyB(PIN_KEYB, INPUT);

uint32_t ledTimer;
uint8_t scoreA, scoreB;
int8_t score;
uint8_t ledX, ledY;
bool playStatus;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  randomSeed(80);
  soundInit();
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
  if (playStatus)//Are you in the middle of a game?
  {
    int8_t scoreCache = score;
    score = updateScore();  //Calculate difference of points.
    if (abs(score) >= SCORE_MAX)  //Game ends.
    {
      playStatus = false;
      playIndex = 0;
    }
    else if (score != scoreCache)
    {
      ledScore(score);  //Blink the difference of points.
    }
    updateLed();//Flash the new LED color.
  }
  else if (!playSound(10))//Play game over music.
  {
    noTone(PIN_BUZZER);
    ledBreath(COLOR_GREEN, 15);
    if (playReset())
      playStatus = true;
  }
}

//--------------Flash a New LED Color-----------------//
void updateLed()
{
  if (millis() > ledTimer)
  {
    if (ledX == 0 || ledY == 0)
    {
      ledX = random(5,9);
      ledY = random(5,9);
      ledTimer = millis() + random(LED_INTERVAL - 200, LED_INTERVAL);//Flash for a certain amount of time.
    }
    else
    {
      ledX = 0;
      ledY = 0;
      ledTimer = millis() + LED_INTERVAL / 2;   //Stay dark for a certain amount of time.
    }
    setLed(ledX, 0);
    setLed(ledY, 1);
  }
}

void soundInit()
{
  setAllLed(COLOR_RED);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_RED);
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
//---------------Blink the Difference of Points-----------------//
void ledScore(int8_t _score)
{
  setAllLed(COLOR_NONE);
  if (_score > 0)
    ledBlinkNum(_score, COLOR_COLD, 0, 300);
  else if (_score < 0)
    ledBlinkNum(abs(_score), COLOR_COLD, 1, 300);
}
//---------------Calculate the Scores-----------------//
int8_t updateScore()
{
  if (keyA.read() == SHORT_PRESS)
  {
    setLed(COLOR_NONE, 1);
    if (ledX == ledY && ledX > 0)
    {
      tone(PIN_BUZZER, 500, 300);
      scoreA++;
      ledBlinkNum(1, COLOR_GREEN, 0, 600);
    }
    else
    {
      tone(PIN_BUZZER, 1000, 300);
      scoreB++;
      ledBlinkNum(1, COLOR_RED, 0, 600);
    }
    ledX = 0;
    ledY = 0;
  }
  else if (keyB.read() == SHORT_PRESS)
  {
    setLed(COLOR_NONE, 0);
    if (ledX == ledY && ledX > 0)
    {
      tone(PIN_BUZZER, 500, 300);
      scoreB++;
      ledBlinkNum(1, COLOR_GREEN, 1, 600);
    }
    else
    {
      tone(PIN_BUZZER, 1000, 300);
      scoreA++;
      ledBlinkNum(1, COLOR_RED, 1, 600);
    }
    ledX = 0;
    ledY = 0;
  }
  return (scoreA - scoreB);
}

bool playReset()
{
  if (keyA.read() == LONG_PRESS || keyB.read() == LONG_PRESS)
  {
    scoreA = 0;
    scoreB = 0;
    soundInit();
    return true;
  }
  else
    return false;
}
