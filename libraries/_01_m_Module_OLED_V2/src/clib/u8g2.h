/*

  u8g2.h

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


  call sequence
  
  u8g2_SetupBuffer_XYZ
    u8x8_Setup_XYZ
      u8x8_SetupDefaults(u8g2);
      assign u8x8 callbacks
      u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SETUP_MEMORY, 0, NULL);  
    setup tile buffer
    
  
  Arduino Uno Text Example
>	FONT_ROTATION	INTERSECTION	CLIPPING	text	   	data		bss		dec		hex	
>																		8700
>	x				x				x			7450	104		1116	8670	21de
>	-				x				x			7132	104		1115	8351	209f
>	x				-				x			7230	104		1116	8450	2102
>	-				-				x			7010	104		1115	8229	2025
>	-				-				-			6880	104		1115	8099	1fa3
  
  
*/


#ifndef _U8G2_H
#define _U8G2_H

#include "u8x8.h"


/*
  The following macro enables 16 Bit mode. 
  Without defining this macro all calulations are done with 8 Bit (1 Byte) variables.
  Especially on AVR architecture, this will save some space. 
  If this macro is defined, then U8g2 will switch to 16 Bit mode.
  Use 16 Bit mode for any display with more than 240 pixel in one 
  direction.
*/
//#define U8G2_16BIT


/*
  The following macro enables a special check and optimization
  in the HVLine procedure for lines with one pixel length.
  It enabled, it will consume about 60 bytes in flash memory of the AVR and
  there will be a speed improvement of about 20% for any single pixel draw.
*/
#define U8G2_WITH_ONE_PIXEL_OPTIMIZATION


/*
  The following macro enables the HVLine speed optimization.
  It will consume about 40 bytes more in flash memory of the AVR.
  HVLine procedures are also used by the text drawing functions.
*/
#define U8G2_HVLINE_SPEED_OPTIMIZATION

/*
  The following macro enables all four drawing directions for glyphs and strings.
  If this macro is not defined, than a string can be drawn only in horizontal direction.
*/
#define U8G2_WITH_FONT_ROTATION

/*
  The following macro activates the early intersection check with the current visible area.
  Clipping (and low level intersection calculation) will still happen and is controlled by U8G2_WITH_CLIPPING.
  This early intersection check only improves speed for the picture loop (u8g2_FirstPage/NextPage).
  With a full framebuffer in RAM and if most graphical elements are drawn within the visible area, then this
  macro can be commented to reduce code size.
*/
#define U8G2_WITH_INTERSECTION

/*
  U8glib V2 contains support for unicode plane 0 (Basic Multilingual Plane, BMP).
  The following macro activates this support. Deactivation would save some ROM.
  This definition also defines the behavior of the expected string encoding.
  If the following macro is defined, then the DrawUTF8 function is enabled and 
  the string argument for this function is assumed 
  to be UTF-8 encoded.
  If the following macro is not defined, then all strings in the c-code are assumed 
  to be ISO 8859-1/CP1252 encoded. 
  Independently from this macro, the Arduino print function never accepts UTF-8
  strings.
  
  This macro does not affect the u8x8 string draw function.
  u8x8 has also two function, one for pure strings and one for UTF8
  
  Conclusion:
    U8G2_WITH_UNICODE defined
      - C-Code Strings must be UTF-8 encoded
      - Full support of all 65536 glyphs of the unicode basic multilingual plane
      - Up to 65536 glyphs of the font file can be used.
    U8G2_WITH_UNICODE not defined
      - C-Code Strings are assumbed to be ISO 8859-1/CP1252 encoded
      - Only character values 0 to 255 are supported in the font file.
*/
#define U8G2_WITH_UNICODE


/*
  Internal performance test for the effect of enabling U8G2_WITH_INTERSECTION
  Should not be defined for production code
*/
//#define U8G2_WITH_HVLINE_COUNT

/*
  Defining the following variable adds the clipping and check procedures agains the display boundaries.
  Clipping procedures are mandatory for the picture loop (u8g2_FirstPage/NextPage).
  Clipping procedures will also allow strings to exceed the display boundaries.
  On the other side, without clipping, all the setting of pixels must happen within the display boundaries.
  
  WARNING: Adding a comment in front of the following macro or removing the following line
  may lead to memory faults if you write any pixel outside the display boundary.
*/
#define U8G2_WITH_CLIPPING




