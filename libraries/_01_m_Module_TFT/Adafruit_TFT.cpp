/*!
* @file Adafruit_TFT.cpp
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

#include "Adafruit_TFT.h"
#ifndef ARDUINO_STM32_FEATHER
  #include "pins_arduino.h"
  #ifndef RASPI
    #include "wiring_private.h"
  #endif
#endif
#include <limits.h>

#include "HardwareSerial.h"




/**************************************************************************/
/*!
    @brief  Instantiate Adafruit ST7789 driver with SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
//Adafruit_ST7789::Adafruit_ST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst, int8_t miso) : 
//	Adafruit_TFT(cs,dc,mosi,sclk,rst,miso) {
//}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit ST7789 driver with SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
//Adafruit_ST7789::Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst) : 
//	Adafruit_TFT(cs,dc,rst) {
//}




/**************************************************************************/
/*!
    @brief  Instantiate Adafruit ST7789 driver with SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_TFT::Adafruit_TFT(int8_t cs, int8_t dc, int8_t mosi,
        int8_t sclk, int8_t rst, int8_t miso, uint16_t width, uint16_t height) : Adafruit_GFX(width, height) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
#if defined(ESP32) || defined(K210) || defined(LE501X)
    _sclk = sclk;
    _mosi  = mosi;
    _miso = miso;
#else
    _sclk  = -1;
    _mosi  = -1;
    _miso  = -1;
#endif
    _freq = 0;
    
#ifdef USE_FAST_PINIO
    if (_cs >= 0) {
    csport    = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
	}
    dcport    = portOutputRegister(digitalPinToPort(_dc));
    dcpinmask = digitalPinToBitMask(_dc);
#endif
}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit ST7789 driver with SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_TFT::Adafruit_TFT(int8_t cs, int8_t dc, int8_t rst, uint16_t width, uint16_t height) : Adafruit_GFX(width, height) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _sclk  = -1;
    _mosi  = -1;
    _miso  = -1;
    _freq = 0;
#ifdef USE_FAST_PINIO
    if (_cs >= 0) {
    csport    = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
	}
    dcport    = portOutputRegister(digitalPinToPort(_dc));
    dcpinmask = digitalPinToBitMask(_dc);
#endif
}

/**************************************************************************/
/*!
    @brief  Companion code to the initiliazation tables. Reads and issues
            a series of LCD commands stored in PROGMEM byte array.
    @param  addr  Flash memory array with commands and data to send
*/
/**************************************************************************/
void Adafruit_TFT::displayInit(const uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;

  startWrite();
  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...

    writeCommand(pgm_read_byte(addr++)); // Read, issue command
    numArgs  = pgm_read_byte(addr++);    // Number of args to follow
    ms       = numArgs & ST_CMD_DELAY;   // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;            // Mask out delay bit
    while(numArgs--) {                   // For each argument...
      spiWrite(pgm_read_byte(addr++));   // Read, issue argument
    }

    if(ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
    @brief   Initialize ST7789 chip
    Connects to the ST7789 over SPI and sends initialization procedure commands
    @param    freq  Desired SPI clock frequency
*/
/**************************************************************************/
#if defined(ESP32) || defined(K210) || defined(LE501X)
void Adafruit_TFT::begin(uint32_t freq, SPIClass *spiclass, uint8_t spiNeedInit)
#else
void Adafruit_TFT::begin(uint32_t freq, uint8_t spiNeedInit)
#endif
{
#if defined(ESP32) || defined(K210) || defined(LE501X)
	spiInit(freq, spiclass, spiNeedInit);
#else
	spiInit(freq, spiNeedInit);
#endif
	
	//memcpy((uint8_t*)&_config, (uint8_t*)&config, sizeof(tft_config_t));
	
/*	
	_config.type = config.type;
	_config.xStart = config.xStart;
	_config.yStart = config.yStart;
	_config.width = config.width;
	_config.height = config.height;
	_config.invert = config.invert;
*/
	
    _width  = _config.width;
    _height = _config.height;

    Adafruit_GFX::WIDTH = _width;
    Adafruit_GFX::HEIGHT = _height;
	
	
	softReset();
	displayInit((const uint8_t *)_config.initcodeAddr);					
	invertDisplay(_config.invert);	
	setRotation(_config.rotation);
}



/**************************************************************************/
/*!
    @brief   Initialize ST7789 chip
    Connects to the ST7789 over SPI and sends initialization procedure commands
    @param    freq  Desired SPI clock frequency
*/
/**************************************************************************/
#if defined(ESP32) || defined(K210) || defined(LE501X)
void Adafruit_TFT::spiInit(uint32_t freq, SPIClass *spiclass, uint8_t spiNeedInit)
#else
void Adafruit_TFT::spiInit(uint32_t freq, uint8_t spiNeedInit)
#endif
{
#if defined(ESP32) || defined(K210) || defined(LE501X)
    if (!spiclass)
    {
      Serial.println(F("ERROR: spiclass is no define!"));
      return;
    }
    else
    {
      _spi = spiclass;
    }
#else
    _spi = &SPI;
#endif

    if(!freq){
        freq = SPI_DEFAULT_FREQ;
    }
    _freq = freq;

    // Control Pins
    if (_cs >= 0) {
	  pinMode(_cs, OUTPUT);
	  digitalWrite(_cs, HIGH);
	}
    pinMode(_dc, OUTPUT);
    digitalWrite(_dc, LOW);	//HIGH

    // Hardware SPI
    if (spiNeedInit)
    {
      SPI_BEGIN();
    }
    // toggle RST low to reset
    if (_rst >= 0) {
      pinMode(_rst, OUTPUT);
      digitalWrite(_rst, LOW);
      delay(10);
      digitalWrite(_rst, HIGH);
      delay(150);
    }
}

void Adafruit_TFT::softReset() {
	startWrite();
	writeCommand(TFT_SWRESET);
	endWrite();
    delay(150);
}


/**************************************************************************/
/*!
    @brief  Pass 8-bit (each) R,G,B, get back 16-bit packed color
            This function converts 8-8-8 RGB data to 16-bit 5-6-5
    @param    red   Red 8 bit color
    @param    green Green 8 bit color
    @param    blue  Blue 8 bit color
    @return   Unsigned 16-bit down-sampled color in 5-6-5 format
*/
/**************************************************************************/
uint16_t Adafruit_TFT::color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue) >> 3);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Adafruit_TFT::setRotation(uint8_t m) {
    rotation = m % 4; // can't be higher than 3

    switch (rotation) {
        case 0:
            m = (0x00 | MADCTL_RGB);
//			m=0x00;
            _width = _config.width;
            _height = _config.height;
            _smartX = _config.xStart;
            _smartY = _config.yStart;
			scrollTo(0);
            break;
        case 1:
            m = (MADCTL_MX | MADCTL_MV | MADCTL_RGB);
//			m=0x60;
            _width = _config.height;
            _height = _config.width;
            _smartX = _config.yStart;
            _smartY = _config.xStart;
			scrollTo(0);
            break;
        case 2:
            m = (MADCTL_MX | MADCTL_MY | MADCTL_RGB);
//			m=0xC0;
            _width = _config.width;
            _height = _config.height;
            _smartX = _config.xStart;
            _smartY = _config.yStart;
            scrollTo(_config.yStart);
            break;
        case 3:
            m = (MADCTL_MY | MADCTL_MV | MADCTL_RGB);
//			m=0xA0;
            _width = _config.height;
            _height = _config.width;
            _smartX = _config.yStart;
            _smartY = _config.xStart;
            scrollTo(_config.yStart);
            break;
    }

/*	
   switch (cwj) {
      case 0x00:    //！正
      case 0x20:    //逆时针 镜像
      case 0x40:    //正 镜像
      case 0x60:    //！顺时针
      case 0x80:    //倒立 镜像
      case 0xA0:    //！逆时针
      case 0xC0:    //！倒立	
      case 0xE0:    //顺时针 镜像
    }
*/
    startWrite();
    writeCommand(TFT_MADCTL);
    spiWrite(m);
    endWrite();
}

