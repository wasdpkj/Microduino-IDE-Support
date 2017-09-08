/*

  PowerSaveTest.ino

  This test will turn on and off your display for two seconds.
  If the text "power off" is visible on your display, then power save mode
  does not work with your display.
  
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
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//Software I2C
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA);   // All Boards without Reset of the Display
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C
//U8G2_SSD1306_128X64_NONAME_2_SW_I2C
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C
//U8G2_SSD1306_128X64_VCOMH0_1_SW_I2C	//可通过setContrast()设置亮度
//U8G2_SSD1306_128X64_VCOMH0_2_SW_I2C	//可通过setContrast()设置亮度
//U8G2_SSD1306_128X64_VCOMH0_F_SW_I2C	//可通过setContrast()设置亮度


//Hardware I2C
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C
//U8G2_SSD1306_128X64_NONAME_2_HW_I2C
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C
//U8G2_SSD1306_128X64_VCOMH0_1_HW_I2C	//可通过setContrast()设置亮度
//U8G2_SSD1306_128X64_VCOMH0_2_HW_I2C	//可通过setContrast()设置亮度
//U8G2_SSD1306_128X64_VCOMH0_F_HW_I2C	//可通过setContrast()设置亮度


void setup(void) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
}


void draw(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.drawStr(2,15,"PowerSaveTest");    
    u8g2.drawStr(2,30,s);    
    u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight() );
  } while ( u8g2.nextPage() );
  delay(2000);
}

void loop(void) {
  u8g2.setPowerSave(0);
  draw("power on");
  u8g2.setPowerSave(1);
  draw("power off");
}