/*==========================================*/


#ifdef __GNUC__
#  define U8G2_NOINLINE __attribute__((noinline))
#else
#  define U8G2_NOINLINE
#endif

#define U8G2_FONT_SECTION(name) U8X8_FONT_SECTION(name) 


/* the macro U8G2_USE_LARGE_FONTS disables large fonts (>32K) */
/* it can be enabled for those uC supporting larger arrays */
#if defined(__arm__) || defined(__arc__) || defined(ESP8266) || defined(ESP32) || defined(ESP_PLATFORM) || defined(LE501X) 
#define U8G2_USE_LARGE_FONTS
#endif

/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================*/

#ifdef U8G2_16BIT
typedef uint16_t u8g2_uint_t;	/* for pixel position only */
typedef int16_t u8g2_int_t;		/* introduced for circle calculation */
typedef int32_t u8g2_long_t;		/* introduced for ellipse calculation */
#else
typedef uint8_t u8g2_uint_t;		/* for pixel position only */
typedef int8_t u8g2_int_t;		/* introduced for circle calculation */
typedef int16_t u8g2_long_t;		/* introduced for ellipse calculation */
#endif


typedef struct u8g2_struct u8g2_t;
typedef struct u8g2_cb_struct u8g2_cb_t;

typedef void (*u8g2_update_dimension_cb)(u8g2_t *u8g2);
typedef void (*u8g2_draw_l90_cb)(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);
typedef void (*u8g2_draw_ll_hvline_cb)(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);

typedef uint8_t (*u8g2_get_kerning_cb)(u8g2_t *u8g2, uint16_t e1, uint16_t e2);


/* from ucglib... */
struct _u8g2_font_info_t
{
  /* offset 0 */
  uint8_t glyph_cnt;
  uint8_t bbx_mode;
  uint8_t bits_per_0;
  uint8_t bits_per_1;
  
  /* offset 4 */
  uint8_t bits_per_char_width;
  uint8_t bits_per_char_height;		
  uint8_t bits_per_char_x;
  uint8_t bits_per_char_y;
  uint8_t bits_per_delta_x;
  
  /* offset 9 */
  int8_t max_char_width;
  int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
  int8_t x_offset;
  int8_t y_offset;
  
  /* offset 13 */
  int8_t  ascent_A;
  int8_t  descent_g;	/* usually a negative value */
  int8_t  ascent_para;
  int8_t  descent_para;
    
  /* offset 17 */
  uint16_t start_pos_upper_A;
  uint16_t start_pos_lower_a; 
  
  /* offset 21 */
#ifdef U8G2_WITH_UNICODE  
  uint16_t start_pos_unicode;
#endif
};
typedef struct _u8g2_font_info_t u8g2_font_info_t;

/* from ucglib... */
struct _u8g2_font_decode_t
{
  const uint8_t *decode_ptr;			/* pointer to the compressed data */
  
  u8g2_uint_t target_x;
  u8g2_uint_t target_y;
  
  int8_t x;						/* local coordinates, (0,0) is upper left */
  int8_t y;
  int8_t glyph_width;	
  int8_t glyph_height;

  uint8_t decode_bit_pos;			/* bitpos inside a byte of the compressed data */
  uint8_t is_transparent;
  uint8_t fg_color;
  uint8_t bg_color;
#ifdef U8G2_WITH_FONT_ROTATION  
  uint8_t dir;				/* direction */
#endif
};
typedef struct _u8g2_font_decode_t u8g2_font_decode_t;

struct _u8g2_kerning_t
{
  uint16_t first_table_cnt;
  uint16_t second_table_cnt;
  const uint16_t *first_encoding_table;  
  const uint16_t *index_to_second_table;
  const uint16_t *second_encoding_table;
  const uint8_t *kerning_values;
};
typedef struct _u8g2_kerning_t u8g2_kerning_t;


struct u8g2_cb_struct
{
  u8g2_update_dimension_cb update;
  u8g2_draw_l90_cb draw_l90;
};

typedef u8g2_uint_t (*u8g2_font_calc_vref_fnptr)(u8g2_t *u8g2);


struct u8g2_struct
{
  u8x8_t u8x8;
  u8g2_draw_ll_hvline_cb ll_hvline;	/* low level hvline procedure */
  const u8g2_cb_t *cb;		/* callback drawprocedures, can be replaced for rotation */
  
