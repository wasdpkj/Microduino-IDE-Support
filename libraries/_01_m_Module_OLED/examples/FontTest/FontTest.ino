/*
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


//-------字体设置，大、中、小
#define setFont_L u8g_font_timB14
#define setFont_M u8g_font_9x15
#define setFont_S u8g_font_fixed_v0r
/*
  font:
  u8g_font_timB18
  u8g_font_timB14
  u8g_font_timB10
  u8g_font_5x7
  u8g_font_6x10
  u8g_font_7x13
  u8g_font_9x15
  u8g_font_unifont
  u8g_font_fixed_v0r
  u8g_font_chikitar
  number:
  u8g_font_freedoomr25n
*/


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(setFont_L);
  u8g.setPrintPos(0, 30);
  u8g.print("Font_L:0123");

  u8g.setFont(setFont_M);
  u8g.setPrintPos(0, 50);
  u8g.print("Font_M:0123");

  u8g.setFont(setFont_S);
  u8g.setPrintPos(0, 60);
  u8g.print("Font_S:0123");
}

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
}

void loop(void) {
  // picture loop
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(500);
}
