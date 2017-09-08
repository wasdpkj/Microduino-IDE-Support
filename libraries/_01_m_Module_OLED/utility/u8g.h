/*

  u8g.h
  
  Universal 8bit Graphics Library
  
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

#ifndef _U8G_H
#define _U8G_H

/* uncomment the following line to support displays larger than 240x240 */
//#define U8G_16BIT 1


#include <stddef.h>
#include <stdint.h>

#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif 

/* 
  use the com interface directly on any systems which are not AVR or ARDUINO 
*/
#if defined(__AVR__) || defined(ARDUINO)
#define U8G_WITH_PINLIST
#endif


#ifdef __cplusplus
extern "C" {
#endif

  
/*===============================================================*/
#ifdef __GNUC__
#  define U8G_NOINLINE __attribute__((noinline))
#  define U8G_PURE  __attribute__ ((pure))
#  define U8G_NOCOMMON __attribute__ ((nocommon))
#  define U8G_SECTION(name) __attribute__ ((section (name)))
#  if defined(__MSPGCC__)
/* mspgcc does not have .progmem sections. Use -fdata-sections. */
#    define U8G_FONT_SECTION(name)
#  endif
#  if defined(__AVR__)
#    define U8G_FONT_SECTION(name) U8G_SECTION(".progmem." name)
#  endif
#else
#  define U8G_NOINLINE
#  define U8G_PURE
#  define U8G_NOCOMMON
#  define U8G_SECTION(name)
#  define U8G_FONT_SECTION(name)
#endif

#ifndef U8G_FONT_SECTION
#  define U8G_FONT_SECTION(name)
#endif


/*===============================================================*/
/* flash memory access */

#if defined(__AVR__)
/* U8G_PROGMEM is used by the XBM example */
#define U8G_PROGMEM U8G_SECTION(".progmem.data")
typedef uint8_t PROGMEM u8g_pgm_uint8_t;
typedef uint8_t u8g_fntpgm_uint8_t;
#define u8g_pgm_read(adr) pgm_read_byte_near(adr)
#define U8G_PSTR(s) ((u8g_pgm_uint8_t *)PSTR(s))

#else

#define U8G_PROGMEM
#define PROGMEM
typedef uint8_t u8g_pgm_uint8_t;
typedef uint8_t u8g_fntpgm_uint8_t;
#define u8g_pgm_read(adr) (*(const u8g_pgm_uint8_t *)(adr)) 
#define U8G_PSTR(s) ((u8g_pgm_uint8_t *)(s))

#endif
  
  
/*===============================================================*/
/* forward */
typedef struct _u8g_t u8g_t;
typedef struct _u8g_dev_t u8g_dev_t;

typedef struct _u8g_dev_arg_pixel_t u8g_dev_arg_pixel_t;
typedef struct _u8g_dev_arg_bbx_t u8g_dev_arg_bbx_t;
typedef struct _u8g_box_t u8g_box_t;
typedef struct _u8g_dev_arg_irgb_t u8g_dev_arg_irgb_t;


/*===============================================================*/
/* generic */
#if defined(U8G_16BIT)
typedef uint16_t u8g_uint_t;
typedef int16_t u8g_int_t;
#else
typedef uint8_t u8g_uint_t;
typedef int8_t u8g_int_t;
#endif

#ifdef OBSOLETE
struct _u8g_box_t
{
  u8g_uint_t x0, y0, x1, y1;  
};
typedef struct _u8g_box_t u8g_box_t;
#endif /* OBSOLETE */


/*===============================================================*/
/* device structure */

#ifdef __XC8
/* device prototype */
typedef uint8_t (*u8g_dev_fnptr)(void *u8g, void *dev, uint8_t msg, void *arg);

/* com prototype */
typedef uint8_t (*u8g_com_fnptr)(void *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#else
/* device prototype */
typedef uint8_t (*u8g_dev_fnptr)(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* com prototype */
typedef uint8_t (*u8g_com_fnptr)(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#endif



struct _u8g_dev_t
{
  u8g_dev_fnptr dev_fn;         /* device procedure */
  void *dev_mem;                /* device memory */
  u8g_com_fnptr com_fn;         /* communication procedure */
};


/*===============================================================*/
/* device list */


/* LY120 OLED with SSD1327 Controller (tested with Seeedstudio module) */
extern u8g_dev_t u8g_dev_ssd1327_96x96_gr_i2c;

extern u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_i2c;

/* OLED 128x64 Display with SSD1306 Controller */
extern u8g_dev_t u8g_dev_ssd1306_128x64_i2c;

extern u8g_dev_t u8g_dev_ssd1306_128x64_2x_i2c;

/* u8g_virtual_screen.c  */
extern u8g_dev_t u8g_dev_vs;


/*===============================================================*/
/* device messages */

struct _u8g_dev_arg_pixel_t
{
  u8g_uint_t x, y;              /* will be modified */
  uint8_t pixel;                  /* will be modified, pixel sequence or transparency value */
  uint8_t dir;
  uint8_t color;			/* color or index value, red value for true color mode */
  uint8_t hi_color;		/* high byte for 64K color mode, low byte is in "color", green value for true color mode */
  uint8_t blue;			/* blue value in true color mode */
};
/* typedef struct _u8g_dev_arg_pixel_t u8g_dev_arg_pixel_t; */ /* forward decl */


struct _u8g_dev_arg_bbx_t
{
  u8g_uint_t x, y, w, h;
};
/* typedef struct _u8g_dev_arg_bbx_t u8g_dev_arg_bbx_t; */ /* forward decl */

struct _u8g_box_t
{
  u8g_uint_t x0, y0, x1, y1;
};
/* typedef struct _u8g_box_t u8g_box_t; */ /* forward decl */

struct _u8g_dev_arg_irgb_t
{
  u8g_uint_t idx, r, g, b;		/* index with rgb value */
};
/* typedef struct _u8g_dev_arg_irgb_t u8g_dev_arg_irgb_t; */ /* forward decl */



#define U8G_DEV_MSG_INIT                10
#define U8G_DEV_MSG_STOP                  11

/* arg: pointer to uint8_t, contranst value between 0 and 255 */
#define U8G_DEV_MSG_CONTRAST            15

#define U8G_DEV_MSG_SLEEP_ON            16
#define U8G_DEV_MSG_SLEEP_OFF            17

#define U8G_DEV_MSG_PAGE_FIRST                  20
#define U8G_DEV_MSG_PAGE_NEXT                   21

/* arg: u8g_dev_arg_bbx_t * */
/* new algorithm with U8G_DEV_MSG_GET_PAGE_BOX makes this msg obsolete */
/* #define U8G_DEV_MSG_IS_BBX_INTERSECTION 22 */

/* arg: u8g_box_t *, fill structure with current page properties */
#define U8G_DEV_MSG_GET_PAGE_BOX 23

/*
#define U8G_DEV_MSG_PRIMITIVE_START             30
#define U8G_DEV_MSG_PRIMITIVE_END               31
*/

/* arg: u8g_dev_arg_pixel_t * */
#define U8G_DEV_MSG_SET_TPIXEL				44
#define U8G_DEV_MSG_SET_4TPIXEL				45

#define U8G_DEV_MSG_SET_PIXEL               50
#define U8G_DEV_MSG_SET_8PIXEL              59

#define U8G_DEV_MSG_SET_COLOR_ENTRY         60

#define U8G_DEV_MSG_SET_XY_CB               61

#define U8G_DEV_MSG_GET_WIDTH               70
#define U8G_DEV_MSG_GET_HEIGHT              71
#define U8G_DEV_MSG_GET_MODE                72

/*===============================================================*/
/* device modes */
#define U8G_MODE(is_index_mode, is_color, bits_per_pixel) (((is_index_mode)<<6) | ((is_color)<<5)|(bits_per_pixel))

#define U8G_MODE_UNKNOWN     0
#define U8G_MODE_BW     U8G_MODE(0, 0, 1)
#define U8G_MODE_INDEX  U8G_MODE(1, 1, 8)


#define U8G_MODE_GET_BITS_PER_PIXEL(mode) ((mode)&31)
#define U8G_MODE_IS_COLOR(mode) (((mode)&32)==0?0:1)
#define U8G_MODE_IS_INDEX_MODE(mode) (((mode)&64)==0?0:1)


/*===============================================================*/
/* com options */

/* uncomment the following line for Atmega HW SPI double speed, issue 89 */
/* #define U8G_HW_SPI_2X 1 */

/* com messages */

#define U8G_COM_MSG_STOP        0
#define U8G_COM_MSG_INIT        1

#define U8G_COM_MSG_ADDRESS 2

/* CHIP_SELECT argument: number of the chip which needs to be activated, so this is more like high active */
#define U8G_COM_MSG_CHIP_SELECT 3

#define U8G_COM_MSG_RESET 4

#define U8G_COM_MSG_WRITE_BYTE 5
#define U8G_COM_MSG_WRITE_SEQ 6
#define U8G_COM_MSG_WRITE_SEQ_P 7


/* com driver */
uint8_t u8g_com_null_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);               /* u8g_com_null.c */
uint8_t u8g_com_arduino_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);		/* u8g_com_arduino_ssd_i2c.c */


/* 
  Translation of system specific com drives to generic com names
  At the moment, the following generic com drives are available
  U8G_COM_SSD_I2C
  
*/


#if defined(ARDUINO)
#if defined(__AVR__)
#define U8G_COM_SSD_I2C u8g_com_arduino_ssd_i2c_fn
#endif
#endif

#ifndef U8G_COM_SSD_I2C
#if defined(__AVR__)
/* AVR variant can use the arduino version at the moment */
#define U8G_COM_SSD_I2C u8g_com_arduino_ssd_i2c_fn
#endif
#endif
#ifndef U8G_COM_SSD_I2C
#define U8G_COM_SSD_I2C u8g_com_null_fn
#endif



/*===============================================================*/
/* com api */

#define U8G_SPI_CLK_CYCLE_50NS 1
#define U8G_SPI_CLK_CYCLE_300NS 2
#define U8G_SPI_CLK_CYCLE_400NS 3
#define U8G_SPI_CLK_CYCLE_NONE 255

uint8_t u8g_InitCom(u8g_t *u8g, u8g_dev_t *dev, uint8_t clk_cycle_time);
void u8g_StopCom(u8g_t *u8g, u8g_dev_t *dev);
void u8g_EnableCom(u8g_t *u8g, u8g_dev_t *dev);         /* obsolete */
void u8g_DisableCom(u8g_t *u8g, u8g_dev_t *dev);        /* obsolete */
void u8g_SetChipSelect(u8g_t *u8g, u8g_dev_t *dev, uint8_t cs);
void u8g_SetResetLow(u8g_t *u8g, u8g_dev_t *dev);
void u8g_SetResetHigh(u8g_t *u8g, u8g_dev_t *dev);
void u8g_SetAddress(u8g_t *u8g, u8g_dev_t *dev, uint8_t address);
uint8_t u8g_WriteByte(u8g_t *u8g, u8g_dev_t *dev, uint8_t val);
uint8_t u8g_WriteSequence(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, uint8_t *seq);
uint8_t u8g_WriteSequenceP(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, const uint8_t *seq);



#define U8G_ESC_DLY(x) 255, ((x) & 0x7f)
#define U8G_ESC_CS(x) 255, (0xd0 | ((x)&0x0f))
#define U8G_ESC_ADR(x) 255, (0xe0 | ((x)&0x0f))
#define U8G_ESC_RST(x) 255, (0xc0 | ((x)&0x0f))
#define U8G_ESC_VCC(x) 255, (0xbe | ((x)&0x01))
#define U8G_ESC_END 255, 254
#define U8G_ESC_255 255, 255
//uint8_t u8g_WriteEscSeqP(u8g_t *u8g, u8g_dev_t *dev, u8g_pgm_uint8_t *esc_seq);
uint8_t u8g_WriteEscSeqP(u8g_t *u8g, u8g_dev_t *dev, const uint8_t *esc_seq);


/* u8g_com_api_16gr.c */
uint8_t u8g_WriteByteBWTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t b);
uint8_t u8g_WriteSequenceBWTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, uint8_t *ptr);
uint8_t u8g_WriteByte4LTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t b);
uint8_t u8g_WriteSequence4LTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, uint8_t *ptr);