  /* the following variables must be assigned during u8g2 setup */
  uint8_t *tile_buf_ptr;	/* ptr to memory area with u8g2.display_info->tile_width * 8 * tile_buf_height bytes */
  uint8_t tile_buf_height;	/* height of the tile memory area in tile rows */
  uint8_t tile_curr_row;	/* current row for picture loop */
  
  /* dimension of the buffer in pixel */
  u8g2_uint_t pixel_buf_width;		/* equal to tile_buf_height*8 */
  u8g2_uint_t pixel_buf_height;		/* u8g2.display_info->tile_width*8 */
  u8g2_uint_t pixel_curr_row;		/* u8g2.tile_curr_row*8 */
  
  /* the following variables are set by the update dimension callback */
  /* this is clipbox after rotation for the hvline procedures */
  //u8g2_uint_t buf_x0;	/* left corner of the buffer */
  //u8g2_uint_t buf_x1;	/* right corner of the buffer (excluded) */
  u8g2_uint_t buf_y0;
  u8g2_uint_t buf_y1;
  
  /* display dimensions in pixel for the user, calculated in u8g2_update_dimension_common(), used in u8g2_draw_hv_line_2dir() */
  u8g2_uint_t width;
  u8g2_uint_t height;
  
  /* ths is the clip box for the user to check if a specific box has an intersection */
  /* use u8g2_IsIntersection from u8g2_intersection.c to test against this intersection */
  /* boundary values are part of the box so that they can be used with u8g2_IsIntersection */
  u8g2_uint_t user_x0;	/* left corner of the buffer */
  u8g2_uint_t user_x1;	/* right corner of the buffer (excluded) */
  u8g2_uint_t user_y0;	/* upper edge of the buffer */
  u8g2_uint_t user_y1;	/* lower edge of the buffer (excluded) */
  
  /* information about the current font */
  const uint8_t *font;             /* current font for all text procedures */
  // removed: const u8g2_kerning_t *kerning;		/* can be NULL */
  // removed: u8g2_get_kerning_cb get_kerning_cb;
  
  u8g2_font_calc_vref_fnptr font_calc_vref;
  u8g2_font_decode_t font_decode;		/* new font decode structure */
  u8g2_font_info_t font_info;			/* new font info structure */

  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;
  
  int8_t glyph_x_offset;		/* set by u8g2_GetGlyphWidth as a side effect */
  
  uint8_t bitmap_transparency;	/* black pixels will be treated as transparent (not drawn) */

  uint8_t draw_color;		/* 0: clear pixel, 1: set pixel, modified and restored by font procedures */
					/* draw_color can be used also directly by the user API */
					
  uint8_t is_auto_page_clear; 		/* set to 0 to disable automatic page clear in firstPage() and nextPage() */
  
#ifdef U8G2_WITH_HVLINE_COUNT
  unsigned long hv_cnt;
#endif /* U8G2_WITH_HVLINE_COUNT */   
};

#define u8g2_GetU8x8(u8g2) ((u8x8_t *)(u8g2))

#ifdef U8X8_WITH_USER_PTR
#define u8g2_GetUserPtr(u8g2) ((u8g2_GetU8x8(u8g2))->user_ptr)
#define u8g2_SetUserPtr(u8g2, p) ((u8g2_GetU8x8(u8g2))->user_ptr = (p))
#endif


#define u8g2_SetAutoPageClear(u8g2, mode) ((u8g2)->is_auto_page_clear = (mode))

/*==========================================*/
/* u8x8 wrapper */

#define u8g2_SetupDisplay(u8g2, display_cb, cad_cb, byte_cb, gpio_and_delay_cb) \
  u8x8_Setup(u8g2_GetU8x8(u8g2), (display_cb), (cad_cb), (byte_cb), (gpio_and_delay_cb))

#define u8g2_InitDisplay(u8g2) u8x8_InitDisplay(u8g2_GetU8x8(u8g2))
#define u8g2_SetPowerSave(u8g2, is_enable) u8x8_SetPowerSave(u8g2_GetU8x8(u8g2), (is_enable))
#define u8g2_SetFlipMode(u8g2, mode) u8x8_SetFlipMode(u8g2_GetU8x8(u8g2), (mode))
#define u8g2_SetContrast(u8g2, value) u8x8_SetContrast(u8g2_GetU8x8(u8g2), (value))
//#define u8g2_ClearDisplay(u8g2) u8x8_ClearDisplay(u8g2_GetU8x8(u8g2))  obsolete, can not be used in all cases
void u8g2_ClearDisplay(u8g2_t *u8g2);

