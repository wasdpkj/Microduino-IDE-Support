//BMP转hex工具：https://www.microduino.cn/wiki/index.php/File:DataToHex.zip

#include "bmpFile.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFT.h> // Hardware-specific library
#include <SPI.h>

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


#define BUFFPIXEL 20

#define read16(Y,Z)  (uint16_t)((uint8_t)pgm_read_byte((Y) + (Z++)) | ((uint8_t)pgm_read_byte((Y) + (Z++)) << 8))
#define read32(Y,Z) (uint32_t)((uint8_t)pgm_read_byte((Y) + (Z++)) | ((uint8_t)pgm_read_byte((Y) + (Z++)) << 8) | ((uint8_t)pgm_read_byte((Y) + (Z++)) << 16) | ((uint8_t)pgm_read_byte((Y) + (Z++)) << 24))

void drawBMP(int16_t x, int16_t y, const uint8_t *bitmap) {
  uint32_t _dataNum = 0;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint16_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;
  Serial.print(tft.width());
  Serial.print(F("x"));
  Serial.println(tft.height());

  // Parse BMP header
  if (read16(bitmap, _dataNum) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bitmap, _dataNum));
    (void)read32(bitmap, _dataNum); // Read & ignore creator bytes
    bmpImageoffset = read32(bitmap, _dataNum); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bitmap, _dataNum));
    bmpWidth  = read32(bitmap, _dataNum);
    bmpHeight = read32(bitmap, _dataNum);
    if (read16(bitmap, _dataNum) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bitmap, _dataNum); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bitmap, _dataNum) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print("x");
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);
        for (row = 0; row < h; row++) { // For each scanline...
          pos = bmpImageoffset + (flip ? (bmpHeight - 1 - row) : row) * rowSize;;
          buffidx = sizeof(sdbuffer); //Current position in _dataBuffer

          uint32_t pkj = 0;
          for (col = 0; col < w; col++) { // For each pixel...
            if (buffidx >= sizeof(sdbuffer)) { //Indeed
              buffidx = 0; //Set index to beginning
              uint32_t cwj = pkj * sizeof(sdbuffer) + pos;
              for (int a = 0; a < sizeof(sdbuffer); a++) {
                sdbuffer[a] = pgm_read_byte(bitmap + a + cwj);
              }
              pkj++;
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            //tft.pushColor(tft.color565(r, g, b));
            //tft.writePixel(x + col, y + row, tft.color565(r, g, b));
            tft.writePixel(tft.color565(r, g, b));
          } // end pixel
        } // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(F(" ms"));
      } // end goodBmp
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println(F("BOM decoder test!"));

#ifdef ESP32
  tft.begin(SPI_DEFAULT_FREQ, SPI_PORT);
#else
  tft.begin(SPI_DEFAULT_FREQ);
#endif
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
}

void loop(void) {
  for (uint8_t r = 0; r < 4; r++) {
    tft.setRotation(r);
    tft.fillScreen(TFT_BLACK);
    drawBMP(0, 0, bmp_File);
    delay(3000);
  }
}