/*===============================================================*/
/* u8g_arduino_common.c */
void u8g_com_arduino_digital_write(u8g_t *u8g, uint8_t pin_index, uint8_t value);
void u8g_com_arduino_assign_pin_output_high(u8g_t *u8g);

/*===============================================================*/
/* u8g_com_io.c */

/* create internal number from port and pin */
uint8_t u8g_Pin(uint8_t port, uint8_t bitpos);
#define PN(port,bitpos) u8g_Pin(port,bitpos)

/* low level procedures */
void u8g_SetPinOutput(uint8_t internal_pin_number);
void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level);
void u8g_SetPinInput(uint8_t internal_pin_number);
uint8_t u8g_GetPinLevel(uint8_t internal_pin_number);

/* u8g level procedures, expect U8G_PI_xxx macro */
void u8g_SetPIOutput(u8g_t *u8g, uint8_t pi);
void u8g_SetPILevel(u8g_t *u8g, uint8_t pi, uint8_t level);


/*===============================================================*/
/* page */
struct _u8g_page_t
{
  u8g_uint_t page_height;
  u8g_uint_t total_height;
  u8g_uint_t page_y0;
  u8g_uint_t page_y1;
  uint8_t page;
};
typedef struct _u8g_page_t u8g_page_t;

void u8g_page_First(u8g_page_t *p) U8G_NOINLINE;                                                                                        /* u8g_page.c */
void u8g_page_Init(u8g_page_t *p, u8g_uint_t page_height, u8g_uint_t total_height ) U8G_NOINLINE;            /* u8g_page.c */
uint8_t u8g_page_Next(u8g_page_t *p) U8G_NOINLINE;                                                                                   /* u8g_page.c */

