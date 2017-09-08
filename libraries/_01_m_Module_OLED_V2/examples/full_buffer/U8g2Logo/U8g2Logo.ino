/*

  U8g2Logo.ino

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

/*
  U8glib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a frame buffer example.    
*/

//Software I2C
//U8G2_SSD1306_128X64_VCOMH0_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA);   // All Boards without Reset of the Display
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C
//U8G2_SSD1306_128X64_VCOMH0_F_SW_I2C	//可通过setContrast()设置亮度


//Hardware I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C
//U8G2_SSD1306_128X64_VCOMH0_F_HW_I2C	//可通过setContrast()设置亮度


//#define MINI_LOGO

void setup(void) {
  pinMode(9, OUTPUT);
  digitalWrite(9, 0);	// default output in I2C mode for the SSD1306 test shield: set the i2c adr to 0

  u8g2.begin();
}

void drawLogo(void)
{
    u8g2.setFontMode(1);	// Transparent
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36,22,"g");
    u8g2.drawStr(48,22,"\xb2");
    
    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30,"g");
    u8g2.drawStr(67,30,"\xb2");
    
    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);
    
#endif
}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g2.setFont(u8g2_font_4x6_tr);
  if ( u8g2.getDisplayHeight() < 59 )
  {
    u8g2.drawStr(89,20,"github.com");
    u8g2.drawStr(73,29,"/olikraus/u8g2");
  }
  else
  {
    u8g2.drawStr(1,54,"github.com/olikraus/u8g2");
  }
#endif
}

void loop(void) {
  u8g2.clearBuffer();
  drawLogo();
  drawURL();
  u8g2.sendBuffer();
  delay(1000);
}


