#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define FONT u8g2_font_ncenB10_tr
/*
u8g2_font_freedoomr25_tn
u8g2_font_freedoomr25_mn
u8g2_font_timB10_tr
u8g2_font_timB14_tr
u8g2_font_timB18_tn
u8g2_font_unifont_t_arabic
u8g2_font_unifont_t_chinese1
u8g2_font_unifont_t_chinese2
u8g2_font_unifont_t_chinese3
u8g2_font_unifont_t_japanese1
u8g2_font_unifont_t_korean1
u8g2_font_unifont_t_korean2
u8g2_font_unifont_t_symbols
u8g2_font_logisoso62_tn
u8g2_font_4x6_tf
u8g2_font_4x6_tr
u8g2_font_5x7_tf
u8g2_font_5x7_tr
u8g2_font_6x10_tf
u8g2_font_6x10_tr
u8g2_font_6x10_mf
u8g2_font_6x10_mr
u8g2_font_6x10_mn
u8g2_font_6x12_tf
u8g2_font_6x12_tr
u8g2_font_7x13_tf
u8g2_font_7x13_tr
u8g2_font_7x13_tn
u8g2_font_7x13_te
u8g2_font_7x13_mf
u8g2_font_7x13_mr
u8g2_font_7x13_mn
u8g2_font_7x13_me
u8g2_font_7x13_t_symbols
u8g2_font_7x13B_tf
u8g2_font_7x13B_tr
u8g2_font_7x13B_mf
u8g2_font_7x13B_mr
u8g2_font_7x13B_mn
u8g2_font_7x14_tf
u8g2_font_7x14_tr
u8g2_font_7x14_tn
u8g2_font_7x14_mf
u8g2_font_7x14_mr
u8g2_font_7x14_mn
u8g2_font_7x14B_tf
u8g2_font_7x14B_tr
u8g2_font_7x14B_tn
u8g2_font_7x14B_mf
u8g2_font_7x14B_mr
u8g2_font_7x14B_mn
u8g2_font_9x15_tr
u8g2_font_inb19_mn
u8g2_font_inb16_mr
u8g2_font_inb16_mf
u8g2_font_inb24_mf
u8g2_font_inb30_mr
u8g2_font_inb30_mn
u8g2_font_ncenB08_tr
u8g2_font_ncenB10_tr
u8g2_font_ncenB14_tr
u8g2_font_ncenB24_tr
u8g2_font_ncenB24_tn
*/

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
  u8g2.begin();  
}

void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(FONT);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(1000);
}