/*===============================================================*/
/* page buffer (pb) */

struct _u8g_pb_t
{
  u8g_page_t p;
  u8g_uint_t width;		/* pixel width */
  void *buf;
};
typedef struct _u8g_pb_t u8g_pb_t;


/* u8g_pb.c */
void u8g_pb_Clear(u8g_pb_t *b);
uint8_t u8g_pb_IsYIntersection(u8g_pb_t *pb, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb_IsXIntersection(u8g_pb_t *b, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb_IsIntersection(u8g_pb_t *pb, u8g_dev_arg_bbx_t *bbx);
void u8g_pb_GetPageBox(u8g_pb_t *pb, u8g_box_t *box);
uint8_t u8g_pb_Is8PixelVisible(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel);
uint8_t u8g_pb_WriteBuffer(u8g_pb_t *b, u8g_t *u8g, u8g_dev_t *dev);

/*
  note on __attribute__ ((nocommon))
    AVR scripts often use  --gc-sections on the linker to remove unused section.
    This works fine for initialed data and text sections. In principle .bss is also
    handled, but the name##_pb definition is not removed. Reason is, that
    array definitions are placed in the COMMON section, by default
    The attribute "nocommon" removes this automatic assignment to the
    COMMON section and directly puts it into .bss. As a result, if more
    than one buffer is defined in one file, then it will be removed with --gc-sections

    .. not sure if Arduino IDE uses -fno-common... if yes, then the attribute is
    redundant.
*/
#define U8G_PB_DEV(name, width, height, page_height, dev_fn, com_fn) \
uint8_t name##_buf[width] U8G_NOCOMMON ; \
u8g_pb_t name##_pb = { {page_height, height, 0, 0, 0},  width, name##_buf}; \
u8g_dev_t name = { dev_fn, &name##_pb, com_fn }


void u8g_pb8v1_Init(u8g_pb_t *b, void *buf, u8g_uint_t width)   U8G_NOINLINE;
void u8g_pb8v1_Clear(u8g_pb_t *b) U8G_NOINLINE;

uint8_t u8g_pb8v1_IsYIntersection(u8g_pb_t *b, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb8v1_IsXIntersection(u8g_pb_t *b, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb8v1_WriteBuffer(u8g_pb_t *b, u8g_t *u8g, u8g_dev_t *dev);

uint8_t u8g_dev_pb8v1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb16v1.c */
uint8_t u8g_dev_pb16v1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);


/*===============================================================*/
/* u8g_ll_api.c */

/* cursor draw callback */
typedef void (*u8g_draw_cursor_fn)(u8g_t *u8g);

/* vertical reference point calculation callback */
typedef u8g_uint_t (*u8g_font_calc_vref_fnptr)(u8g_t *u8g);

/* state backup and restore procedure */
typedef void (*u8g_state_cb)(uint8_t msg);


/* PI = Pin Index */

/* reset pin, usually optional */
#define U8G_PI_RESET 0

/* address / data or instruction */
#define U8G_PI_A0 1
#define U8G_PI_DI 1

/* chip select line */
#define U8G_PI_CS 2
#define U8G_PI_CS1 2
#define U8G_PI_CS2 3
/* Feb 2013: A0 state moved from 7 to 3 for t6963 controller*/
#define U8G_PI_A0_STATE 3

/* enable / clock signal */
#define U8G_PI_EN 4
#define U8G_PI_CS_STATE 4
#define U8G_PI_SCK 4
#define U8G_PI_SCL 4
#define U8G_PI_RD 4


/* data pins, shared with SPI and I2C pins */
#define U8G_PI_D0 5
#define U8G_PI_MOSI 5
#define U8G_PI_SDA 5
#define U8G_PI_D1 6
#define U8G_PI_MISO 6
#define U8G_PI_D2 7
#define U8G_PI_D3 8
#define U8G_PI_SET_A0 8
#define U8G_PI_D4 9
#define U8G_PI_D5 10
#define U8G_PI_I2C_OPTION 11
#define U8G_PI_D6 11
#define U8G_PI_D7 12

/* read/write pin, must be the last pin in the list, this means U8G_PIN_LIST_LEN =  U8G_PI_RW + 1*/
#define U8G_PI_WR 13
#define U8G_PI_RW 13 

#define U8G_PIN_LIST_LEN 14


#define U8G_PIN_DUMMY 254
#define U8G_PIN_NONE 255

#define U8G_FONT_HEIGHT_MODE_TEXT 0
#define U8G_FONT_HEIGHT_MODE_XTEXT 1
#define U8G_FONT_HEIGHT_MODE_ALL 2

struct _u8g_t
{
  u8g_uint_t width;
  u8g_uint_t height;
  
  
  u8g_dev_t *dev;               /* first device in the device chain */
  const u8g_pgm_uint8_t *font;             /* regular font for all text procedures */
  const u8g_pgm_uint8_t *cursor_font;  /* special font for cursor procedures */
  uint8_t cursor_fg_color, cursor_bg_color;
  uint8_t cursor_encoding;
  uint8_t mode;                         /* display mode, one of U8G_MODE_xxx */
  u8g_uint_t cursor_x;
  u8g_uint_t cursor_y;
  u8g_draw_cursor_fn cursor_fn;
  
  int8_t glyph_dx;
  int8_t glyph_x;
  int8_t glyph_y;
  uint8_t glyph_width;
  uint8_t glyph_height;
  
  u8g_font_calc_vref_fnptr font_calc_vref;
  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;
  uint8_t font_line_spacing_factor;     /* line_spacing = factor * (ascent - descent) / 64 */
  uint8_t line_spacing;
  
  u8g_dev_arg_pixel_t arg_pixel;
  /* uint8_t color_index; */

#ifdef U8G_WITH_PINLIST
  uint8_t pin_list[U8G_PIN_LIST_LEN];
#endif
  
  u8g_state_cb state_cb;
  
  u8g_box_t current_page;		/* current box of the visible page */

};

#define u8g_GetFontAscent(u8g) ((u8g)->font_ref_ascent)
#define u8g_GetFontDescent(u8g) ((u8g)->font_ref_descent)
#define u8g_GetFontLineSpacing(u8g) ((u8g)->line_spacing)

uint8_t u8g_call_dev_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

uint8_t u8g_InitLL(u8g_t *u8g, u8g_dev_t *dev);
void u8g_FirstPageLL(u8g_t *u8g, u8g_dev_t *dev);
uint8_t u8g_NextPageLL(u8g_t *u8g, u8g_dev_t *dev);
uint8_t u8g_SetContrastLL(u8g_t *u8g, u8g_dev_t *dev, uint8_t contrast);
void u8g_DrawPixelLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y);
void u8g_Draw8PixelLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
void u8g_Draw4TPixelLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
uint8_t u8g_IsBBXIntersectionLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h);	/* obsolete */
u8g_uint_t u8g_GetWidthLL(u8g_t *u8g, u8g_dev_t *dev);
u8g_uint_t u8g_GetHeightLL(u8g_t *u8g, u8g_dev_t *dev);

void u8g_UpdateDimension(u8g_t *u8g);
uint8_t u8g_Begin(u8g_t *u8g);				/* reset device, put it into default state and call u8g_UpdateDimension() */
uint8_t u8g_Init(u8g_t *u8g, u8g_dev_t *dev);   /* only usefull if the device only as hardcoded ports */
uint8_t u8g_InitComFn(u8g_t *u8g, u8g_dev_t *dev, u8g_com_fnptr com_fn);	/* Init procedure for anything which is not Arduino or AVR (e.g. ARM, but not Due, which is Arduino) */
uint8_t u8g_InitI2C(u8g_t *u8g, u8g_dev_t *dev, uint8_t options);	/* use U8G_I2C_OPT_NONE as options */
void u8g_FirstPage(u8g_t *u8g);
uint8_t u8g_NextPage(u8g_t *u8g);
uint8_t u8g_SetContrast(u8g_t *u8g, uint8_t contrast);
void u8g_SleepOn(u8g_t *u8g);
void u8g_SleepOff(u8g_t *u8g);
void u8g_DrawPixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y);
void u8g_Draw8Pixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
void u8g_Draw4TPixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);

