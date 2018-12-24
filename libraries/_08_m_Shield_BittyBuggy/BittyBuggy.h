#ifndef __BittyBuggy_H__
#define __BittyBuggy_H__

#if defined(__AVR__)
#include <SoftwareSerial.h>
#endif

//#define HSLCOLOR

#define HEADA 0x7A
#define HEADB 0x7B
#define HEADC 0x7C
#define HEADD 0x7D

#define MOTORCMD 0x01
#define BUZZERCMD 0x02
#define LINEFINDERCMD 0x03
#define COLORDISCERNCMD 0x04
#define SETCOLORCMD 0x05
#define SETBAUDCMD 0x06
#define GETLINECMD 0xD3
#define GETCOLORCMD 0xD5
#define VERSIONCMD 0xE0

#define STEP_WAIT_AA 1
#define STEP_WAIT_BB 2
#define STEP_WAIT_CMD 3
#define STEP_WAIT_LEN 4
#define STEP_WAIT_DATA 5
#define STEP_WAIT_BCC 6
#define STEP_SET_CMD 7
#define STEP_SET_RES 8

#define VERSION_LEN 1
#define DATA_RXLINE_LEN 2
#define DATA_RXCOLOR_LEN 6
#define MAXLEN 6

#define CARBAUD 38400

#define MINSPEED 45
#define MAXSPEED 255

#define MINFRE 20
#define MAXFRE 21000

#define FREE 0
#define BRAKE -500

#define TIMEOUT 30

#define COLOR_NONE 0X000000
#define COLOR_BLACK 0X000000
#define COLOR_WHITE 0XFFFFFF
#define COLOR_RED 0XFF0000
#define COLOR_ORANGE 0XEA8B17
#define COLOR_YELLOW 0XFFFF00
#define COLOR_GREEN 0X00FF00
#define COLOR_CYAN 0X00FFFF
#define COLOR_BLUE 0X0000FF
#define COLOR_PURPLE 0X4B0082
#define COLOR_WARM 0X9B6400
#define COLOR_COLD 0X648264

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 3

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

class BittyBuggy
{
  public:
#if defined(__AVR__)
    BittyBuggy(SoftwareSerial *ser);
    BittyBuggy(HardwareSerial *ser);
#elif defined(ESP32)
    BittyBuggy(HardwareSerial *ser, int _rx = D2, int _tx = D3);
#endif

    boolean begin(uint32_t _baud = CARBAUD);
    boolean available();
    uint8_t getChecksum(uint8_t _cmd, uint8_t *_data, uint8_t _len);
    boolean parse(uint8_t _inByte);
    void readBytes(uint8_t *_data, uint8_t _len);
    void readWords(int16_t *_data, uint8_t _len);
    void write(uint8_t cmd, uint8_t *_data, uint8_t _len);
    void getData(uint8_t _cmd);
    void setColorLED(uint8_t _cmd, uint8_t ar, uint8_t ag, uint8_t ab, uint8_t br, uint8_t bg, uint8_t bb);
    void setColorLEDA(uint8_t ar, uint8_t ag, uint8_t ab);
    void setColorLEDB(uint8_t br, uint8_t bg, uint8_t bb);
    void setColorLEDA(uint32_t _rgb);
    void setColorLEDB(uint32_t _rgb);
    void setAllLED(uint32_t _rgb);
    void setAllLED(uint8_t ar, uint8_t ag, uint8_t ab);
    void setSpeed(int16_t left, int16_t right);
    void setSpeedA(int16_t left);
    void setSpeedB(int16_t right);
    void tone(int16_t _fre, int16_t _time);
    void tone(int16_t _fre);
    void noTone();
    uint8_t getVersion();
    void getLineAlone(uint8_t *_array, uint8_t _colorA = RED, uint8_t _colorB = RED);
    void getLine(uint8_t *_array, uint8_t _color = RED);
    uint8_t getLineA(uint8_t _color = RED);
    uint8_t getLineB(uint8_t _color = RED);
    void getLineLowpassFilter(uint8_t *_array, uint8_t _color, float _num = 0.5);
    void getLineAverageFilter(uint8_t *_array, uint8_t _color, uint8_t len = 12);
    void getColorRaw(uint8_t *_array, uint8_t _ar = 255, uint8_t _ag = 255, uint8_t _ab = 255, uint8_t _br = 255, uint8_t _bg = 255, uint8_t _bb = 255);
    void getColor(uint8_t *_array);
    uint8_t getColorA();
    uint8_t getColorB();
    uint8_t connectionErr();

  private:
    HardwareSerial *carHwSerial;
#if defined(__AVR__)
    SoftwareSerial *carSwSerial;
#elif defined(ESP32)
    uint8_t pinRX = D2;
    uint8_t pinTX = D3;
#endif

    void common_init(void);
    uint8_t isValid(uint8_t value);
    uint8_t GetMax(uint8_t __R , uint8_t __G , uint8_t __B);
    uint8_t GetMin(uint8_t __R , uint8_t __G , uint8_t __B);
    boolean RGBtoHSV(uint8_t __R , uint8_t __G , uint8_t __B , float  HSV[3]);

    uint8_t uartRxstep;
    uint8_t uartRxindex;
    uint8_t datacmd;
    uint8_t datalen;
    uint8_t uartdata[MAXLEN];
    uint8_t connectionSta;
    uint8_t version;
    uint32_t last_time;
    uint8_t colorledData[6];
    uint8_t colorledDataCache[6];
    uint8_t carmotorData[4];
    int16_t motorspeed[2];
    uint8_t buzzerData[4];
    uint8_t colorledline[6];
    uint8_t ledcolor[6];
    uint8_t cachecolor[6];
    uint8_t linefilter[2];
    uint8_t setlineLED[2];
    uint8_t aloneColor[2];
    uint8_t Max, Min;
    float hsvcolorA[3];
    float hsvcolorB[3];
};

#endif
