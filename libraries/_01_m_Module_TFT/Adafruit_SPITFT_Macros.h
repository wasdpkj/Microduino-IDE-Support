#ifndef _ADAFRUIT_SPITFT_MACROS
#define _ADAFRUIT_SPITFT_MACROS

#include <SPI.h>

/*
 * Control Pins
 * */

#ifdef USE_FAST_PINIO
#define SPI_DC_HIGH()           *dcport |=  dcpinmask   ///< set DataCommand pin high
#define SPI_DC_LOW()            *dcport &= ~dcpinmask   ///< set DataCommand pin low
#define SPI_CS_HIGH()           *csport |= cspinmask    ///< set ChipSelect pin high
#define SPI_CS_LOW()            *csport &= ~cspinmask   ///< set ChipSelect pin high
#else
#define SPI_DC_HIGH()           digitalWrite(_dc, HIGH) ///< set DataCommand pin high
#define SPI_DC_LOW()            digitalWrite(_dc, LOW)  ///< set DataCommand pin low
#define SPI_CS_HIGH()           digitalWrite(_cs, HIGH) ///< set ChipSelect pin high
#define SPI_CS_LOW()            digitalWrite(_cs, LOW)  ///< set ChipSelect pin high
#endif

/*
 * Hardware SPI Macros
 * */

#ifndef ESP32
    #define SPI_OBJECT  SPI         ///< Default SPI hardware peripheral
#else
    #define SPI_OBJECT  _spi        ///< Default SPI hardware peripheral
#endif

#if defined (__AVR__) ||  defined(TEENSYDUINO) ||  defined(ARDUINO_ARCH_STM32F1)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClockDivider(SPI_CLOCK_DIV2);
#elif defined (__arm__)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClockDivider(11);
#elif defined(ESP8266) || defined(ESP32)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setFrequency(_freq);
#elif defined(RASPI)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClock(_freq);
#elif defined(ARDUINO_ARCH_STM32F1)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClock(_freq);
#else
    #define HSPI_SET_CLOCK()   ///< Hardware SPI set clock frequency
#endif

#ifdef SPI_HAS_TRANSACTION
    #define HSPI_BEGIN_TRANSACTION() SPI_OBJECT.beginTransaction(SPISettings(_freq, MSBFIRST, SPI_MODE0))
    #define HSPI_END_TRANSACTION()   SPI_OBJECT.endTransaction()
#else
    #define HSPI_BEGIN_TRANSACTION() HSPI_SET_CLOCK(); SPI_OBJECT.setBitOrder(MSBFIRST); SPI_OBJECT.setDataMode(SPI_MODE0)        ///< Hardware SPI begin transaction
    #define HSPI_END_TRANSACTION()    ///< Hardware SPI end transaction
#endif

#ifdef ESP32
    #define SPI_HAS_WRITE_PIXELS
#endif
#if defined(ESP8266) || defined(ESP32)
    // Optimized SPI (ESP8266 and ESP32)
    #define HSPI_READ()              SPI_OBJECT.transfer(0)    ///< Hardware SPI read 8 bits
    #define HSPI_WRITE(b)            SPI_OBJECT.write(b)       ///< Hardware SPI write 8 bits
    #define HSPI_WRITE16(s)          SPI_OBJECT.write16(s)     ///< Hardware SPI write 16 bits
    #define HSPI_WRITE32(l)          SPI_OBJECT.write32(l)     ///< Hardware SPI write 32 bits
    #ifdef SPI_HAS_WRITE_PIXELS
        #define SPI_MAX_PIXELS_AT_ONCE  32
        #define HSPI_WRITE_PIXELS(c,l)   SPI_OBJECT.writePixels(c,l)
    #else
        #define HSPI_WRITE_PIXELS(c,l)   for(uint32_t i=0; i<((l)/2); i++){ SPI_WRITE16(((uint16_t*)(c))[i]); }
    #endif
#else
    // Standard Byte-by-Byte SPI

    #if defined (__AVR__) || defined(TEENSYDUINO)
static inline uint8_t _avr_spi_read(void) __attribute__((always_inline));
static inline uint8_t _avr_spi_read(void) {
    uint8_t r = 0;
    SPDR = r;
    while(!(SPSR & _BV(SPIF)));
    r = SPDR;
    return r;
}
        #define HSPI_WRITE(b)            {SPDR = (b); while(!(SPSR & _BV(SPIF)));}
        #define HSPI_READ()              _avr_spi_read()
    #else
        #define HSPI_WRITE(b)            SPI_OBJECT.transfer((uint8_t)(b))    ///< Hardware SPI write 8 bits
        #define HSPI_READ()              HSPI_WRITE(0)    ///< Hardware SPI read 8 bits
    #endif
    #define HSPI_WRITE16(s)          HSPI_WRITE((s) >> 8); HSPI_WRITE(s)  ///< Hardware SPI write 16 bits
    #define HSPI_WRITE32(l)          HSPI_WRITE((l) >> 24); HSPI_WRITE((l) >> 16); HSPI_WRITE((l) >> 8); HSPI_WRITE(l)          ///< Hardware SPI write 32 bits
    #define HSPI_WRITE_PIXELS(c,l)   for(uint32_t i=0; i<(l); i+=2){ HSPI_WRITE(((uint8_t*)(c))[i+1]); HSPI_WRITE(((uint8_t*)(c))[i]); }       ///< Hardware SPI write 'l' pixels 16-bits each
#endif

/*
 * Final SPI Macros
 * */
#if defined (ARDUINO_ARCH_ARC32) || defined (ARDUINO_MAXIM)
#define SPI_DEFAULT_FREQ         16000000
#elif defined (__AVR__) || defined(TEENSYDUINO)
#define SPI_DEFAULT_FREQ         8000000
#elif defined(ESP8266) || defined(ESP32)
#define SPI_DEFAULT_FREQ         40000000
#elif defined(RASPI)
#define SPI_DEFAULT_FREQ         80000000
#elif defined(ARDUINO_ARCH_STM32F1)
#define SPI_DEFAULT_FREQ         36000000
#else
#define SPI_DEFAULT_FREQ         24000000      ///< Default SPI data clock frequency
#endif

#if defined(ESP8266) || defined(ESP32)
#define SPI_BEGIN()             if(_sclk < 0){SPI_OBJECT.begin();}else{SPI_OBJECT.begin(_sclk,_miso,_mosi,-1);}          ///< SPI initialize
#else
#define SPI_BEGIN()             SPI_OBJECT.begin();          ///< SPI initialize
#endif
#define SPI_BEGIN_TRANSACTION() HSPI_BEGIN_TRANSACTION();    ///< SPI begin transaction
#define SPI_END_TRANSACTION()   HSPI_END_TRANSACTION();      ///< SPI end transaction
#define SPI_WRITE16(s)          HSPI_WRITE16(s);  ///< SPI write 16 bits
#define SPI_WRITE32(l)          HSPI_WRITE32(l);  ///< SPI write 32 bits
#define SPI_WRITE_PIXELS(c,l)   HSPI_WRITE_PIXELS(c,l);  ///< SPI write 'l' pixels of 16-bits each

#endif // _ADAFRUIT_SPITFT_MACROS