uint8_t u8g_Stop(u8g_t *u8g);
void u8g_SetColorEntry(u8g_t *u8g, uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void u8g_SetColorIndex(u8g_t *u8g, uint8_t idx);
void u8g_SetHiColor(u8g_t *u8g, uint16_t rgb);
void u8g_SetHiColorByRGB(u8g_t *u8g, uint8_t r, uint8_t g, uint8_t b);
void u8g_SetRGB(u8g_t *u8g, uint8_t r, uint8_t g, uint8_t b);
uint8_t u8g_GetColorIndex(u8g_t *u8g);

uint8_t u8g_GetDefaultForegroundColor(u8g_t *u8g);
void u8g_SetDefaultForegroundColor(u8g_t *u8g);

uint8_t u8g_GetDefaultBackgroundColor(u8g_t *u8g);
void u8g_SetDefaultBackgroundColor(u8g_t *u8g);

uint8_t u8g_GetDefaultMidColor(u8g_t *u8g);
void u8g_SetDefaultMidColor(u8g_t *u8g);

#define u8g_GetWidth(u8g) ((u8g)->width)
#define u8g_GetHeight(u8g) ((u8g)->height)
#define u8g_GetMode(u8g) ((u8g)->mode)
/*
  U8G_MODE_GET_BITS_PER_PIXEL(u8g_GetMode(u8g))
  U8G_MODE_IS_COLOR(u8g_GetMode(u8g)) 
*/

/* u8g_state.c */
#define U8G_STATE_ENV_IDX 0
#define U8G_STATE_U8G_IDX 1
#define U8G_STATE_RESTORE 0
#define U8G_STATE_BACKUP 1
#define U8G_STATE_MSG_COMPOSE(cmd,idx) (((cmd)<<1) | (idx))

#define U8G_STATE_MSG_RESTORE_ENV U8G_STATE_MSG_COMPOSE(U8G_STATE_RESTORE,U8G_STATE_ENV_IDX)
#define U8G_STATE_MSG_BACKUP_ENV U8G_STATE_MSG_COMPOSE(U8G_STATE_BACKUP,U8G_STATE_ENV_IDX)
#define U8G_STATE_MSG_RESTORE_U8G U8G_STATE_MSG_COMPOSE(U8G_STATE_RESTORE,U8G_STATE_U8G_IDX)
#define U8G_STATE_MSG_BACKUP_U8G U8G_STATE_MSG_COMPOSE(U8G_STATE_BACKUP,U8G_STATE_U8G_IDX)

#define U8G_STATE_MSG_GET_IDX(msg) ((msg)&1)
#define U8G_STATE_MSG_IS_BACKUP(msg) ((msg)&2)



void u8g_state_dummy_cb(uint8_t msg);
/* backward compatible definition */
#define u8g_backup_avr_spi u8g_backup_spi

void u8g_SetHardwareBackup(u8g_t *u8g, u8g_state_cb backup_cb);

/* u8g_clip.c */

uint8_t u8g_IsBBXIntersection(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h);


/* u8g_rot.c */

void u8g_UndoRotation(u8g_t *u8g);
void u8g_SetRot90(u8g_t *u8g);
void u8g_SetRot180(u8g_t *u8g);
void u8g_SetRot270(u8g_t *u8g);

/* u8g_scale.c */

void u8g_UndoScale(u8g_t *u8g);
void u8g_SetScale2x2(u8g_t *u8g);


/* u8g_font.c */

size_t u8g_font_GetSize(const void *font);
uint8_t u8g_font_GetFontStartEncoding(const void *font) U8G_NOINLINE;
uint8_t u8g_font_GetFontEndEncoding(const void *font) U8G_NOINLINE;

void u8g_SetFont(u8g_t *u8g, const u8g_fntpgm_uint8_t *font);

uint8_t u8g_GetFontBBXWidth(u8g_t *u8g);
uint8_t u8g_GetFontBBXHeight(u8g_t *u8g);
int8_t u8g_GetFontBBXOffX(u8g_t *u8g);
int8_t u8g_GetFontBBXOffY(u8g_t *u8g);
uint8_t u8g_GetFontCapitalAHeight(u8g_t *u8g);

uint8_t u8g_IsGlyph(u8g_t *u8g, uint8_t requested_encoding);
int8_t u8g_GetGlyphDeltaX(u8g_t *u8g, uint8_t requested_encoding);

int8_t u8g_draw_glyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding); /* used by u8g_cursor.c */

