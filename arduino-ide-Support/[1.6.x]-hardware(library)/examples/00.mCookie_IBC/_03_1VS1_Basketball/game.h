#include <Microduino_Key.h>

AnalogKey keyLineA(PIN_LINEA);
AnalogKey keyLineB(PIN_LINEB);
DigitalKey keyButton(PIN_KEY);

uint8_t scoreA, scoreB, scoreCmp;
uint8_t colorNum;
uint32_t scoreTimer;

//----------ColorLED和Buzzer--------------//
void soundInit()
{
  setAllColor(COLOR_RED);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllColor(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllColor(COLOR_WARM);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllColor(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllColor(COLOR_GREEN);
  tone(PIN_BUZZER, 1500);
  delay(500);
  noTone(PIN_BUZZER);
  setAllColor(COLOR_NONE);
}

//------------得分-------------//
void updateScore()
{
  if (keyLineA.readEvent(0, DOCK_MAX) == SHORT_PRESS)
  {
    tone(PIN_BUZZER, 600, 300);
    setAllColor(COLOR_NONE);
    ledBlinkNum(1, leftColor, 0, 5);
    scoreA++;
    scoreCmp = abs(scoreA - scoreB);
  }
  else if (keyLineB.readEvent(0, DOCK_MAX) == SHORT_PRESS)
  {
    tone(PIN_BUZZER, 600, 300);
    setAllColor(COLOR_NONE);
    ledBlinkNum(1, rightColor, 1, 5);
    scoreB++;
    scoreCmp = abs(scoreA - scoreB);
  }
}

void gameReset()
{
  if (keyButton.readEvent() == LONG_PRESS)
  {
    scoreA = 0;
    scoreB = 0;
    scoreCmp = 0;
    setAllColor(COLOR_NONE);
    soundInit();
  }
}

//------------游戏结束-------------//
void gameOver()
{
  colorNum++;
  if (colorNum > 9)
    colorNum = 1;
  if (scoreA > scoreB) {
    setColor(Color[colorNum], 0);
    setColor(COLOR_RED, 1);
  }
  else {
    setColor(Color[colorNum], 1);
    setColor(COLOR_RED, 0);
  }
  delay(250);
}
