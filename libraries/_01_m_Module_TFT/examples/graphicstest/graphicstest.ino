/***************************************************
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFT.h> // Hardware-specific library
#include <SPI.h>

//#define JOYPAD_ESP       //ESP32 + ST7789
//#define MICROBIT_ESP     //ESP32 + ST7789

/*
 * ST7735_128x160
 * ST7789_240x240
 * ST7789_240x320
 * ST7789_170x320
 */
#if defined(K210)
SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
#define TFT_CLK   22
#define TFT_MOSI  20
#define TFT_MISO  -1
#define TFT_CS     19
#define TFT_RST    18  // you can also connect this to the Arduino reset
#define TFT_DC     21
#define TFT_BL     23
#define SPI_PORT  spi_
ST7789_240x240 tft = ST7789_240x240(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);
#elif defined(MICROBIT_ESP)
#define TFT_CLK   18
#define TFT_MOSI  23
#define TFT_MISO  -1
#define TFT_CS    25
#define TFT_DC    26   //0
#define TFT_RST   -1   //15
#define TFT_BL    5
#define SPI_PORT  SPI
ST7789_240x240 tft = ST7789_240x240(TFT_CS,  TFT_DC, TFT_RST);
#elif defined(JOYPAD_ESP)
#define TFT_CLK   14
#define TFT_MOSI  13
#define TFT_MISO  -1
#define TFT_CS    4
#define TFT_DC    15  //0
#define TFT_RST   -1   //15
#define TFT_BL    2
#define SPI_PORT  TFTSPI
ST7789_240x240 tft = ST7789_240x240(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);
#elif defined(LE501X)
#define TFT_CLK   D13
#define TFT_MOSI  D11
#define TFT_MISO  -1
#define TFT_CS     D5
#define TFT_RST    -1  // you can also connect this to the Arduino reset
#define TFT_DC     D4
#define TFT_BL     D6
#if (SDK_HCLK_MHZ == 64)
#define TFT_SPI_FREQ    32000000
#define SPI_PORT  SSI   //SPI or SSI
#else
#define TFT_SPI_FREQ    SPI_DEFAULT_FREQ
#define SPI_PORT  SPI   //SPI or SSI
#endif 
ST7789_240x240 tft = ST7789_240x240(TFT_CS, TFT_DC, TFT_RST);
#else
#define TFT_CLK   D13
#define TFT_MOSI  D11
#define TFT_MISO  -1
#define TFT_CS     D5
#define TFT_RST    -1  // you can also connect this to the Arduino reset
#define TFT_DC     D4
#define TFT_BL     D6
#define SPI_PORT  SPI
ST7789_240x240 tft = ST7789_240x240(TFT_CS, TFT_DC, TFT_RST);
#endif


float p = 3.1415926;


void testlines(uint16_t color);
void testdrawtext(char *text, uint16_t color);
void testfastlines(uint16_t color1, uint16_t color2);
void testdrawrects(uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testtriangles();
void testroundrects();
void tftPrintTest();
void mediabuttons();

void setup(void) {
  Serial.begin(115200);
  Serial.print("Hello! TFT Test");

  SPI_PORT.begin();

#if defined(K210) || defined(ESP32)
  tft.begin(SPI_DEFAULT_FREQ, &SPI_PORT);
#elif defined(LE501X)
  tft.begin(TFT_SPI_FREQ, &SPI_PORT, false);
#else
  tft.begin(SPI_DEFAULT_FREQ);
#endif

  if(TFT_BL != -1){
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
  }
  
  Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(TFT_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  tft.fillScreen(TFT_BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", TFT_WHITE);
  delay(1000);

  // tft print function!
  tftPrintTest();
  delay(4000);

  // a single pixel
  tft.drawPixel(tft.width() / 2, tft.height() / 2, TFT_GREEN);
  delay(500);

  // line draw test
  testlines(TFT_YELLOW);
  delay(500);

  // optimized lines
  testfastlines(TFT_RED, TFT_BLUE);
  delay(500);

  testdrawrects(TFT_GREEN);
  delay(500);

  testfillrects(TFT_YELLOW, TFT_MAGENTA);
  delay(500);

  tft.fillScreen(TFT_BLACK);
  testfillcircles(10, TFT_BLUE);
  testdrawcircles(10, TFT_WHITE);
  delay(500);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

  mediabuttons();
  delay(500);

  Serial.println("done");
  delay(1000);
}

void loop() {
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
}

void testlines(uint16_t color) {
  tft.fillScreen(TFT_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(0, 0, x, tft.height() - 1, color);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(0, 0, tft.width() - 1, y, color);
  }

  tft.fillScreen(TFT_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(tft.width() - 1, 0, 0, y, color);
  }

  tft.fillScreen(TFT_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(0, tft.height() - 1, x, 0, color);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
  }

  tft.fillScreen(TFT_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(TFT_BLACK);
  for (int16_t y = 0; y < tft.height(); y += 5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x = 0; x < tft.width(); x += 5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(TFT_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(TFT_BLACK);
  for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
    tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2 , x, x, color1);
    tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x = radius; x < tft.width(); x += radius * 2) {
    for (int16_t y = radius; y < tft.height(); y += radius * 2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
    for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(TFT_BLACK);
  int color = 0xF800;
  int t;
  int w = tft.width() / 2;
  int x = tft.height() - 1;
  int y = 0;
  int z = tft.width();
  for (t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x -= 4;
    y += 4;
    z -= 4;
    color += 100;
  }
}

void testroundrects() {
  tft.fillScreen(TFT_BLACK);
  int color = 100;
  int i;
  int t;
  for (t = 0 ; t <= 4; t += 1) {
    int x = 0;
    int y = 0;
    int w = tft.width() - 2;
    int h = tft.height() - 2;
    for (i = 0 ; i <= 16; i += 1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x += 2;
      y += 3;
      w -= 4;
      h -= 6;
      color += 1100;
    }
    color += 100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(TFT_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(TFT_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(TFT_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(TFT_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, TFT_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, TFT_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, TFT_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, TFT_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, TFT_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, TFT_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, TFT_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, TFT_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, TFT_GREEN);
}
