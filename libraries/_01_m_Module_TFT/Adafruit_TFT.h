/*!
* @file Adafruit_TFT.h
* 
* This is the documentation for ST7735/ST7789 driver for the
* Arduino platform. 
*
*
* Adafruit invests time and resources providing this open source code,
* please support Adafruit and open-source hardware by purchasing
* products from Adafruit!
*
*
* This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
* Adafruit_GFX</a> being present on your system. Please make sure you have
* installed the latest version before using this library.
*
* Written by Limor "ladyada" Fried for Adafruit Industries.
*
* BSD license, all text here must be included in any redistribution.
*
*/

#ifndef _ADAFRUIT_ST7789H_
#define _ADAFRUIT_ST7789H_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SPITFT_Macros.h"

#if defined(ARDUINO_STM32_FEATHER)
typedef volatile uint32 RwReg;
#endif
#if defined(ARDUINO_FEATHER52) || defined (ARDUINO_MAXIM)
typedef volatile uint32_t RwReg;
#endif

//**ST7735**
#define ST7735_TFTWIDTH	  128
#define ST7735_TFTHEIGHT  160

#define ST7735_128x160_XSTART 0
#define ST7735_128x160_YSTART 0

//**ST7789**
#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 80

#define ST7789_TFTWIDTH   240       ///< ST7789 max TFT width
#define ST7789_TFTHEIGHT  240       ///< ST7789 max TFT height


#define ST_CMD_DELAY 0x80 // special signifier for command lists

#define MADCTL_MY  0x80     ///< Right to left
#define MADCTL_MX  0x40     ///< Bottom to top
#define MADCTL_MV  0x20     ///< Reverse Mode
#define MADCTL_ML  0x10     ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00     ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08     ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04     ///< LCD refresh right to left


#define TFT_NOP   	   	  0x00      ///< No-op register
#define TFT_SWRESET    	  0x01      ///< Software reset register
#define TFT_RDDID      	  0x04      ///< Read display identification information
#define TFT_RDDST      	  0x09      ///< Read Display Status 
#define TFT_SLPIN      	  0x10      ///< Enter Sleep Mode
#define TFT_SLPOUT     	  0x11      ///< Sleep Out
#define TFT_PTLON      	  0x12      ///< Partial Mode ON
#define TFT_NORON      	  0x13      ///< Normal Display Mode ON
#define TFT_INVOFF     	  0x20      ///< Display Inversion OFF
#define TFT_INVON      	  0x21      ///< Display Inversion ON 
#define TFT_GAMMASET   	  0x26      ///< Gamma Set 
#define TFT_DISPOFF    	  0x28      ///< Display OFF 
#define TFT_DISPON     	  0x29      ///< Display ON
#define TFT_CASET      	  0x2A      ///< Column Address Set 
#define TFT_RASET      	  0x2B      ///< Page Address Set
#define TFT_RAMWR      	  0x2C      ///< Memory Write
#define TFT_RAMRD      	  0x2E      ///< Memory Read
#define TFT_PTLAR      	  0x30      ///< Partial Area
#define TFT_MADCTL     	  0x36      ///< Memory Access Control
#define TFT_VSCRSADD  	  0x37      ///< Vertical Scrolling Start Address
#define TFT_COLMOD     	  0x3A      ///< COLMOD: Pixel Format Set
#define TFT_RDID1      	  0xDA      ///< Read ID 1
#define TFT_RDID2      	  0xDB      ///< Read ID 2
#define TFT_RDID3      	  0xDC      ///< Read ID 3
#define TFT_GMCTRP1    	  0xE0      ///< Positive Gamma Correction
#define TFT_GMCTRN1    	  0xE1      ///< Negative Gamma Correction

//**ST7735**
#define ST7735_FRMCTR1 	  0xB1
#define ST7735_FRMCTR2 	  0xB2
#define ST7735_FRMCTR3 	  0xB3
#define ST7735_INVCTR  	  0xB4
#define ST7735_DISSET5 	  0xB6
#define ST7735_PWCTR1  	  0xC0
#define ST7735_PWCTR2  	  0xC1
#define ST7735_PWCTR3  	  0xC2
#define ST7735_PWCTR4  	  0xC3
#define ST7735_PWCTR5  	  0xC4
#define ST7735_VMCTR1  	  0xC5
#define ST7735_RDID4   	  0xDD
#define ST7735_PWCTR6  	  0xFC

