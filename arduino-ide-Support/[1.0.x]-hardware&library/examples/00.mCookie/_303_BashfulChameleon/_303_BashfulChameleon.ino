#include <Wire.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>


#define redMax 32767
#define greenMax 32767
#define blueMax 32767

int redValue,greenValue,blueValue;

#include "colorSensor.h"

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop()
{
  setTimingReg(INTEG_MODE_FREE);//Set trigger mode.Including free mode,manually mode,single synchronizition mode or so.
  setInterruptSourceReg(INT_SOURCE_GREEN); //Set interrupt source 
  setInterruptControlReg(INTR_LEVEL|INTR_PERSIST_EVERY);//Set interrupt mode
  setGain(GAIN_1|PRESCALER_4);//Set gain value and prescaler value
  setEnableADC();//Start ADC of the color sensor
  while(1)
 {
   readRGB();
   calculateCoordinate();
   delay(50);
   clearInterrupt(); 
   colorWipe(strip.Color(redValue, greenValue, blueValue), 0);
 }
}



void readRGB()
{
   Wire.beginTransmission(COLOR_SENSOR_ADDR);
   Wire.write(REG_BLOCK_READ);
   Wire.endTransmission();
 
   Wire.beginTransmission(COLOR_SENSOR_ADDR);
   Wire.requestFrom(COLOR_SENSOR_ADDR,8);
   //delay(500);
   delay(50);
   if(8<= Wire.available())    // if two bytes were received 
  { 
    for(i=0;i<8;i++)
    {
      readingdata[i]=Wire.read();
      //Serial.println(readingdata[i],BIN);
     }
  }
  green=readingdata[1]*256+readingdata[0];
  red=readingdata[3]*256+readingdata[2];
  blue=readingdata[5]*256+readingdata[4];
  clr=readingdata[7]*256+readingdata[6];
  Serial.println("The RGB value and Clear channel value are");

  Serial.println(red,DEC);
  Serial.println(green,DEC);
  Serial.println(blue,DEC);
  Serial.println(clr,DEC);  

  // redValue=map(red, 0, 29888, 0, 255);
  // greenValue=map(green, 8, 31048, 0, 255);
  // blueValue=map(blue, 0, 21888, 0, 255);

  redValue=map(red, 0, redMax, 0, 255);
  greenValue=map(green, 0, greenMax, 0, 255);
  blueValue=map(blue, 0, blueMax, 0, 255);

//  Serial.println(redValue);
//  Serial.println(greenValue);
//  Serial.println(blueValue);
//
//  Serial.println(clr);  

}



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