int8_t u8g_DrawGlyphDir(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding);
int8_t u8g_DrawGlyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyph90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyph180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyph270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyphFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding);

u8g_uint_t u8g_DrawStr(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);
u8g_uint_t u8g_DrawStr90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);
u8g_uint_t u8g_DrawStr180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);
u8g_uint_t u8g_DrawStr270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);

u8g_uint_t u8g_DrawStrDir(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, const char *s);


u8g_uint_t u8g_DrawStrP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_DrawStr90P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_DrawStr180P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_DrawStr270P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);


void u8g_SetFontRefHeightText(u8g_t *u8g);
void u8g_SetFontRefHeightExtendedText(u8g_t *u8g);
void u8g_SetFontRefHeightAll(u8g_t *u8g);
void u8g_SetFontLineSpacingFactor(u8g_t *u8g, uint8_t factor);

u8g_uint_t u8g_font_calc_vref_font(u8g_t *u8g);
u8g_uint_t u8g_font_calc_vref_bottom(u8g_t *u8g);
u8g_uint_t u8g_font_calc_vref_top(u8g_t *u8g);
u8g_uint_t u8g_font_calc_vref_center(u8g_t *u8g);

void u8g_SetFontPosBaseline(u8g_t *u8g);
void u8g_SetFontPosBottom(u8g_t *u8g);
void u8g_SetFontPosCenter(u8g_t *u8g);
void u8g_SetFontPosTop(u8g_t *u8g);