#define u8g2_GetDisplayHeight(u8g2) ((u8g2)->height)
#define u8g2_GetDisplayWidth(u8g2) ((u8g2)->width)
#define u8g2_GetDrawColor(u8g2) ((u8g2)->draw_color)

#define u8g2_SetI2CAddress(u8g2, address) ((u8g2_GetU8x8(u8g2))->i2c_address = (address))
#define u8g2_GetI2CAddress(u8g2)   u8x8_GetI2CAddress(u8g2_GetU8x8(u8g2))

#ifdef U8X8_USE_PINS 
#define u8g2_SetMenuSelectPin(u8g2, val) u8x8_SetMenuSelectPin(u8g2_GetU8x8(u8g2), (val)) 
#define u8g2_SetMenuNextPin(u8g2, val) u8x8_SetMenuNextPin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuPrevPin(u8g2, val) u8x8_SetMenuPrevPin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuHomePin(u8g2, val) u8x8_SetMenuHomePin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuUpPin(u8g2, val) u8x8_SetMenuUpPin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuDownPin(u8g2, val) u8x8_SetMenuDownPin(u8g2_GetU8x8(u8g2), (val))
#endif

/*==========================================*/
/* u8g2_setup.c */

extern const u8g2_cb_t u8g2_cb_r0;
extern const u8g2_cb_t u8g2_cb_r1;
extern const u8g2_cb_t u8g2_cb_r2;
extern const u8g2_cb_t u8g2_cb_r3;
extern const u8g2_cb_t u8g2_cb_mirror;

#define U8G2_R0	(&u8g2_cb_r0)
#define U8G2_R1	(&u8g2_cb_r1)
#define U8G2_R2	(&u8g2_cb_r2)
#define U8G2_R3	(&u8g2_cb_r3)
#define U8G2_MIRROR	(&u8g2_cb_mirror)
/*
  u8g2:			A new, not yet initialized u8g2 memory areay
  buf:			Memory are of size tile_buf_height*<width of the display in pixel>
  tile_buf_height:	Number of full lines
  ll_hvline_cb:		one of:
    u8g2_ll_hvline_vertical_top_lsb
    u8g2_ll_hvline_horizontal_right_lsb
  u8g2_cb			U8G2_R0 .. U8G2_R3
      
*/

void u8g2_SetupBuffer(u8g2_t *u8g2, uint8_t *buf, uint8_t tile_buf_height, u8g2_draw_ll_hvline_cb ll_hvline_cb, const u8g2_cb_t *u8g2_cb);
void u8g2_SetDisplayRotation(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8g2_d_memory.c generated code start */

uint8_t *u8g2_m_ssd1306_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_sh1106_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_sh1106_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_sh1106_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_8_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_8_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_8_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_12_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_12_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_12_f(uint8_t *page_cnt);


/* u8g2_d_memory.c generated code end */

/*==========================================*/
/* u8g2_d_setup.c generated code start */
void u8g2_Setup_ssd1306_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x32_univision_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x32_univision_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x32_univision_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x32_univision_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x32_univision_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x32_univision_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x48_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x48_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x48_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x48_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x48_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x48_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);

/* u8g2_d_setup.c generated code end */

/*==========================================*/
/* u8g2_buffer.c */

void u8g2_SendBuffer(u8g2_t *u8g2);
void u8g2_ClearBuffer(u8g2_t *u8g2);

void u8g2_SetPageCurrTileRow(u8g2_t *u8g2, uint8_t row) U8G2_NOINLINE;
void u8g2_FirstPage(u8g2_t *u8g2);
uint8_t u8g2_NextPage(u8g2_t *u8g2);

#define u8g2_GetBufferPtr(u8g2) ((u8g2)->tile_buf_ptr)
#define u8g2_GetBufferTileHeight(u8g2)	((u8g2)->tile_buf_height)
#define u8g2_GetBufferTileWidth(u8g2)	(u8g2_GetU8x8(u8g2)->display_info->tile_width)
/* the following variable is only valid after calling u8g2_FirstPage */
#define u8g2_GetPageCurrTileRow(u8g2) ((u8g2)->tile_curr_row)

