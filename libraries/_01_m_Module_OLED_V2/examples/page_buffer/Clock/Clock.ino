/*

  Clock.ino
  
  Demonstrates a clock-like display with a very huge font.
  Also shows usage of u8x8_u8toa().

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
    
  This is a page buffer example.    
*/


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

  /* U8g2 Project: SSD1306 Test Board */
  //pinMode(10, OUTPUT);
  //pinMode(9, OUTPUT);
  //digitalWrite(10, 0);
  //digitalWrite(9, 0);		

  /* U8g2 Project: T6963 Test Board */
  //pinMode(18, OUTPUT);
  //digitalWrite(18, 1);	

  /* U8g2 Project: KS0108 Test Board */
  //pinMode(16, OUTPUT);
  //digitalWrite(16, 0);	

  /* U8g2 Project: LC7981 Test Board, connect RW to GND */
  //pinMode(17, OUTPUT);
  //digitalWrite(17, 0);	

  /* U8g2 Project: Pax Instruments Shield: Enable Backlight */
  //pinMode(6, OUTPUT);
  //digitalWrite(6, 0);	

  u8g2.begin();  
}

uint8_t m = 24;

void loop(void) {
  char m_str[3];
  strcpy(m_str, u8x8_u8toa(m, 2));		/* convert m to a string with two digits */
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso62_tn);
    u8g2.drawStr(0,63,"9");
    u8g2.drawStr(33,63,":");
    u8g2.drawStr(50,63,m_str);
  } while ( u8g2.nextPage() );
  delay(1000);
  m++;
  if ( m == 60 )
    m = 0;
}

