#include <Adafruit_NeoPixel.h>//Import the library for the ColorLED.
#include "userDef.h"

#define COLOR_MAX 255   //Max value for ColorLED 
#define COLOR_MIN 0     //Min value for ColorLED

#define COLOR_NONE    0    //colorLED OFF
#define COLOR_WARM    1    
#define COLOR_COLD    2    
#define COLOR_RED     3    
#define COLOR_ORANGE  4    
#define COLOR_YELLOW  5  
#define COLOR_GREEN   6   
#define COLOR_BLUE    7   
#define COLOR_INDIGO  8  
#define COLOR_PURPLE  9  
#define COLOR_WHITE   10 


//Array to hold preset colors.
const uint16_t colorArray[10][3] = {
  {0,   0,   0   },        //colorLED OFF
  {155, 100, 0   },        //Warm
  {100, 130, 100 },        //Cold
  {255, 0,   0   },        //Red
  {234, 139, 23  },        //Orange
  {255, 255, 0   },        //Yellow
  {0,   255, 0   },        //Green
  {0,   255, 255 },        //Lime
  {0,   0,   255 },        //Blue
  {255, 0,   255 },        //Purple
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, PIN_LED, NEO_GRB + NEO_KHZ800);

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
  setAllColor(strip.Color(colorArray[color][0], colorArray[color][1], colorArray[color][2]));
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
  setColor(strip.Color(colorArray[color][0], colorArray[color][1], colorArray[color][2]), i);
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
void ledBreath(uint8_t color, uint8_t wait) {
  if (millis() - breathTimer > wait)
  {
    uint8_t i = (millis() / wait) & 0xFF;
    if (i == 0)
      breathFlag = !breathFlag;
    if (!breathFlag)
      i = 255 - i;
    setAllColor(strip.Color(map(i, COLOR_MIN, COLOR_MAX, 0, colorArray[color][0]), map(i, COLOR_MIN, COLOR_MAX, 0, colorArray[color][1]), map(i, COLOR_MIN, COLOR_MAX, 0, colorArray[color][2])));
    breathTimer = millis();
  }
}

//------------Blink Effect-------------//
void ledBlink(uint8_t wait, uint8_t color, uint8_t j)
{
  if (millis() - blinkTimer > wait)
  {
    if ((millis() / wait) % 2)
      setLed(color, j);
    else
      setLed(0, j);
    blinkTimer = millis();
  }
}

//------------Blink Effect for multiple LEDs-------------//
void allLedBlinkNum(uint8_t num, uint8_t color, uint16_t wait)
{
  for (uint8_t i = 0; i < num; i++)
  {
    setAllLed(COLOR_NONE);
    delay(wait);
    setAllLed(color);
    delay(wait);
  }
  setAllLed(COLOR_NONE);
}

//------------Blink Effect for single LED-------------//
void ledBlinkNum(uint8_t num, uint8_t color, uint8_t index, uint16_t wait)
{
  for (uint8_t i = 0; i < num; i++)
  {
    setLed(COLOR_NONE, index);
    delay(wait);
    setLed(color, index);
    delay(wait);
  }
  setLed(COLOR_NONE, index);
  delay(wait);
}