//**ST7789**
#define ST7789_RGBCTL     0xB1      ///< RGB Interface Control
#define ST7789_PORCTL     0xB2      ///< Porch Setting
#define ST7789_FRMCTR1    0xB3      ///< Frame Rate Control (In Partial Mode/Idle Mode)
#define ST7789_FRMCTR2    0xC6      ///< Frame Rate Control (In Normal Mode)
#define ST7789_GCCTL      0xB7      ///< Gate Control
#define ST7789_PWCTR1     0xD0      ///< Power Control 1
#define ST7789_PWCTR2     0xE8      ///< Power Control 2
#define ST7789_LCMCTRL    0xC0      ///< LCM Control
#define ST7789_VDVVRHEN   0xC2      ///< VDV and VRH Command Enable
#define ST7789_VRHSET     0xC3      ///< VRH set
#define ST7789_VDVSET     0xC4      ///< VDV set
#define ST7789_VCMOFSET   0xC5      ///< VCOM Offset
#define ST7789_VCMOSET    0xBB      ///< VCOM Setting
#define ST7789_RDMODE     0x0A      ///< Read Display Power Mode 
#define ST7789_RDMADCTL   0x0B      ///< Read Display MADCTL
#define ST7789_RDPIXFMT   0x0C      ///< Read Display Pixel Format
#define ST7789_RDIMGFMT   0x0D      ///< Read Display Image Format 
#define ST7789_RDSELFDIAG 0x0F      ///< Read Display Self-Diagnostic Result


// Color definitions
#define TFT_BLACK       0x0000      ///<   0,   0,   0
#define TFT_NAVY        0x000F      ///<   0,   0, 128
#define TFT_DARKGREEN   0x03E0      ///<   0, 128,   0
#define TFT_DARKCYAN    0x03EF      ///<   0, 128, 128
#define TFT_MAROON      0x7800      ///< 128,   0,   0
#define TFT_PURPLE      0x780F      ///< 128,   0, 128
#define TFT_OLIVE       0x7BE0      ///< 128, 128,   0
#define TFT_LIGHTGREY   0xC618      ///< 192, 192, 192
#define TFT_DARKGREY    0x7BEF      ///< 128, 128, 128
#define TFT_BLUE        0x001F      ///<   0,   0, 255
#define TFT_GREEN       0x07E0      ///<   0, 255,   0
#define TFT_CYAN        0x07FF      ///<   0, 255, 255
#define TFT_RED         0xF800      ///< 255,   0,   0
#define TFT_MAGENTA     0xF81F      ///< 255,   0, 255
#define TFT_YELLOW      0xFFE0      ///< 255, 255,   0
#define TFT_WHITE       0xFFFF      ///< 255, 255, 255
#define TFT_ORANGE      0xFD20      ///< 255, 165,   0
#define TFT_GREENYELLOW 0xAFE5      ///< 173, 255,  47
#define TFT_PINK        0xFC18      ///< 255, 128, 192

// Color definitions
#define ST7789_BLACK       TFT_BLACK            ///<   0,   0,   0
#define ST7789_NAVY        TFT_NAVY             ///<   0,   0, 128
#define ST7789_DARKGREEN   TFT_DARKGREEN        ///<   0, 128,   0
#define ST7789_DARKCYAN    TFT_DARKCYAN         ///<   0, 128, 128
#define ST7789_MAROON      TFT_MAROON           ///< 128,   0,   0
#define ST7789_PURPLE      TFT_PURPLE           ///< 128,   0, 128
#define ST7789_OLIVE       TFT_OLIVE            ///< 128, 128,   0
#define ST7789_LIGHTGREY   TFT_LIGHTGREY        ///< 192, 192, 192
#define ST7789_DARKGREY    TFT_DARKGREY         ///< 128, 128, 128
#define ST7789_BLUE        TFT_BLUE             ///<   0,   0, 255
#define ST7789_GREEN       TFT_GREEN            ///<   0, 255,   0
#define ST7789_CYAN        TFT_CYAN             ///<   0, 255, 255
#define ST7789_RED         TFT_RED              ///< 255,   0,   0
#define ST7789_MAGENTA     TFT_MAGENTA          ///< 255,   0, 255
#define ST7789_YELLOW      TFT_YELLOW           ///< 255, 255,   0
#define ST7789_WHITE       TFT_WHITE            ///< 255, 255, 255
#define ST7789_ORANGE      TFT_ORANGE           ///< 255, 165,   0
#define ST7789_GREENYELLOW TFT_GREENYELLOW      ///< 173, 255,  47
#define ST7789_PINK        TFT_PINK             ///< 255, 128, 192


// Color definitions
#define ST7735_BLACK       TFT_BLACK            ///<   0,   0,   0
#define ST7735_NAVY        TFT_NAVY             ///<   0,   0, 128
#define ST7735_DARKGREEN   TFT_DARKGREEN        ///<   0, 128,   0
#define ST7735_DARKCYAN    TFT_DARKCYAN         ///<   0, 128, 128
#define ST7735_MAROON      TFT_MAROON           ///< 128,   0,   0
#define ST7735_PURPLE      TFT_PURPLE           ///< 128,   0, 128
#define ST7735_OLIVE       TFT_OLIVE            ///< 128, 128,   0
#define ST7735_LIGHTGREY   TFT_LIGHTGREY        ///< 192, 192, 192
#define ST7735_DARKGREY    TFT_DARKGREY         ///< 128, 128, 128
#define ST7735_BLUE        TFT_BLUE             ///<   0,   0, 255
#define ST7735_GREEN       TFT_GREEN            ///<   0, 255,   0
#define ST7735_CYAN        TFT_CYAN             ///<   0, 255, 255
#define ST7735_RED         TFT_RED              ///< 255,   0,   0
#define ST7735_MAGENTA     TFT_MAGENTA          ///< 255,   0, 255
#define ST7735_YELLOW      TFT_YELLOW           ///< 255, 255,   0
#define ST7735_WHITE       TFT_WHITE            ///< 255, 255, 255
#define ST7735_ORANGE      TFT_ORANGE           ///< 255, 165,   0
#define ST7735_GREENYELLOW TFT_GREENYELLOW      ///< 173, 255,  47
#define ST7735_PINK        TFT_PINK             ///< 255, 128, 192