u8g_uint_t u8g_GetStrPixelWidth(u8g_t *u8g, const char *s);
u8g_uint_t u8g_GetStrPixelWidthP(u8g_t *u8g, const u8g_pgm_uint8_t *s);
int8_t u8g_GetStrX(u8g_t *u8g, const char *s);
int8_t u8g_GetStrXP(u8g_t *u8g, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_GetStrWidth(u8g_t *u8g, const char *s) U8G_NOINLINE;
u8g_uint_t u8g_GetStrWidthP(u8g_t *u8g, const u8g_pgm_uint8_t *s);

u8g_uint_t u8g_DrawStrFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, const char *s);

void u8g_GetStrMinBox(u8g_t *u8g, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height);
void u8g_GetStrAMinBox(u8g_t *u8g, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height);


u8g_uint_t u8g_DrawAAStr(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);

/* u8g_rect.c */

void u8g_draw_box(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) U8G_NOINLINE; 

void u8g_DrawHLine(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w) U8G_NOINLINE;
void u8g_DrawVLine(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w) U8G_NOINLINE;
void u8g_DrawFrame(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) U8G_NOINLINE;
void u8g_DrawBox(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) U8G_NOINLINE;

void u8g_DrawRFrame(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r) U8G_NOINLINE;
void u8g_DrawRBox(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r) U8G_NOINLINE;

