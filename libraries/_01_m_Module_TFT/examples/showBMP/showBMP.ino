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
#include "bmp.h"

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

  // Parse BMP header
  if (read16(bitmap, _dataNum) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bitmap, _dataNum));
    (void)read32(bitmap, _dataNum); // Read & ignore creator bytes
    bmpImageoffset = read32(bitmap, _dataNum); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bitmap, _dataNum));
    bmpWidth  = read32(bitmap, _dataNum);
    bmpHeight = read32(bitmap, _dataNum);
    if (read16(bitmap, _dataNum) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bitmap, _dataNum); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bitmap, _dataNum) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
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
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) { // For each scanline...
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          buffidx = sizeof(sdbuffer); //Current position in _dataBuffer

          int pkj = 0;
          for (col = 0; col < w; col++) { // For each pixel...
            if (buffidx >= sizeof(sdbuffer)) { //Indeed
              for (int a = 0; a < sizeof(sdbuffer); a++) {
                sdbuffer[a] = pgm_read_byte(bitmap + (pos + a) + (pkj * sizeof(sdbuffer)));
                uint32_t pkj = (uint32_t)bitmap + (pos + a);
              }
              pkj++;
              buffidx = 0; //Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r, g, b));
            //tft.drawPixel(x + col, y + row, tft.color565(r, g, b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }
}

void setup() {
  Serial.begin(9600);
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
    drawBMP(0, 0, logoA);
    delay(3000);
  }
}
