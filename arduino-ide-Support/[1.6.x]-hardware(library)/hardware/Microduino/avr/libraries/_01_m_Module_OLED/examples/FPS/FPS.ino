/*
  FPS.pde

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
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

#define SECONDS 10
uint8_t flip_color = 0;
uint8_t draw_color = 1;

void draw_set_screen(void) {
  u8g.setColorIndex(flip_color);
  u8g.drawBox( 0, 0, u8g.getWidth(), u8g.getHeight() );
}

void draw_char(void) {
  char buf[2] = "@";
  u8g_uint_t i, j;
  u8g.setColorIndex(draw_color);
  u8g.setFont(u8g_font_6x10);
  j = 8;
  for (;;) {
    i = 0;
    for (;;) {
      u8g.drawStr( i, j, buf);
      i += 8;
      if ( i > u8g.getWidth() )
        break;
    }
    j += 8;
    if ( j > u8g.getHeight() )
      break;
  }

}

void draw_pixel(void) {
  u8g_uint_t x, y, w2, h2;
  u8g.setColorIndex(draw_color);
  w2 = u8g.getWidth();
  h2 = u8g.getHeight();
  w2 /= 2;
  h2 /= 2;
  for ( y = 0; y < h2; y++ ) {
    for ( x = 0; x < w2; x++ ) {
      if ( (x + y) & 1 ) {
        u8g.drawPixel(x, y);
        u8g.drawPixel(x, y + h2);
        u8g.drawPixel(x + w2, y);
        u8g.drawPixel(x + w2, y + h2);
      }
    }
  }
}

// returns unadjusted FPS
uint16_t picture_loop_with_fps(void (*draw_fn)(void)) {
  uint16_t FPS10 = 0;
  uint32_t time;

  time = millis() + SECONDS * 1000;

  // picture loop
  do {
    u8g.firstPage();
    do {
      draw_fn();
    } while ( u8g.nextPage() );
    FPS10++;
    flip_color = flip_color ^ 1;
  } while ( millis() < time );
  return FPS10;
}

const char *convert_FPS(uint16_t fps) {
  static char buf[6];
  strcpy(buf, u8g_u8toa( (uint8_t)(fps / 10), 3));
  buf[3] =  '.';
  buf[4] = (fps % 10) + '0';
  buf[5] = '\0';
  return buf;
}

void show_result(const char *s, uint16_t fps) {
  // assign default color value
  u8g.setColorIndex(draw_color);
  u8g.setFont(u8g_font_7x13);
  u8g.firstPage();
  do {
    u8g.drawStr(0, 12, s);
    u8g.drawStr(0, 24, convert_FPS(fps));
  } while ( u8g.nextPage() );
}

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();

  draw_color = 1;         // pixel on
}

void loop(void) {
  uint16_t fps;
  fps = picture_loop_with_fps(draw_set_screen);
  show_result("clear screen", fps);
  delay(5000);
  fps = picture_loop_with_fps(draw_char);
  show_result("draw @", fps);
  delay(5000);
  fps = picture_loop_with_fps(draw_pixel);
  show_result("draw pixel", fps);
  delay(5000);
}