/*==========================================*/
/* u8g2_ll_hvline.c */
/*
  x,y		Upper left position of the line within the local buffer (not the display!)
  len		length of the line in pixel, len must not be 0
  dir		0: horizontal line (left to right)
		1: vertical line (top to bottom)
  asumption: 
    all clipping done
*/

/* SSD13xx, UC17xx, UC16xx */
void u8g2_ll_hvline_vertical_top_lsb(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);
/* ST7920 */
void u8g2_ll_hvline_horizontal_right_lsb(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);


/*==========================================*/
/* u8g2_hvline.c */

/* u8g2_DrawHVLine does not use u8g2_IsIntersection */
void u8g2_DrawHVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);

/* the following three function will do an intersection test of this is enabled with U8G2_WITH_INTERSECTION */
void u8g2_DrawHLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len);
void u8g2_DrawVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len);
void u8g2_DrawPixel(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y);
void u8g2_SetDrawColor(u8g2_t *u8g2, uint8_t color) U8G2_NOINLINE;  /* u8g: u8g_SetColorIndex(u8g_t *u8g, uint8_t idx); */


/*==========================================*/
/* u8g2_bitmap.c */
void u8g2_SetBitmapMode(u8g2_t *u8g2, uint8_t is_transparent);
void u8g2_DrawHorizontalBitmap(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, const uint8_t *b);
void u8g2_DrawBitmap(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap);
void u8g2_DrawXBM(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap);
void u8g2_DrawXBMP(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap);	/* assumes bitmap in PROGMEM */


/*==========================================*/
/* u8g2_intersection.c */
#ifdef U8G2_WITH_INTERSECTION    
uint8_t u8g2_IsIntersection(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t x1, u8g2_uint_t y1);
#endif /* U8G2_WITH_INTERSECTION */



/*==========================================*/
/* u8g2_circle.c */
#define U8G2_DRAW_UPPER_RIGHT 0x01
#define U8G2_DRAW_UPPER_LEFT  0x02
#define U8G2_DRAW_LOWER_LEFT 0x04
#define U8G2_DRAW_LOWER_RIGHT  0x08
#define U8G2_DRAW_ALL (U8G2_DRAW_UPPER_RIGHT|U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_LOWER_RIGHT|U8G2_DRAW_LOWER_LEFT)
void u8g2_DrawCircle(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t option);
void u8g2_DrawDisc(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t option);
void u8g2_DrawEllipse(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t option);
void u8g2_DrawFilledEllipse(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t option);

/*==========================================*/
/* u8g2_line.c */
void u8g2_DrawLine(u8g2_t *u8g2, u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2);


/*==========================================*/
/* u8g2_box.c */
void u8g2_DrawBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h);
void u8g2_DrawFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h);
void u8g2_DrawRBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r);
void u8g2_DrawRFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r);


/*==========================================*/
/* u8g2_polygon.c */
void u8g2_ClearPolygonXY(void);
void u8g2_AddPolygonXY(u8g2_t *u8g2, int16_t x, int16_t y);
void u8g2_DrawPolygon(u8g2_t *u8g2);
void u8g2_DrawTriangle(u8g2_t *u8g2, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);



/*==========================================*/
/* u8g2_kerning.c */
//uint8_t u8g2_GetNullKerning(u8g2_t *u8g2, uint16_t e1, uint16_t e2);
uint8_t u8g2_GetKerning(u8g2_t *u8g2, u8g2_kerning_t *kerning, uint16_t e1, uint16_t e2);
uint8_t u8g2_GetKerningByTable(u8g2_t *u8g2, const uint16_t *kt, uint16_t e1, uint16_t e2);


/*==========================================*/
/* u8g2_font.c */

size_t u8g2_GetFontSize(const uint8_t *font_arg);

#define U8G2_FONT_HEIGHT_MODE_TEXT 0
#define U8G2_FONT_HEIGHT_MODE_XTEXT 1
#define U8G2_FONT_HEIGHT_MODE_ALL 2

void u8g2_SetFont(u8g2_t *u8g2, const uint8_t  *font);
void u8g2_SetFontMode(u8g2_t *u8g2, uint8_t is_transparent);