/* u8g_bitmap.c */

void u8g_DrawHBitmap(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, const uint8_t *bitmap);
void u8g_DrawHBitmapP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, const u8g_pgm_uint8_t *bitmap);
void u8g_DrawBitmap(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap);
void u8g_DrawBitmapP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap);

void u8g_DrawXBM(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const uint8_t *bitmap);
void u8g_DrawXBMP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap);


/* u8g_line.c */
void u8g_DrawLine(u8g_t *u8g, u8g_uint_t x1, u8g_uint_t y1, u8g_uint_t x2, u8g_uint_t y2);


/* u8g_circle.c */

/* the following, commented code has been rewritten or is not yet finished
#define U8G_CIRC_UPPER_RIGHT 0x01
#define U8G_CIRC_UPPER_LEFT  0x02
#define U8G_CIRC_LOWER_LEFT 0x04
#define U8G_CIRC_LOWER_RIGHT  0x08
#define U8G_CIRC_ALL (U8G_CIRC_UPPER_RIGHT|U8G_CIRC_UPPER_LEFT|U8G_CIRC_LOWER_RIGHT|U8G_CIRC_LOWER_LEFT)
void u8g_DrawEmpCirc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);
void u8g_DrawFillCirc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);
void u8g_DrawEllipseRect(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t x1, u8g_uint_t y1);
*/

#define U8G_DRAW_UPPER_RIGHT 0x01
#define U8G_DRAW_UPPER_LEFT  0x02
#define U8G_DRAW_LOWER_LEFT 0x04
#define U8G_DRAW_LOWER_RIGHT  0x08
#define U8G_DRAW_ALL (U8G_DRAW_UPPER_RIGHT|U8G_DRAW_UPPER_LEFT|U8G_DRAW_LOWER_RIGHT|U8G_DRAW_LOWER_LEFT)

void u8g_draw_circle(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option) U8G_NOINLINE;
void u8g_draw_disc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option) U8G_NOINLINE;

void u8g_DrawCircle(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);
void u8g_DrawDisc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);

/* u8g_ellipse.c */
void u8g_DrawEllipse(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rx, u8g_uint_t ry, uint8_t option);
void u8g_DrawFilledEllipse(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rx, u8g_uint_t ry, uint8_t option);

/* u8g_clip.c */
uint8_t u8g_is_box_bbx_intersection(u8g_box_t *box, u8g_dev_arg_bbx_t *bbx);


/* u8g_cursor.c */
void u8g_SetCursorFont(u8g_t *u8g, const u8g_pgm_uint8_t *cursor_font);
void u8g_SetCursorStyle(u8g_t *u8g, uint8_t encoding);
void u8g_SetCursorPos(u8g_t *u8g, u8g_uint_t cursor_x, u8g_uint_t cursor_y);
void u8g_SetCursorColor(u8g_t *u8g, uint8_t fg, uint8_t bg);
void u8g_EnableCursor(u8g_t *u8g);
void u8g_DisableCursor(u8g_t *u8g);
void u8g_DrawCursor(u8g_t *u8g);

/* u8g_polygon.c */

typedef int16_t pg_word_t;

#define PG_NOINLINE U8G_NOINLINE

struct pg_point_struct
{
  pg_word_t x;
  pg_word_t y;
};

typedef struct _pg_struct pg_struct;	/* forward declaration */

struct pg_edge_struct
{
  pg_word_t x_direction;	/* 1, if x2 is greater than x1, -1 otherwise */
  pg_word_t height;
  pg_word_t current_x_offset;
  pg_word_t error_offset;
  
  /* --- line loop --- */
  pg_word_t current_y;
  pg_word_t max_y;
  pg_word_t current_x;
  pg_word_t error;

  /* --- outer loop --- */
  uint8_t (*next_idx_fn)(pg_struct *pg, uint8_t i);
  uint8_t curr_idx;
};

/* maximum number of points in the polygon */
/* can be redefined, but highest possible value is 254 */
#define PG_MAX_POINTS 6

/* index numbers for the pge structures below */
#define PG_LEFT 0
#define PG_RIGHT 1


