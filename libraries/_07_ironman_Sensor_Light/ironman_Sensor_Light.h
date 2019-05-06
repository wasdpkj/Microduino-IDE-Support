#ifndef _ironmanLight_H_
#define _ironmanLight_H_

#include "Arduino.h"

#define SENSOR_ID       0x02

//#define HSLCOLOR

#define  INQUIRY_TIME 1000

#define COLORLIFHT_ADDRESS 0x02
#define OFFLINE 4

#define SEND_LEN_LIEN   2
#define SEND_LEN_COLOR  6

#define COLORLEDMODE    0
#define LINEMODE        1
#define COLORMODE       2

#define LIGHTID         0
#define LIGHTPOWER      2
#define LIGHTHANDWARE   3
#define LIGHTSOFTWARE   4
#define LIGHTERR        5
#define SENSORREST      6
#define NRFENABLE       7
#define SENSORPORT      8
#define SETAVRADDR      9
#define SETPOWER        10
#define SETMODE         17
#define GETLINEA        30
#define GETLINEB        32
#define GETCOLOR        34

#define BLACK 0
#define RED   1
#define GREEN 2
#define BLUE  3

#define SATURATED  0.25
#define BRIGHT     0.7

#define RED_S      0.3
#define GREEN_S    0.3
#define BLUE_S     0.28
#define YELLOW_S   0.3
#define PURPLE_S   0.26
#define CYAN_S     0.3

#define RED_MAX      340
#define RED_MIN      16
#define YELLOW_MIN   16
#define YELLOW_MAX   90
#define GREEN_MIN    90
#define GREEN_MAX    150
#define CYAN_MIN     150
#define CYAN_MAX     210
#define BLUE_MIN     210
#define BLUE_MAX     270
#define PURPLE_MIN   270
#define PURPLE_MAX   340

class ironmanLight {

  public:
    ironmanLight();
    boolean begin (uint8_t i2cAddr = COLORLIFHT_ADDRESS);
    boolean available();
    void write8(uint8_t addr, uint8_t d);
    void writecmddata(uint8_t addr, uint8_t cmd, uint8_t *_data, uint8_t _len);
    void Rest();
    void setNrfEN(boolean _en);
    void setPort(uint8_t port);
    void setAddr(uint8_t addr);
    void setPower(uint8_t _power);
    void setColorLEDA(int ar, int ag, int ab);
    void setColorLEDB(int br, int bg, int bb);
    void setColorLEDA(int32_t _rgb);
    void setColorLEDB(int32_t _rgb);
    void setAllLED(int32_t _rgb);
    void setAllLED(int ar, int ag, int ab);
    uint8_t getReg8(uint8_t _reg);
    uint16_t getReg7to8(uint8_t _reg);
    uint16_t getID();
    uint8_t getPower();
    uint8_t getHandware();
    uint8_t getSoftware();
    uint8_t getErr();
    uint8_t getNrfEn();
    void getLine(uint8_t *_line, uint8_t _colorA = RED, uint8_t _colorB = RED);
    uint8_t getLineA(uint8_t _colorA = RED);
    uint8_t getLineB(uint8_t _colorB = RED);
    void getColorRaw(uint8_t *_color, uint8_t _ar = 1, uint8_t _ag = 1, uint8_t _ab = 1, uint8_t __Br = 1, uint8_t __Bg = 1, uint8_t __Bb = 1);
    void getColor(uint8_t *_array);
    uint8_t getColorA();
    uint8_t getColorB();

  private:
    uint8_t isValid(uint8_t value);
    uint8_t GetMax(uint8_t __R , uint8_t __G , uint8_t __B);
    uint8_t GetMin(uint8_t __R , uint8_t __G , uint8_t __B);
    boolean RGBtoHSV(uint8_t __R , uint8_t __G , uint8_t __B , float  HSV[3]);

    boolean iicsta;

    uint8_t Max, Min;
    float hsvcolorA[3];
    float hsvcolorB[3];
    uint8_t cachecolor[6];
    uint8_t ledcolor[6];
    uint8_t aloneColor[2];
    uint8_t aloneline[2];

    uint8_t _i2cAddr;
    uint8_t colorledDataCache[12];
    uint8_t colorledline[12];
    uint8_t colorledcolor[12];

};

#endif
