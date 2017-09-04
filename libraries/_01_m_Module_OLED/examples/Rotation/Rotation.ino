/*

  HelloWorld.pde

  "Hello World!" example code.

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

uint8_t offset = 0;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0 + 0, 20 + 0, "Hello!");
  u8g.drawStr( 0 + 2, 20 + 16, "Hello!");

  u8g.drawBox(0, 0, 3, 3);
  u8g.drawBox(u8g.getWidth() - 6, 0, 6, 6);
  u8g.drawBox(u8g.getWidth() - 9, u8g.getHeight() - 9, 9, 9);
  u8g.drawBox(0, u8g.getHeight() - 12, 12, 12);
}

void setup(void) {
}


void rotate(void) {
  static  uint8_t dir = 0;
  static  unsigned long next_rotation = 0;

  if ( next_rotation < millis() )
  {
    switch (dir) {
      case 0: u8g.undoRotation(); break;
      case 1: u8g.setRot90(); break;
      case 2: u8g.setRot180(); break;
      case 3: u8g.setRot270(); offset = ( offset + 1 ) & 0x0f; break;
    }

    dir++;
    dir &= 3;
    next_rotation = millis();
    next_rotation += 1000;
  }
}

void loop(void) {
  // screen rotation
  rotate();

  // picture loop
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(100);
}