uint8_t u8g2_IsGlyph(u8g2_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_t *u8g2, uint16_t requested_encoding);
u8g2_uint_t u8g2_DrawGlyph(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding);
int8_t u8g2_GetStrX(u8g2_t *u8g2, const char *s);	/* for u8g compatibility */

void u8g2_SetFontDirection(u8g2_t *u8g2, uint8_t dir);
u8g2_uint_t u8g2_DrawStr(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str);
u8g2_uint_t u8g2_DrawUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str);
u8g2_uint_t u8g2_DrawExtendedUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint8_t to_left, u8g2_kerning_t *kerning, const char *str);
u8g2_uint_t u8g2_DrawExtUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint8_t to_left, const uint16_t *kerning_table, const char *str);

#define u8g2_GetMaxCharHeight(u8g2) ((u8g2)->font_info.max_char_height)
#define u8g2_GetMaxCharWidth(u8g2) ((u8g2)->font_info.max_char_width)
#define u8g2_GetAscent(u8g2) ((u8g2)->font_ref_ascent)
#define u8g2_GetDescent(u8g2) ((u8g2)->font_ref_descent)
#define u8g2_GetFontAscent(u8g2) ((u8g2)->font_ref_ascent)
#define u8g2_GetFontDescent(u8g2) ((u8g2)->font_ref_descent)

u8g2_uint_t u8g2_GetStrWidth(u8g2_t *u8g2, const char *s);
u8g2_uint_t u8g2_GetUTF8Width(u8g2_t *u8g2, const char *str);

void u8g2_SetFontPosBaseline(u8g2_t *u8g2);
void u8g2_SetFontPosBottom(u8g2_t *u8g2);
void u8g2_SetFontPosTop(u8g2_t *u8g2);
void u8g2_SetFontPosCenter(u8g2_t *u8g2);

void u8g2_SetFontRefHeightText(u8g2_t *u8g2);
void u8g2_SetFontRefHeightExtendedText(u8g2_t *u8g2);
void u8g2_SetFontRefHeightAll(u8g2_t *u8g2);



/*==========================================*/
/* u8g2_selection_list.c */
void u8g2_DrawUTF8Line(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, const char *s, uint8_t border_size, uint8_t is_invert);
u8g2_uint_t u8g2_DrawUTF8Lines(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t line_height, const char *s);
uint8_t u8g2_UserInterfaceSelectionList(u8g2_t *u8g2, const char *title, uint8_t start_pos, const char *sl);

/*==========================================*/
/* u8g2_message.c */
uint8_t u8g2_UserInterfaceMessage(u8g2_t *u8g2, const char *title1, const char *title2, const char *title3, const char *buttons);

/*==========================================*/
/* u8g2_input_value.c */
uint8_t u8g2_UserInterfaceInputValue(u8g2_t *u8g2, const char *title, const char *pre, uint8_t *value, uint8_t lo, uint8_t hi, uint8_t digits, const char *post);


