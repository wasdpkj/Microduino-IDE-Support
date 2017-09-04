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
  Visit us at http://www.microduino.cc/ and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/
#include "colorLed.h"
#include "userDef.h"
#include "game.h"

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyLineA.begin(INPUT);
  keyLineB.begin(INPUT);
  keyButton.begin(INPUT_PULLUP);

#if DEBUG
  Serial.println("**************START************");
#endif
  soundInit();
}

void loop() {
#if DEBUG
  Serial.print("LINEA Val:");
  Serial.print(analogRead(PIN_LINEA));  //Line Finder A Value
  Serial.print("\tLINEB Val:");
  Serial.println(analogRead(PIN_LINEB));//Line Finder B Value
#endif
  gameReset();
  if (scoreCmp < SCORE_MAX) {
    updateScore();
  }
  else {
    if (scoreCmp == SCORE_MAX) {
      delay(1000);
      tone(PIN_BUZZER, 800);
      delay(500);
      tone(PIN_BUZZER, 1000);
      delay(500);
      tone(PIN_BUZZER, 1200);
      delay(500);
      noTone(PIN_BUZZER);
      scoreCmp++;
    }
    else {
      gameOver();
    }
  }
  delay(15);
}