struct _pg_struct
{
  struct pg_point_struct list[PG_MAX_POINTS];
  uint8_t cnt;
  uint8_t is_min_y_not_flat;
  pg_word_t total_scan_line_cnt;
  struct pg_edge_struct pge[2];	/* left and right line draw structures */
};

void pg_ClearPolygonXY(pg_struct *pg);
void pg_AddPolygonXY(pg_struct *pg, u8g_t *u8g, int16_t x, int16_t y);
void pg_DrawPolygon(pg_struct *pg, u8g_t *u8g);
void u8g_ClearPolygonXY(void);
void u8g_AddPolygonXY(u8g_t *u8g, int16_t x, int16_t y);
void u8g_DrawPolygon(u8g_t *u8g);
void u8g_DrawTriangle(u8g_t *u8g, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);


/*===============================================================*/
/* u8g_virtual_screen.c */
void u8g_SetVirtualScreenDimension(u8g_t *vs_u8g, u8g_uint_t width, u8g_uint_t height);
uint8_t u8g_AddToVirtualScreen(u8g_t *vs_u8g, u8g_uint_t x, u8g_uint_t y, u8g_t *child_u8g);

/*===============================================================*/
void st_Draw(uint8_t fps);
void st_Step(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire);

/*===============================================================*/
/* u8g_com_i2c.c */

/* options for u8g_i2c_init() */
#define U8G_I2C_OPT_NONE 0
#define U8G_I2C_OPT_NO_ACK 2

/* retrun values from u8g_twi_get_error() */
#define U8G_I2C_ERR_NONE 0x00
/* the following values are bit masks */
#define U8G_I2C_ERR_TIMEOUT 0x01
#define U8G_I2C_ERR_BUS 0x02

void u8g_i2c_clear_error(void) U8G_NOINLINE;
uint8_t  u8g_i2c_get_error(void) U8G_NOINLINE;
uint8_t u8g_i2c_get_err_pos(void) U8G_NOINLINE;
void u8g_i2c_init(uint8_t options) U8G_NOINLINE;		/* use U8G_I2C_OPT_NONE as options */
uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos) U8G_NOINLINE;
uint8_t u8g_i2c_start(uint8_t sla) U8G_NOINLINE;
uint8_t u8g_i2c_send_byte(uint8_t data) U8G_NOINLINE;
void u8g_i2c_stop(void) U8G_NOINLINE;


/*===============================================================*/
/* u8g_u8toa.c */
/* v = value, d = number of digits */
const char *u8g_u8toa(uint8_t v, uint8_t d);

/* u8g_u8toa.c */
/* v = value, d = number of digits */
const char *u8g_u16toa(uint16_t v, uint8_t d);

/*===============================================================*/
/* u8g_delay.c */

/* delay by the specified number of milliseconds */
void u8g_Delay(uint16_t val);

/* delay by one microsecond */
void u8g_MicroDelay(void);

/* delay by 10 microseconds */
void u8g_10MicroDelay(void);

/*===============================================================*/
/* chessengine.c */
#define CHESS_KEY_NONE 0
#define CHESS_KEY_NEXT 1
#define CHESS_KEY_PREV 2
#define CHESS_KEY_SELECT 3
#define CHESS_KEY_BACK 4

void chess_Init(u8g_t *u8g, uint8_t empty_body_color);
void chess_Draw(void);
void chess_Step(uint8_t keycode);

/*===============================================================*/
/* font definitions */
extern const u8g_fntpgm_uint8_t u8g_font_timB18[] U8G_FONT_SECTION("u8g_font_timB18");
extern const u8g_fntpgm_uint8_t u8g_font_timB14[] U8G_FONT_SECTION("u8g_font_timB14");
extern const u8g_fntpgm_uint8_t u8g_font_timB10[] U8G_FONT_SECTION("u8g_font_timB10");


extern const u8g_fntpgm_uint8_t u8g_font_5x7[] U8G_FONT_SECTION("u8g_font_5x7");
extern const u8g_fntpgm_uint8_t u8g_font_6x10[] U8G_FONT_SECTION("u8g_font_6x10");
extern const u8g_fntpgm_uint8_t u8g_font_7x13[] U8G_FONT_SECTION("u8g_font_7x13");
extern const u8g_fntpgm_uint8_t u8g_font_9x15[] U8G_FONT_SECTION("u8g_font_9x15");


extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0r[] U8G_FONT_SECTION("u8g_font_fixed_v0r");


extern const u8g_fntpgm_uint8_t u8g_font_chikitar[] U8G_FONT_SECTION("u8g_font_chikitar");


extern const u8g_fntpgm_uint8_t u8g_font_unifont[] U8G_FONT_SECTION("u8g_font_unifont");


/* contributed */
extern const u8g_fntpgm_uint8_t u8g_font_freedoomr25n[] U8G_FONT_SECTION("u8g_font_freedoomr25n");


#ifdef __cplusplus
}
#endif

#endif /* _U8G_H */

