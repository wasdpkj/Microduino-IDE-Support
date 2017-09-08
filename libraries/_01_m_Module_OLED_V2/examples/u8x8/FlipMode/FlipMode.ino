/*

  FlipMode.ino
  
  Test flip functionality (180 degree roation)
  Note: Not supported with all displays

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

//Software I2C
//U8X8_SSD1306_128X64_VCOMH0_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA);   // All Boards without Reset of the Display

//Hardware I2C
U8X8_SSD1306_128X64_VCOMH0_HW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA);


void setup(void)
{
  /* U8g2 Project: Pax Instruments Shield: Enable Backlight */
  //pinMode(6, OUTPUT);
  //digitalWrite(6, 0);	
  
  u8x8.begin();
}

void loop(void)
{
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  u8x8.clear();
  u8x8.setFlipMode(0);
  u8x8.setInverseFont(1);
  u8x8.drawString(0,0,"FlipMode 0");
  u8x8.setInverseFont(0);
  u8x8.drawString(0,2,"FlipMode 0");
  delay(1500);

  u8x8.clear();
  u8x8.setFlipMode(1);
  u8x8.setInverseFont(1);
  u8x8.drawString(0,0,"FlipMode 1");
  u8x8.setInverseFont(0);
  u8x8.drawString(0,3,"FlipMode 1");
  delay(1500);

}
