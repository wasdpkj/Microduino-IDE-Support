/*

  Console.pde
  
  Read from serial monitor, output to display
  
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/
  
  Copyright (c) 2011, olikraus@gmail.com
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


#include <U8glib.h>

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

// setup input buffer
#define LINE_MAX 30 
uint8_t line_buf[LINE_MAX] = "U8GLIB Console";
uint8_t line_pos = 0;

// setup a text screen to support scrolling
#define ROW_MAX 12


uint8_t screen[ROW_MAX][LINE_MAX];
uint8_t rows, cols;

// line height, which matches the selected font (5x7)
#define LINE_PIXEL_HEIGHT 7

// clear entire screen, called during setup
void clear_screen(void) {
  uint8_t i, j;
  for( i = 0; i < ROW_MAX; i++ )
    for( j = 0; j < LINE_MAX; j++ )
      screen[i][j] = 0;  
}

// append a line to the screen, scroll up
void add_line_to_screen(void) {
  uint8_t i, j;
  for( j = 0; j < LINE_MAX; j++ )
    for( i = 0; i < rows-1; i++ )
      screen[i][j] = screen[i+1][j];
  
  for( j = 0; j < LINE_MAX; j++ )
    screen[rows-1][j] = line_buf[j];
}

// U8GLIB draw procedure: output the screen
void draw(void) {
  uint8_t i, y;
  // graphic commands to redraw the complete screen are placed here    
  y = 0;       // reference is the top left -1 position of the string
  y--;           // correct the -1 position of the drawStr 
  for( i = 0; i < rows; i++ )
  {
    u8g.drawStr( 0, y, (char *)(screen[i]));
    y += u8g.getFontLineSpacing();
  }
}

void exec_line(void) {
  // echo line to the serial monitor
  Serial.println((const char *)line_buf);
  
  // add the line to the screen
  add_line_to_screen();
  
  // U8GLIB picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}

// clear current input buffer
void reset_line(void) { 
      line_pos = 0;
      line_buf[line_pos] = '\0';  
}

// add a single character to the input buffer 
void char_to_line(uint8_t c) {
      line_buf[line_pos] = c;
      line_pos++;
      line_buf[line_pos] = '\0';  
}

// check serial in and handle the character
void read_line(void) {
  if ( Serial.available() )
  {
    uint8_t c;
    c = Serial.read();
    if ( line_pos >= cols-1 ) {
      exec_line();
      reset_line();
      char_to_line(c);
    } 
    else if ( c == '\n' ) {
      // ignore '\n' 
    }
    else if ( c == '\r' ) {
      exec_line();
      reset_line();
    }
    else {
      char_to_line(c);
    }
  }
}

// Arduino master setup
void setup(void) {
  // set font for the console window
  u8g.setFont(u8g_font_5x7);
  //u8g.setFont(u8g_font_9x15);
  
  // set upper left position for the string draw procedure
  u8g.setFontPosTop();
  
  // calculate the number of rows for the display
  rows = u8g.getHeight() / u8g.getFontLineSpacing();
  if ( rows > ROW_MAX )
    rows = ROW_MAX; 
  
  // estimate the number of columns for the display
  cols = u8g.getWidth() / u8g.getStrWidth("m");
  if ( cols > LINE_MAX-1 )
    cols = LINE_MAX-1; 
  
  clear_screen();               // clear screen
  delay(1000);                  // do some delay
  Serial.begin(9600);        // init serial
  exec_line();                    // place the input buffer into the screen
  reset_line();                   // clear input buffer
}

// Arduino main loop
void loop(void) {
  read_line();
}

