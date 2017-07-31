#include <Microduino_ColorLED.h> //Import the library for the ColorLED.
#include "userDef.h"

#define COLOR_MAX 255   //Max value for ColorLED 
#define COLOR_MIN 0     //Min value for ColorLED

uint32_t Color[11] = {
  COLOR_NONE,
  COLOR_WARM,
  COLOR_COLD,
  COLOR_RED,
  COLOR_ORANGE,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_CYAN,
  COLOR_BLUE,
  COLOR_PURPLE,
  COLOR_WHITE
};


ColorLED strip = ColorLED(LED_NUM, PIN_LED);

uint32_t rainTimer;
uint32_t breathTimer;
uint32_t blinkTimer;
bool breathFlag = true;

//------------Sets all LEDs to one color-------------//
void setAllColor(uint32_t c)
{
  for (uint8_t i = 0; i < strip.numPixels(); i++)//Select all LEDs.
  {
    strip.setPixelColor(i, c);//Set the color.
  }
  strip.show();  //Light up the LEDs to new colors.
}

//------------Sets all LEDs to one color-------------//
void setAllLed(uint8_t color)
{
  setAllColor(Color[color]);
}


//------------Sets color of one LED-------------//
void setColor(uint32_t c, uint8_t i)
{
  strip.setPixelColor(i, c);//Set the color.
  strip.show();  //Light up the LED to new color.
}

//------------Sets color of one LED-------------//
void setLed(uint8_t color, uint8_t i)
{
  setColor(Color[color], i);
}


//------------Returns different colors depending on WheelPos-------------//
uint32_t Wheel(uint8_t  WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//------------Rainbow Effect-------------//
void ledRainbow(uint8_t wait) {
  if (millis() - rainTimer > wait)
  {
    uint8_t i = (millis() / wait) & 0xFF;
    setAllColor(Wheel(i));
    rainTimer = millis();
  }
}


//------------Breathe Effect-------------//
void ledBreath(uint32_t color, uint8_t wait) {
  if (millis() - breathTimer > wait)
  {
    uint8_t i = (millis() / wait) & 0xFF;
    if (i < 2)
      breathFlag = !breathFlag;
    if (!breathFlag)
      i = 255 - i;

    strip.setBrightness(i);       //设置彩灯亮度
    setAllColor(color);
    breathTimer = millis();
  }
}

//------------Blink Effect-------------//
void ledBlink(uint8_t wait, uint32_t color, uint8_t j)
{
  if (millis() - blinkTimer > wait)
  {
    if ((millis() / wait) % 2)
      setColor(color, j);
    else
      setColor(0, j);
    blinkTimer = millis();
  }
}

//------------Blink Effect for multiple LEDs-------------//
void allLedBlinkNum(uint8_t num, uint32_t color, uint16_t wait)
{
  for (uint8_t i = 0; i < num; i++)
  {
    setAllColor(COLOR_NONE);
    delay(wait);
    setAllColor(color);
    delay(wait);
  }
  setAllLed(COLOR_NONE);
}

//------------Blink Effect for single LED-------------//
void ledBlinkNum(uint8_t num, uint32_t color, uint8_t index, uint16_t wait)
{
  for (uint8_t i = 0; i < num; i++)
  {
    setColor(COLOR_NONE, index);
    delay(wait);
    setColor(color, index);
    delay(wait);
  }
  setColor(COLOR_NONE, index);
  delay(wait);
}