/*==========================================*/
/* u8x8_d_sdl_128x64.c */
void u8g2_SetupBuffer_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_SetupBuffer_SDL_128x64_4(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8x8_d_tga.c */
void u8g2_SetupBuffer_TGA_DESC(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_SetupBuffer_TGA_LCD(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8x8_d_utf8.c */
/* 96x32 stdout */
void u8g2_SetupBuffer_Utf8(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);




/*==========================================*/
/* itoa procedures */
#define u8g2_u8toa u8x8_u8toa
#define u8g2_u16toa u8x8_u16toa


/*==========================================*/

/* start font list */
extern const uint8_t u8g2_font_freedoomr25_tn[] U8G2_FONT_SECTION("u8g2_font_freedoomr25_tn");
extern const uint8_t u8g2_font_freedoomr25_mn[] U8G2_FONT_SECTION("u8g2_font_freedoomr25_mn");
extern const uint8_t u8g2_font_unifont_t_arabic[] U8G2_FONT_SECTION("u8g2_font_unifont_t_arabic");
extern const uint8_t u8g2_font_unifont_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese1");
extern const uint8_t u8g2_font_unifont_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese2");
extern const uint8_t u8g2_font_unifont_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese3");
extern const uint8_t u8g2_font_unifont_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_japanese1");
extern const uint8_t u8g2_font_unifont_t_korean1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_korean1");
extern const uint8_t u8g2_font_unifont_t_korean2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_korean2");
extern const uint8_t u8g2_font_unifont_t_symbols[] U8G2_FONT_SECTION("u8g2_font_unifont_t_symbols");
extern const uint8_t u8g2_font_logisoso62_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso62_tn");
extern const uint8_t u8g2_font_4x6_tf[] U8G2_FONT_SECTION("u8g2_font_4x6_tf");
extern const uint8_t u8g2_font_4x6_tr[] U8G2_FONT_SECTION("u8g2_font_4x6_tr");
extern const uint8_t u8g2_font_5x7_tf[] U8G2_FONT_SECTION("u8g2_font_5x7_tf");
extern const uint8_t u8g2_font_5x7_tr[] U8G2_FONT_SECTION("u8g2_font_5x7_tr");
extern const uint8_t u8g2_font_6x10_tf[] U8G2_FONT_SECTION("u8g2_font_6x10_tf");
extern const uint8_t u8g2_font_6x10_tr[] U8G2_FONT_SECTION("u8g2_font_6x10_tr");
extern const uint8_t u8g2_font_6x10_mf[] U8G2_FONT_SECTION("u8g2_font_6x10_mf");
extern const uint8_t u8g2_font_6x10_mr[] U8G2_FONT_SECTION("u8g2_font_6x10_mr");
extern const uint8_t u8g2_font_6x10_mn[] U8G2_FONT_SECTION("u8g2_font_6x10_mn");
extern const uint8_t u8g2_font_6x12_tf[] U8G2_FONT_SECTION("u8g2_font_6x12_tf");
extern const uint8_t u8g2_font_6x12_tr[] U8G2_FONT_SECTION("u8g2_font_6x12_tr");
extern const uint8_t u8g2_font_7x13_tf[] U8G2_FONT_SECTION("u8g2_font_7x13_tf");
extern const uint8_t u8g2_font_7x13_tr[] U8G2_FONT_SECTION("u8g2_font_7x13_tr");
extern const uint8_t u8g2_font_7x13_tn[] U8G2_FONT_SECTION("u8g2_font_7x13_tn");
extern const uint8_t u8g2_font_7x13_te[] U8G2_FONT_SECTION("u8g2_font_7x13_te");
extern const uint8_t u8g2_font_7x13_mf[] U8G2_FONT_SECTION("u8g2_font_7x13_mf");
extern const uint8_t u8g2_font_7x13_mr[] U8G2_FONT_SECTION("u8g2_font_7x13_mr");
extern const uint8_t u8g2_font_7x13_mn[] U8G2_FONT_SECTION("u8g2_font_7x13_mn");
extern const uint8_t u8g2_font_7x13_me[] U8G2_FONT_SECTION("u8g2_font_7x13_me");
extern const uint8_t u8g2_font_7x13_t_symbols[] U8G2_FONT_SECTION("u8g2_font_7x13_t_symbols");
extern const uint8_t u8g2_font_7x13B_tf[] U8G2_FONT_SECTION("u8g2_font_7x13B_tf");
extern const uint8_t u8g2_font_7x13B_tr[] U8G2_FONT_SECTION("u8g2_font_7x13B_tr");
extern const uint8_t u8g2_font_7x13B_mf[] U8G2_FONT_SECTION("u8g2_font_7x13B_mf");
extern const uint8_t u8g2_font_7x13B_mr[] U8G2_FONT_SECTION("u8g2_font_7x13B_mr");
extern const uint8_t u8g2_font_7x13B_mn[] U8G2_FONT_SECTION("u8g2_font_7x13B_mn");
extern const uint8_t u8g2_font_7x14_tf[] U8G2_FONT_SECTION("u8g2_font_7x14_tf");
extern const uint8_t u8g2_font_7x14_tr[] U8G2_FONT_SECTION("u8g2_font_7x14_tr");
extern const uint8_t u8g2_font_7x14_tn[] U8G2_FONT_SECTION("u8g2_font_7x14_tn");
extern const uint8_t u8g2_font_7x14_mf[] U8G2_FONT_SECTION("u8g2_font_7x14_mf");
extern const uint8_t u8g2_font_7x14_mr[] U8G2_FONT_SECTION("u8g2_font_7x14_mr");
extern const uint8_t u8g2_font_7x14_mn[] U8G2_FONT_SECTION("u8g2_font_7x14_mn");
extern const uint8_t u8g2_font_7x14B_tf[] U8G2_FONT_SECTION("u8g2_font_7x14B_tf");
extern const uint8_t u8g2_font_7x14B_tr[] U8G2_FONT_SECTION("u8g2_font_7x14B_tr");
extern const uint8_t u8g2_font_7x14B_tn[] U8G2_FONT_SECTION("u8g2_font_7x14B_tn");
extern const uint8_t u8g2_font_7x14B_mf[] U8G2_FONT_SECTION("u8g2_font_7x14B_mf");
extern const uint8_t u8g2_font_7x14B_mr[] U8G2_FONT_SECTION("u8g2_font_7x14B_mr");
extern const uint8_t u8g2_font_7x14B_mn[] U8G2_FONT_SECTION("u8g2_font_7x14B_mn");
extern const uint8_t u8g2_font_9x15_tr[] U8G2_FONT_SECTION("u8g2_font_9x15_tr");
extern const uint8_t u8g2_font_inb19_mn[] U8G2_FONT_SECTION("u8g2_font_inb19_mn");
extern const uint8_t u8g2_font_inb30_mr[] U8G2_FONT_SECTION("u8g2_font_inb30_mr");
extern const uint8_t u8g2_font_inb30_mn[] U8G2_FONT_SECTION("u8g2_font_inb30_mn");
extern const uint8_t u8g2_font_ncenB08_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tr");
extern const uint8_t u8g2_font_ncenB10_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tr");
extern const uint8_t u8g2_font_ncenB14_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tr");
extern const uint8_t u8g2_font_ncenB24_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tr");
extern const uint8_t u8g2_font_ncenB24_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tn");
extern const uint8_t u8g2_font_inb16_mr[] U8G2_FONT_SECTION("u8g2_font_inb16_mr");
extern const uint8_t u8g2_font_inb16_mf[] U8G2_FONT_SECTION("u8g2_font_inb16_mf");
extern const uint8_t u8g2_font_inb24_mf[] U8G2_FONT_SECTION("u8g2_font_inb24_mf");
extern const uint8_t u8g2_font_timB10_tr[] U8G2_FONT_SECTION("u8g2_font_timB10_tr");
extern const uint8_t u8g2_font_timB14_tr[] U8G2_FONT_SECTION("u8g2_font_timB14_tr");
extern const uint8_t u8g2_font_timB18_tn[] U8G2_FONT_SECTION("u8g2_font_timB18_tn");

/* end font list */

/*==========================================*/
/* u8g font mapping, might be incomplete.... */



#define u8g_font_4x6   u8g2_font_4x6_tf
#define u8g_font_4x6r   u8g2_font_4x6_tr
#define u8g_font_5x7   u8g2_font_5x7_tf
#define u8g_font_5x7r   u8g2_font_5x7_tr
#define u8g_font_6x10   u8g2_font_6x10_tf
#define u8g_font_6x10r   u8g2_font_6x10_tr
#define u8g_font_6x12   u8g2_font_6x12_tf
#define u8g_font_6x12r   u8g2_font_6x12_tr
#define u8g_font_7x13B   u8g2_font_7x13B_tf
#define u8g_font_7x13Br   u8g2_font_7x13B_tr
#define u8g_font_7x13   u8g2_font_7x13_tf
#define u8g_font_7x13r   u8g2_font_7x13_tr
#define u8g_font_7x14B   u8g2_font_7x14B_tf
#define u8g_font_7x14Br   u8g2_font_7x14B_tr
#define u8g_font_7x14   u8g2_font_7x14_tf
#define u8g_font_7x14r   u8g2_font_7x14_tr
#define u8g_font_9x15r   u8g2_font_9x15_tr
#define u8g_font_ncenB08r   u8g2_font_ncenB08_tr
#define u8g_font_ncenB10r   u8g2_font_ncenB10_tr
#define u8g_font_ncenB14r   u8g2_font_ncenB14_tr
#define u8g_font_ncenB24r   u8g2_font_ncenB24_tr
#define u8g_font_ncenB24n   u8g2_font_ncenB24_tn
#define u8g_font_timB10r   u8g2_font_timB10_tr
#define u8g_font_timB14r   u8g2_font_timB14_tr



/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
}
#endif


#endif

