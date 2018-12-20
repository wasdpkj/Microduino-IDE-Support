/***************************************************
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

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

void setup(void) {
  Serial.begin(115200);
  Serial.print("Hello! TFT Test");

#ifdef ESP32
  tft.begin(SPI_DEFAULT_FREQ, SPI_PORT);
#else
  tft.begin(SPI_DEFAULT_FREQ);
#endif
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  Serial.println("init");

  tft.setTextWrap(false); // Allow text to run off right edge
  tft.fillScreen(TFT_BLACK);

  Serial.println("This is a test of the rotation capabilities of the TFT library!");
  Serial.println("Press <SEND> (or type a character) to advance");
}

void loop(void) {
  rotateLine();
  rotateText();
  rotatePixel();
  rotateFastline();
  rotateDrawrect();
  rotateFillrect();
  rotateDrawcircle();
  rotateFillcircle();
  rotateTriangle();
  rotateFillTriangle();
  rotateRoundRect();
  rotateFillRoundRect();
  rotateChar();
  rotateString();
}

void rotateText() {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

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
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateFillcircle(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.fillCircle(10, 30, 10, TFT_YELLOW);

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateDrawcircle(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawCircle(10, 30, 10, TFT_YELLOW);

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateFillrect(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.fillRect(10, 20, 10, 20, TFT_GREEN);

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateDrawrect(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawRect(10, 20, 10, 20, TFT_GREEN);

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateFastline(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawFastHLine(0, 20, tft.width(), TFT_RED);
    tft.drawFastVLine(20, 0, tft.height(), TFT_BLUE);

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateLine(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawLine(tft.width() / 2, tft.height() / 2, 0, 0, TFT_RED);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotatePixel(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawPixel(10, 20, TFT_WHITE);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateTriangle(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawTriangle(20, 10, 10, 30, 30, 30, TFT_GREEN);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateFillTriangle(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.fillTriangle(20, 10, 10, 30, 30, 30, TFT_RED);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateRoundRect(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawRoundRect(20, 10, 25, 15, 5, TFT_BLUE);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateFillRoundRect(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.fillRoundRect(20, 10, 25, 15, 5, TFT_CYAN);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateChar(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.drawChar(25, 15, 'A', TFT_WHITE, TFT_WHITE, 1);
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}

void rotateString(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillScreen(TFT_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.setCursor(8, 25);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print("Adafruit Industries");
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation() + 1);
  }
}
