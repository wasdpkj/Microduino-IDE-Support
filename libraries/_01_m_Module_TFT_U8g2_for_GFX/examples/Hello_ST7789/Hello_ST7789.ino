#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFT.h>
#include <U8g2_for_Adafruit_GFX.h>

//#define BOARD_TYPE JOYPAD_ESP
//#define BOARD_TYPE MICROBIT_ESP

#define NORMAL    0       //Cube-S2 + ST7735
#define JOYPAD_ESP    1   //ESP32 + ST7789
#define MICROBIT_ESP  2   //ESP32 + ST7789

#if BOARD_TYPE == MICROBIT_ESP
#define TFT_CLK   18
#define TFT_MOSI  23
#define TFT_MISO  -1
#define TFT_CS    25
#define TFT_DC    26   //0
#define TFT_RST   -1   //15
#define TFT_BL    5
#define SPI_PORT  SPI
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS,  TFT_DC, TFT_RST);
#elif BOARD_TYPE == JOYPAD_ESP
#define TFT_CLK   14
#define TFT_MOSI  13
#define TFT_MISO  -1
#define TFT_CS    4
#define TFT_DC    15  //0
#define TFT_RST   -1   //15
#define TFT_BL    2
#define SPI_PORT  TFTSPI
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);
#else
#define TFT_CLK   D13
#define TFT_MOSI  D11
#define TFT_MISO  -1
#define TFT_CS     D5
#define TFT_RST    -1  // you can also connect this to the Arduino reset
#define TFT_DC     D4
#define TFT_BL     D6
#define SPI_PORT  SPI
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#endif

U8G2_FOR_ADAFRUIT_GFX u8g2;


// To read the short story with all glyphs you need at least a font with _gb2312b postfix
// However, a font with _gb2312b postfix is very large and will not always fit on
// the target controller. For testing you can use _chinese1 extenstion, but then
// many gylphs are skipped and not visible.
//
// wqy fonts are available in different sizes, here are only wqy12, wqy14 and wqy16 listed

//#define FONT u8g2_font_wqy12_t_chinese1
//#define FONT u8g2_font_wqy12_t_gb2312b

//#define FONT u8g2_font_wqy14_t_chinese1
//#define FONT u8g2_font_wqy14_t_gb2312b

//#define FONT u8g2_font_wqy16_t_chinese1
#define FONT u8g2_font_wqy16_t_gb2312

// The next two macros define the scroll speed of the short story
#define SCROLL_DELTA 4
#define SCROLL_DELAY 100


// Chinese Short Story
// The Farmer God Shen Nong Tastes All the Plants
// Please excause wrong line breaks


void setup() {
#ifdef ESP32
  tft.begin(SPI_DEFAULT_FREQ, SPI_PORT);
#else
  tft.begin(SPI_DEFAULT_FREQ);
#endif
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  u8g2.begin(tft);                 // connect u8g2 procedures to Adafruit GFX

  u8g2.setFont(FONT);
  u8g2.setFontMode(1);
  u8g2.setForegroundColor(TFT_WHITE);      // apply Adafruit GFX color

  u8g2.setCursor(0,20);                // start writing at this position
  u8g2.print("Hello World");
  u8g2.setCursor(0,40);                // start writing at this position
  u8g2.print("你好!");
}

void loop() {

}