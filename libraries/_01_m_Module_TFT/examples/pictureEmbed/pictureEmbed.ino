// ILI9341 example with embedded color bitmaps in sketch.
// WILL NOT FIT ON ARDUINO UNO OR OTHER AVR BOARDS;
// uses large bitmap image stored in array!

// Options for converting images to the format used here include:
//   http://www.rinkydinkelectronics.com/t_imageconverter565.php
// or
//  GIMP (https://www.gimp.org/) as follows:
//    1. File -> Export As
//    2. In Export Image dialog, use 'C source code (*.c)' as filetype.
//    3. Press export to get the export options dialog.
//    4. Type the desired variable name into the 'prefixed name' box.
//    5. Uncheck 'GLIB types (guint8*)'
//    6. Check 'Save as RGB565 (16-bit)'
//    7. Press export to save your image.
//  Assuming 'image_name' was typed in the 'prefixed name' box of step 4,
//  you can have to include the c file, then using the image can be done with:
//    tft.drawRGBBitmap(0, 0, image_name.pixel_data, image_name.width, image_name.height);
//  See also https://forum.pjrc.com/threads/35575-Export-for-ILI9341_t3-with-GIMP

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFT.h> // Hardware-specific library
#include <SPI.h>
#include "dragon.h"

//#define BOARD_TYPE JOYPAD_ESP
//#define BOARD_TYPE MICROBIT_ESP

/*
 * ST7735_128x160
 * ST7789_240x240
 * ST7789_240x320
 * ST7789_172x320
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
#define SPI_PORT  SPI   //SPI or SSI

#if (SPI_PORT == SPI)
#define TFT_SPI_FREQ    SPI_MAX_FREQ
#else
#define TFT_SPI_FREQ    SSI_MAX_FREQ
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

void setup() {
  Serial.begin(115200);
  Serial.print("Hello! TFT Test");

#if defined(K210) || defined(ESP32)
  tft.begin(SPI_DEFAULT_FREQ, &SPI_PORT);
#elif defined(LE501X)
  SPI_PORT.begin(TFT_CLK, TFT_MISO, TFT_MOSI, -1);
  tft.begin(TFT_SPI_FREQ, &SPI_PORT, false);
#else
  SPI_PORT.begin();
  tft.begin(SPI_DEFAULT_FREQ, false);
#endif

  if(TFT_BL != -1){
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
  }
}

void loop(void) {
  for (uint8_t r = 0; r < 4; r++) {
    tft.setRotation(r);
    tft.fillScreen(TFT_BLACK);
    for (uint8_t j = 0; j < 20; j++) {
      tft.drawRGBBitmap(
        random(-DRAGON_WIDTH , tft.width()),
        random(-DRAGON_HEIGHT, tft.height()),
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32) || defined(LE501X)
        dragonBitmap,
#else
        // Some non-AVR MCU's have a "flat" memory model and don't
        // distinguish between flash and RAM addresses.  In this case,
        // the RAM-resident-optimized drawRGBBitmap in the ILI9341
        // library can be invoked by forcibly type-converting the
        // PROGMEM bitmap pointer to a non-const uint16_t *.
        (uint16_t *)dragonBitmap,
#endif
        DRAGON_WIDTH, DRAGON_HEIGHT);
      delay(1); // Allow ESP8266 to handle watchdog & WiFi stuff
    }
    delay(3000);
  }
}
