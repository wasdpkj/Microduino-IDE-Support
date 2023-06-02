//JPEGDecoder库： https://github.com/MakotoKurauchi/JPEGDecoder
//JPEG转hex工具：https://www.microduino.cn/wiki/index.php/File:DataToHex.zip

// JPEG decoder library
#include <JPEGDecoder.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFT.h> // Hardware-specific library
#include <SPI.h>

#include "jpegFile.h"

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

void drawArrayJpeg(const uint8_t arrayname[], uint32_t array_size, int xpos, int ypos);
void renderJPEG(int xpos, int ypos);
void jpegInfo();
void showTime(uint32_t msTime);

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(115200); // Used for messages and the C array generator
  Serial.println(F("JPEG decoder test!"));

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

  //  tft.setRotation(0);  // 0 & 2 Portrait. 1 & 3 landscape
  tft.fillScreen(TFT_BLACK);

  Serial.println(F("\r\nInitialisation done."));
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop() {
  // Note the / before the SPIFFS file name must be present, this means the file is in
  // the root directory of the SPIFFS, e.g. "/Tiger.rjpg" for a file called "Tiger.jpg"
  uint32_t clearTime = millis();

  //  tft.setRotation(0);  // portrait
  tft.fillScreen(TFT_BLACK);

  // Draw a jpeg image stored in memory
  drawArrayJpeg(jpeg_File1, sizeof(jpeg_File1), 0, 0);

  Serial.print(F("file Sizeof:"));
  Serial.print(sizeof(jpeg_File1));
  Serial.print(F("byte,"));
  Serial.print(sizeof(jpeg_File1) / 1024.0);
  Serial.print(F("Kbyte"));
  Serial.println();

  // calculate how long it took to draw the image
  clearTime = millis() - clearTime; // Calculate the time it took

  showTime(clearTime);
  // print the results to the serial port
  Serial.print(F(  "Total clear time was    : ")); Serial.print(clearTime); Serial.println(F(" ms"));
  Serial.println(F(""));


  delay(5000);
}

//####################################################################################################
// Draw a JPEG on the TFT pulled from a program memory array
//####################################################################################################
void drawArrayJpeg(const uint8_t arrayname[], uint32_t array_size, int xpos, int ypos) {

  int x = xpos;
  int y = ypos;

  JpegDec.decodeArray(arrayname, array_size);

  jpegInfo(); // Print information from the JPEG file (could comment this line out)

  renderJPEG(x, y);

  Serial.println(F("#########################"));
}

//####################################################################################################
// Draw a JPEG on the TFT, images will be cropped on the right/bottom sides if they do not fit
//####################################################################################################
// This function assumes xpos,ypos is a valid screen coordinate. For convenience images that do not
// fit totally on the screen are cropped to the nearest MCU size and may leave right/bottom borders.
void renderJPEG(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = min(mcu_w, max_x % mcu_w);
  uint32_t min_h = min(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while (JpegDec.read()) {
    // save a pointer to the image block
    pImg = JpegDec.pImage ;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right and bottom edges
    if (mcu_x + mcu_w <= max_x) {
      win_w = mcu_w;
    }
    else {
      win_w = min_w;
    }
    if (mcu_y + mcu_h <= max_y) {
      win_h = mcu_h;
    }
    else {
      win_h = min_h;
    }

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height()) {
      tft.startWrite();
      tft.setAddrWindow(mcu_x, mcu_y, win_w, win_h);
#if 0        
      for (int y = 0; y < win_h; y++) {
        for (int x = 0; x < win_w; x++) {
          //tft.writePixel(mcu_x + x, mcu_y + y, *pImg++);
          tft.writePixel(*pImg++);
        }
      }
#else
#define TMPBUF_LONGWORDS (SPI_MAX_PIXELS_AT_ONCE + 1) / 2
#define TMPBUF_PIXELS (TMPBUF_LONGWORDS * 2)

      // Fill temp buffer 32 bits at a time
      uint32_t len = (win_h * win_w);
      uint16_t bufLen = (len < TMPBUF_PIXELS) ? len : TMPBUF_PIXELS;
      uint16_t xferLen;

      uint32_t _offset = 0;
      while (len)
      {                                            // While pixels remain
          xferLen = (bufLen < len) ? bufLen : len; // How many this pass?
          tft.writePixels((uint16_t *)pImg + _offset, xferLen);
          _offset += xferLen;
          len -= xferLen;
      }     
#endif
      tft.endWrite();
    }
    else if ( (mcu_y + win_h) >= tft.height()) {
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
    }
  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime;

  // print the results to the serial port
  Serial.print(F(  "Total render time was    : ")); Serial.print(drawTime); Serial.println(F(" ms"));
  Serial.println(F(""));
}

//####################################################################################################
// Print image information to the serial port (optional)
//####################################################################################################
void jpegInfo() {
  Serial.println(F("==============="));
  Serial.println(F("JPEG image info"));
  Serial.println(F("==============="));
  Serial.print(F(  "Width      :")); Serial.println(JpegDec.width);
  Serial.print(F(  "Height     :")); Serial.println(JpegDec.height);
  Serial.print(F(  "Components :")); Serial.println(JpegDec.comps);
  Serial.print(F(  "MCU / row  :")); Serial.println(JpegDec.MCUSPerRow);
  Serial.print(F(  "MCU / col  :")); Serial.println(JpegDec.MCUSPerCol);
  Serial.print(F(  "Scan type  :")); Serial.println(JpegDec.scanType);
  Serial.print(F(  "MCU width  :")); Serial.println(JpegDec.MCUWidth);
  Serial.print(F(  "MCU height :")); Serial.println(JpegDec.MCUHeight);
  Serial.println(F("==============="));
}

//####################################################################################################
// Show the execution time (optional)
//####################################################################################################
// WARNING: for UNO/AVR legacy reasons printing text to the screen with the Mega might not work for
// sketch sizes greater than ~70KBytes because 16 bit address pointers are used in some libraries.

// The Due will work fine with the HX8357_Due library.

void showTime(uint32_t msTime) {
  tft.setCursor(0, 0);
  //  tft.setTextFont(1);
  //tft.setTextSize(2);
  tft.setTextColor(ST7789_WHITE);
  tft.setTextWrap(true);
  tft.print(F(" JPEG drawn in "));
  tft.print(msTime);
  tft.println(F(" ms "));
  Serial.print(F(" JPEG drawn in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}