/**************************************************************************/
/*!
    @brief   Enable/Disable display color inversion
    @param   invert True to invert, False to have normal color
*/
/**************************************************************************/
void Adafruit_TFT::invertDisplay(boolean invert) {
    startWrite();
    writeCommand(invert ? TFT_INVON : TFT_INVOFF);
    endWrite();
}

/**************************************************************************/
/*!
    @brief   Scroll display memory
    @param   y How many pixels to scroll display by
*/
/**************************************************************************/
void Adafruit_TFT::scrollTo(uint16_t y) {
    startWrite();
    writeCommand(TFT_VSCRSADD);
    SPI_WRITE16(y);
    endWrite();
}

/**************************************************************************/
/*!
    @brief   Set the "address window" - the rectangle we will write to RAM with the next chunk of SPI data writes. The ST7789 will automatically wrap the data as each row is filled
    @param   x  TFT memory 'x' origin
    @param   y  TFT memory 'y' origin
    @param   w  Width of rectangle
    @param   h  Height of rectangle
*/
/**************************************************************************/
void Adafruit_TFT::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint32_t xa = ((uint32_t)(x + _smartX) << 16) | (x + w - 1 + _smartX);
    uint32_t ya = ((uint32_t)(y + _smartY) << 16) | (y + h - 1 + _smartY);
    writeCommand(TFT_CASET); // Column addr set
    SPI_WRITE32(xa);
    writeCommand(TFT_RASET); // Row addr set
    SPI_WRITE32(ya);
    writeCommand(TFT_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Blit 1 pixel of color without setting up SPI transaction
    @param   color 16-bits of 5-6-5 color data
*/
/**************************************************************************/
void Adafruit_TFT::pushColor(uint16_t color) {
	startWrite();
	SPI_WRITE16(color);
	endWrite();
}

/**************************************************************************/
/*!
    @brief   Blit 1 pixel of color without setting up SPI transaction
    @param   color 16-bits of 5-6-5 color data
*/
/**************************************************************************/
void Adafruit_TFT::writePixel(uint16_t color){
    SPI_WRITE16(color);
}

/**************************************************************************/
/*!
    @brief   Blit 'len' pixels of color without setting up SPI transaction
    @param   colors Array of 16-bit 5-6-5 color data
    @param   len Number of 16-bit pixels in colors array
*/
/**************************************************************************/
void Adafruit_TFT::writePixels(uint16_t * colors, uint32_t len){
    SPI_WRITE_PIXELS((uint8_t*)colors , len * 2);
}

/**************************************************************************/
/*!
    @brief   Blit 'len' pixels of a single color without setting up SPI transaction
    @param   color 16-bits of 5-6-5 color data
    @param   len Number of 16-bit pixels you want to write out with same color
*/
/**************************************************************************/
void Adafruit_TFT::writeColor(uint16_t color, uint32_t len){
	if(!len) return; // Avoid 0-byte transfers
	
#ifdef SPI_HAS_WRITE_PIXELS
    #define TMPBUF_LONGWORDS (SPI_MAX_PIXELS_AT_ONCE + 1) / 2
    #define TMPBUF_PIXELS    (TMPBUF_LONGWORDS * 2)
    static uint32_t temp[TMPBUF_LONGWORDS];
    uint32_t        c32    = color * 0x00010001;
    uint16_t        bufLen = (len < TMPBUF_PIXELS) ? len : TMPBUF_PIXELS,
                    xferLen, fillLen;

    // Fill temp buffer 32 bits at a time
    fillLen = (bufLen + 1) / 2; // Round up to next 32-bit boundary
    for(uint32_t t=0; t<fillLen; t++) {
        temp[t] = c32;
    }

    // Issue pixels in blocks from temp buffer
    while(len) {                                 // While pixels remain
        xferLen = (bufLen < len) ? bufLen : len; // How many this pass?
        writePixels((uint16_t *)temp, xferLen);
        len -= xferLen;
    }
#else
	uint8_t hi = color >> 8, lo = color;
    while(len--) {
        uint16_t _data;
	_data = (0xFF00 & (hi << 8)) | (lo);
	HSPI_WRITE16(_data);

       // HSPI_WRITE(hi);
       // HSPI_WRITE(lo);
    }
#endif
}

/**************************************************************************/
/*!
   @brief  Draw a single pixel, DOES NOT set up SPI transaction
    @param    x  TFT X location
    @param    y  TFT Y location
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_TFT::writePixel(int16_t x, int16_t y, uint16_t color) {
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x,y,1,1);
    writePixel(color);
}

/**************************************************************************/
/*!
   @brief  Fill a rectangle, DOES NOT set up SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    w  Width of rectangle
    @param    h  Height of rectangle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_TFT::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    if((x >= _width) || (y >= _height)) return;
    int16_t x2 = x + w - 1, y2 = y + h - 1;
    if((x2 < 0) || (y2 < 0)) return;

    // Clip left/top
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if(x2 >= _width)  w = _width  - x;
    if(y2 >= _height) h = _height - y;

    int32_t len = (int32_t)w * h;
    setAddrWindow(x, y, w, h);
    writeColor(color, len);
}


/**************************************************************************/
/*!
   @brief  Draw a vertical line, DOES NOT set up SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_TFT::writeFastVLine(int16_t x, int16_t y, int16_t l, uint16_t color){
    writeFillRect(x, y, 1, l, color);
}


/**************************************************************************/
/*!
   @brief  Draw a horizontal line, DOES NOT set up SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_TFT::writeFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color){
    writeFillRect(x, y, l, 1, color);
}

/**************************************************************************/
/*!
   @brief  Draw a single pixel, includes code for SPI transaction
    @param    x  TFT X location
    @param    y  TFT Y location
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_TFT::drawPixel(int16_t x, int16_t y, uint16_t color){
    startWrite();
    writePixel(x, y, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Draw a vertical line, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_TFT::drawFastVLine(int16_t x, int16_t y,
        int16_t l, uint16_t color) {
    startWrite();
    writeFastVLine(x, y, l, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Draw a horizontal line, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    l  Length of line in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_TFT::drawFastHLine(int16_t x, int16_t y,
        int16_t l, uint16_t color) {
    startWrite();
    writeFastHLine(x, y, l, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Fill a rectangle, includes code for SPI transaction
    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    w  Width of rectangle
    @param    h  Height of rectangle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_TFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color) {
    startWrite();
    writeFillRect(x,y,w,h,color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief  Draw RGB rectangle of data from RAM to a location on screen
   Adapted from https://github.com/PaulStoffregen/ST7789_t3
   by Marc MERLIN. See examples/pictureEmbed to use this.
   5/6/2017: function name and arguments have changed for compatibility
   with current GFX library and to avoid naming problems in prior
   implementation.  Formerly drawBitmap() with arguments in different order.

    @param    x  TFT X location begin
    @param    y  TFT Y location begin
    @param    pcolors Pointer to 16-bit color data
    @param    w  Width of pcolors rectangle
    @param    h  Height of pcolors rectangle
*/
/**************************************************************************/
void Adafruit_TFT::drawRGBBitmap(int16_t x, int16_t y,
  uint16_t *pcolors, int16_t w, int16_t h) {

    int16_t x2, y2; // Lower-right coord
    if(( x             >= _width ) ||      // Off-edge right
       ( y             >= _height) ||      // " top
       ((x2 = (x+w-1)) <  0      ) ||      // " left
       ((y2 = (y+h-1)) <  0)     ) return; // " bottom

    int16_t bx1=0, by1=0, // Clipped top-left within bitmap
            saveW=w;      // Save original bitmap width value
    if(x < 0) { // Clip left
        w  +=  x;
        bx1 = -x;
        x   =  0;
    }
    if(y < 0) { // Clip top
        h  +=  y;
        by1 = -y;
        y   =  0;
    }
    if(x2 >= _width ) w = _width  - x; // Clip right
    if(y2 >= _height) h = _height - y; // Clip bottom

    pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    startWrite();
    setAddrWindow(x, y, w, h); // Clipped area
    while(h--) { // For each (clipped) scanline...
      writePixels(pcolors, w); // Push one (clipped) row
      pcolors += saveW; // Advance pointer by one full (unclipped) line
    }
    endWrite();
}


/**************************************************************************/
/*!
   @brief  Read 8 bits of data from ST7789 configuration memory. NOT from RAM!
           This is highly undocumented/supported, it's really a hack but kinda works?
    @param    command  The command register to read data from
    @param    index  The byte index into the command to read from
    @return   Unsigned 8-bit data read from ST7789 register
*/
/**************************************************************************/
uint8_t Adafruit_TFT::readcommand8(uint8_t command, uint8_t index) {
    uint32_t freq = _freq;
    if(_freq > 24000000){
        _freq = 24000000;
    }
    startWrite();
    writeCommand(0xD9);  // woo sekret command?
    spiWrite(0x10 + index);
    writeCommand(command);
    uint8_t r = spiRead();
    endWrite();
    _freq = freq;
    return r;
}


/**************************************************************************/
/*!
   @brief  Begin SPI transaction, for hardware SPI
*/
/**************************************************************************/
void Adafruit_TFT::startWrite(void){
    SPI_BEGIN_TRANSACTION();
    if (_cs >= 0) {
    SPI_CS_LOW();
	}
}

/**************************************************************************/
/*!
   @brief  End SPI transaction, for hardware SPI
*/
/**************************************************************************/
void Adafruit_TFT::endWrite(void){
    if (_cs >= 0) {
    SPI_CS_HIGH();
	}
    SPI_END_TRANSACTION();
}

/**************************************************************************/
/*!
   @brief  Write 8-bit data to command/register (DataCommand line low).
   Does not set up SPI transaction.
   @param  cmd The command/register to transmit
*/
/**************************************************************************/
void Adafruit_TFT::writeCommand(uint8_t cmd){
    SPI_DC_LOW();
    spiWrite(cmd);
    SPI_DC_HIGH();
}


/**************************************************************************/
/*!
   @brief  Read 8-bit data via hardware SPI. Does not set up SPI transaction.
   @returns One byte of data from SPI
*/
/**************************************************************************/
uint8_t Adafruit_TFT::spiRead() {
    if(_miso < 0){
        return 0;
    }
    return HSPI_READ();
}

/**************************************************************************/
/*!
   @brief  Write 8-bit data via hardware SPI. Does not set up SPI transaction.
   @param  b Byte of data to write over SPI
*/
/**************************************************************************/
void Adafruit_TFT::spiWrite(uint8_t b) {
    HSPI_WRITE(b);
    return;
}

