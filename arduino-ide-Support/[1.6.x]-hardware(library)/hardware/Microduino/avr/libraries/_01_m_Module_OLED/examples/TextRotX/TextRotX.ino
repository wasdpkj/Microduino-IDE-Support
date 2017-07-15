/*

  TextRotX.pde

  Text rotation example code.

  >>> Before compiling: Please remove comment from the constructor of the
  >>> connected graphics display (see below).

  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/

  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice, this
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


#include <U8glib.h>

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

// graphic commands to redraw the complete screen should be placed here
void draw(void) {
  u8g_uint_t mx, my;

  mx = u8g.getWidth();
  mx >>= 1;

  my = u8g.getHeight();
  my >>= 1;

  u8g.drawStr( mx, my, "Ag");
  u8g.drawStr90( mx, my, "Ag");
  u8g.drawStr180( mx, my, "Ag");
  u8g.drawStr270( mx, my, "Ag");
}

void setup(void) {
  u8g.setFont(u8g_font_9x15);
}

void change_font_pos(void) {
  static  uint8_t dir = 0;
  static  unsigned long next = 0;

  if ( next < millis() )
  {
    switch (dir) {
      case 0: u8g.setFontPosBottom(); break;
      case 1: u8g.setFontPosBaseline(); break;
      case 2: u8g.setFontPosCenter(); break;
      case 3: u8g.setFontPosTop(); break;
    }

    dir++;
    dir &= 3;
    next = millis();
    next += 1000;
  }
}

void loop(void) {
  // change the font position
  change_font_pos();

  // picture loop
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(100);
}

