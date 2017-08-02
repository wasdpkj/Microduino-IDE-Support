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
  Visit us at http://www.microduino.cc/ and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/
#include "colorLed.h"
#include "userDef.h"
#include "music.h"
#include "game.h"

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyA.begin(INPUT_PULLUP);
  keyB.begin(INPUT_PULLUP);
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
  if (playStatus)           //Are you in the middle of a game?
  {
    int8_t scoreCache = score;
    score = updateScore();  //Calculate difference of points.
    playReset();
    if (abs(score) >= SCORE_MAX)  //Game ends.
    {
      playStatus = false;
      playgo = true;
    }
    else if (score != scoreCache)
      ledScore(score);//Blink the difference of points.
    updateLed();//Flash the new LED color.
  }
  else if (!playgo)
  {
    ledBreath(COLOR_PURPLE, 3);
    playReset();
  }
  else if (!playStatus)
    gameOver();
  delay(15);
}