#if defined (ARDUINO_STM32_FEATHER) || defined (ARDUINO_MAXIM)    // doesnt work on wiced feather
  #undef USE_FAST_PINIO
#elif defined (__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined (ESP32) || defined(__arm__)
  #define USE_FAST_PINIO
#endif


/// Class to manage hardware interface with ST7789 chipset (also seems to work with ILI9340)
class Adafruit_TFT : public Adafruit_GFX {
    protected:

    public:
        Adafruit_TFT(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1);
        Adafruit_TFT(int8_t _CS, int8_t _DC, int8_t _RST = -1);

#ifndef ESP32
        void      begin(uint32_t freq = 0);
		void	  spiInit(uint32_t freq = 0);
#else
        void      begin(uint32_t freq = 0, SPIClass &spi = SPI);
		void	  spiInit(uint32_t freq = 0, SPIClass &spi = SPI);
#endif
		void 	  displayInit(const uint8_t *addr);
		void      softReset();
        void      setRotation(uint8_t r);
        void      invertDisplay(boolean i);
        void      scrollTo(uint16_t y);

        // Required Non-Transaction
        void      drawPixel(int16_t x, int16_t y, uint16_t color);

        // Transaction API
        void      startWrite(void);
        void      endWrite(void);
        void      writePixel(int16_t x, int16_t y, uint16_t color);
        void      writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void      writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void      writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

        // Transaction API not used by GFX
        void      setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        void      writePixel(uint16_t color);
        void      writePixels(uint16_t * colors, uint32_t len);
        void      writeColor(uint16_t color, uint32_t len);
		void      pushColor(uint16_t color);

        // Recommended Non-Transaction
        void      drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void      drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
        void      fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        using     Adafruit_GFX::drawRGBBitmap; // Check base class first
        void      drawRGBBitmap(int16_t x, int16_t y,
                    uint16_t *pcolors, int16_t w, int16_t h);

        uint8_t   readcommand8(uint8_t reg, uint8_t index = 0);

        uint16_t  color565(uint8_t r, uint8_t g, uint8_t b);

//    private:
#ifdef ESP32
        SPIClass _spi;
#endif
        uint32_t _freq;
#if defined (__AVR__) || defined(TEENSYDUINO)
        int8_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#ifdef USE_FAST_PINIO
        volatile uint8_t *dcport, *csport;
        uint8_t  cspinmask, dcpinmask;
#endif
#elif defined (__arm__)
        int32_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#ifdef USE_FAST_PINIO
        volatile RwReg *dcport, *csport;
        uint32_t  cspinmask, dcpinmask;
#endif
#elif defined (ESP8266) || defined (ESP32)
        int8_t   _cs, _dc, _rst, _sclk, _mosi, _miso;
#ifdef USE_FAST_PINIO
        volatile uint32_t *dcport, *csport;
        uint32_t  cspinmask, dcpinmask;
#endif
#else
        int8_t      _cs, _dc, _rst, _sclk, _mosi, _miso;
#endif

        void        writeCommand(uint8_t cmd);
        void        spiWrite(uint8_t v);
        uint8_t     spiRead(void);
};

// Subclass of TFT type display for ST7789 TFT Driver
class Adafruit_ST7789 : public Adafruit_TFT {
  public:
    Adafruit_ST7789(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1);
    Adafruit_ST7789(int8_t _CS, int8_t _DC, int8_t _RST = -1);

#ifdef ESP32
	void begin(uint32_t freq = 0, SPIClass &spi = SPI);
#else
	void begin(uint32_t freq = 0);
#endif

    void setRotation(uint8_t m);
};

// Subclass of TFT type display for ST7735 TFT Driver
class Adafruit_ST7735 : public Adafruit_TFT{
  public:
    Adafruit_ST7735(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1);
    Adafruit_ST7735(int8_t _CS, int8_t _DC, int8_t _RST = -1);

#ifdef ESP32
	void begin(uint32_t freq = 0, SPIClass &spi = SPI);
#else
	void begin(uint32_t freq = 0);
#endif

    void setRotation(uint8_t m);
};


#endif
