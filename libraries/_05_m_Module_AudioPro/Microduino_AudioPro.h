// 源自Adafruit_VS1053库
// 修订：@老潘orz  wasdpkj@hotmail.com
// 添加了多种功能和方法
// 播放速度、音量查询、音量整体设置、立体声模式、差分输出、ROM播放、SD音乐文件列表等

/*************************************************** 
  This is a library for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#ifndef AUDIOPRO_VS1053_H
#define AUDIOPRO_VS1053_H

#include <Arduino.h>

#include <SPI.h> 
#include <SD.h>
#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP32)
#include <pgmspace.h>
#endif


typedef volatile uint8_t PortReg;
typedef uint8_t PortMask;


#define VS1053_PIN_MIDI		D2 // There is no IRQ used on Seeduino
#define VS1053_PIN_XCS		A3//Control Chip Select Pin (for accessing SPI Control/Status registers)
#define VS1053_PIN_XDCS		A2//Data Chip Select / BSYNC Pin
#define VS1053_PIN_DREQ		D3 //Data Request Pin: Player asks for more data
#define SD_PIN_SEL		D7 //select pin for SD card

#define VS1053_DEFAULT_VOLUME	96

//#define VS1053_TIMER0_DREQ 255 // allows useInterrupt to accept pins 0 to 254

#define VS1053_CONTROL_SPI_SETTING  SPISettings(250000,  MSBFIRST, SPI_MODE0)
#define VS1053_DATA_SPI_SETTING     SPISettings(8000000, MSBFIRST, SPI_MODE0)
#define VS1053_SCI_READ 0x03
#define VS1053_SCI_WRITE 0x02

#define VS1053_REG_MODE  0x00
#define VS1053_REG_STATUS 0x01
#define VS1053_REG_BASS 0x02
#define VS1053_REG_CLOCKF 0x03
#define VS1053_REG_DECODETIME 0x04
#define VS1053_REG_AUDATA 0x05
#define VS1053_REG_WRAM 0x06
#define VS1053_REG_WRAMADDR 0x07
#define VS1053_REG_HDAT0 0x08
#define VS1053_REG_HDAT1 0x09
#define VS1053_REG_VOLUME 0x0B

#define VS1053_PARA_PLAYSPEED 0x1E04
#define VS1053_PARA_ENDFILL 0x1E06
#define VS1053_PARA_MONOOUTPUT 0x1E09
#define VS1053_PARA_POSITIONMSEC_L 0x1E27
#define VS1053_PARA_POSITIONMSEC_H 0x1E28
#define VS1053_PARA_RESYNC 0x1E29


#define VS1053_GPIO_DDR 0xC017
#define VS1053_GPIO_IDATA 0xC018
#define VS1053_GPIO_ODATA 0xC019

#define VS1053_INT_ENABLE  0xC01A

#define VS1053_MODE_SM_DIFF 0x0001
#define VS1053_MODE_SM_LAYER12 0x0002
#define VS1053_MODE_SM_RESET 0x0004
#define VS1053_MODE_SM_CANCEL 0x0008
#define VS1053_MODE_SM_EARSPKLO 0x0010
#define VS1053_MODE_SM_TESTS 0x0020
#define VS1053_MODE_SM_STREAM 0x0040
#define VS1053_MODE_SM_SDINEW 0x0800
#define VS1053_MODE_SM_ADPCM 0x1000
#define VS1053_MODE_SM_LINE1 0x4000
#define VS1053_MODE_SM_CLKRANGE 0x8000


#define VS1053_SCI_AIADDR 0x0A
#define VS1053_SCI_AICTRL0 0x0C
#define VS1053_SCI_AICTRL1 0x0D
#define VS1053_SCI_AICTRL2 0x0E
#define VS1053_SCI_AICTRL3 0x0F

#define VS1053_DATABUFFERLEN 32


// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
#define VS1053_GM1_ACOUSTIC_GRAND_PIANO 0
#define VS1053_GM1_PIANO 1
#define VS1053_GM1_ELECTRIC_GRAND_PIANO 2
#define VS1053_GM1_HONKY_TONK_PIANO 3
#define VS1053_GM1_RHODES_PIANO 4
#define VS1053_GM1_CHORUSED_PIANO 5
#define VS1053_GM1_OCARINA 30
#define VS1053_GM1_OCARINB 31
#define VS1053_GM1_OCARINC 32

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

#define VS1053_RAW 0
#define DECODE 1
#define VOLUME 1

//software patch for MIDI Play
//const unsigned short  MIDIPatch[] = { /*if you don't let GPIO1 = H,please send this patch by spi*/
static const uint16_t  MIDIPatch[] PROGMEM = { /*if you don't let GPIO1 = H,please send this patch by spi*/
  0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /* 0 */
  0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /* 8 */
  0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /* 10 */
  0x0200, 0x000a, 0x0001, 0x0050
};

//software patch for Recording
static const uint16_t recPlugin[] PROGMEM = { /* Compressed plugin for recording*/
  0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /*    0 */
  0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /*    8 */
  0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /*   10 */
  0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /*   18 */
  0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e
};

//空白的MP3文件，用来清缓存
static const uint8_t  panda[]  PROGMEM = {
  0x49, 0x44, 0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xE3, 0x18, 0xC0, 0x00, 0x08, 0x60, 0x06, 0x8A, 0x30, 0x08, 0x00, 0x00, 0x8D, 0x02, 0x79,
  0x02, 0x1A, 0x53, 0x4B, 0xF3, 0xEE, 0xF5, 0x2F, 0x28, 0xEE, 0x8F, 0xBF, 0x38, 0xFF, 0xFF, 0xFF,
  0xFF, 0x7C, 0xA1, 0x7F, 0xFF, 0xFD, 0x47, 0x0E, 0x7E, 0xAD, 0xBF, 0xF9, 0x74, 0xF9, 0x70, 0xB4,
  0x04, 0xB1, 0x00, 0x00, 0x03, 0x20, 0x00, 0x00, 0xAA, 0xA4, 0x29, 0x00, 0x65, 0x5C, 0x59, 0x9A,
  0x5B, 0x6F, 0x6D, 0x96, 0xB3, 0xF6, 0xFF, 0xFF, 0xFF, 0xE3, 0x18, 0xC0, 0x0F, 0x08, 0xD0, 0x06,
  0xA9, 0x90, 0x08, 0x00, 0x00, 0xFF, 0xFF, 0x9C, 0xEC, 0x77, 0xF5, 0xA9, 0x3B, 0x54, 0x9B, 0xAC,
  0xB7, 0xA2, 0xB6, 0x36, 0x9F, 0xD5, 0xDB, 0xCC, 0xC5, 0x80, 0xA6, 0x1C, 0x22, 0xC9, 0x4B, 0x90,
  0x5B, 0x15, 0xEB, 0xAB, 0x47, 0x9D, 0xD0, 0x8F, 0xFF, 0xFF, 0xFE, 0x5B, 0xAF, 0xFF, 0xFF, 0x6D,
  0x8A, 0x53, 0xBE, 0xC2, 0x2C, 0xF8, 0xF7, 0x78, 0x96, 0xD8, 0x6B, 0x5F, 0x14, 0x46, 0xA0, 0x26,
  0xFF, 0xE3, 0x18, 0xC0, 0x26, 0x09, 0x20, 0x02, 0xA9, 0x70, 0x00, 0x00, 0x00, 0x57, 0x75, 0x02,
  0xDA, 0xD8, 0x15, 0xFB, 0x13, 0xD9, 0x55, 0xFF, 0x58, 0xA7, 0x16, 0xFF, 0xF8, 0xBF, 0x15, 0x6E,
  0x2C, 0x2E, 0xCF, 0xFE, 0xAE, 0xA1, 0x76, 0x7E, 0xA1, 0x76, 0x62, 0xC2, 0xBA, 0x85, 0xFF, 0xAC,
  0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE3, 0x18, 0xC0, 0x3C, 0x09, 0x30, 0x02,
  0x6A, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint16_t panda_len = sizeof(panda);

/*
 * For use with AudioPro::flushCancel(flush_m) as to how to flush the VSdsp's buffer.
 * How to flush the VSdsp's buffer
 * See Data sheet 9.5.2
 */
enum flush_m {
  post,
  pre,
  both,
  none
};

static const char sdData[] = "AudioPro.dat";

class AudioPro {
 public:
  AudioPro(uint8_t midi = VS1053_PIN_MIDI, uint8_t cs = VS1053_PIN_XCS, uint8_t dcs = VS1053_PIN_XDCS, uint8_t dreq = VS1053_PIN_DREQ);
  uint8_t begin(void);
  void end();						//new
  void reset(void);

  boolean useInterrupt(uint8_t type = VS1053_PIN_DREQ);		//new
  boolean detachInterrupt(uint8_t type = VS1053_PIN_DREQ);	//new
  void feedBuffer(void);			//new
  volatile boolean playingMusic;	//new
  boolean paused(void);				//new
  boolean stopped(void);			//new

  void flushCancel(flush_m);		//new
  
  uint16_t getStatus(boolean sta = DECODE);				//new
     
  uint16_t sciRead(uint8_t addr);
  void sciWrite(uint8_t addr, uint16_t data);
  void sciWrite(uint8_t addr, uint8_t data_H, uint8_t data_L);	//new

  void sineTest(uint8_t n, uint16_t ms);

  uint16_t decodeTime(void);
  uint32_t decodeTimeMillis(void);

  uint16_t getVolume(boolean sta = VOLUME);				//new
  void setVolume(uint8_t left, uint8_t right);
  void setVolume(uint8_t left_right);			//new
  uint8_t volumeUp();							//new
  uint8_t volumeDown();							//new

  void setPlaySpeed(uint16_t);					//new
  uint16_t getPlaySpeed();						//new

  void setAmplifier(boolean sta);				//new
  boolean getAmplifier();						//new
  
  uint16_t getMonoMode();						//new
  void setMonoMode(uint16_t );					//new
  void setDifferentialOutput(uint16_t);			//new
  uint8_t getDifferentialOutput();				//new
  
  void playData(uint8_t *buffer, uint8_t buffsiz);
  void playBuffer(uint8_t *buffer, size_t buffsiz);
  boolean playROM(const uint8_t *_buffer, uint32_t _len);
  void stopPlaying(void);
  void pausePlaying(boolean pause);
  void applyPatch(const uint16_t *patch, uint16_t patchsize);

  void midiSetBank(uint8_t, uint8_t);
  void midiSetVolume(uint8_t, uint16_t);
  void midiSetInstrument(uint8_t, uint8_t);
  void noteOn(uint8_t, uint8_t, uint8_t);
  void noteOff(uint8_t, uint8_t, uint8_t);
  
  void startRecordOgg(boolean mic);
  void stopRecordOgg(void);
  uint16_t recordedWordsWaiting(void);
  uint16_t recordedReadWord(void);

 protected:
  uint8_t _midi, _dreq;

  boolean readyForData(void);
  uint8_t mp3buffer[VS1053_DATABUFFERLEN];

 private:
  uint8_t _cs, _dcs;

  boolean romTrack;							//new
  uint32_t romAddr;							//new
  uint32_t romLen;							//new
  uint32_t romLenCache;						//new
  
  void softReset(void);
  void spiwrite(uint8_t d);
  uint8_t spiread(void);

  uint16_t ReadWRAM(uint16_t);				//new
  void WriteWRAM(uint16_t, uint16_t);		//new 
};


class AudioPro_FilePlayer : public AudioPro {
 public:
  AudioPro_FilePlayer (SDClass& _sd,
					   uint8_t midi = VS1053_PIN_MIDI, uint8_t cs = VS1053_PIN_XCS, uint8_t dcs = VS1053_PIN_XDCS, uint8_t dreq = VS1053_PIN_DREQ);

  boolean begin(void);
  void end();

  boolean useInterrupt(uint8_t type = VS1053_PIN_DREQ);
  boolean detachInterrupt(uint8_t type = VS1053_PIN_DREQ);

  uint8_t getMusicNum();
  String getMusicName(uint8_t _FileNum);
  volatile boolean playingMusic;
 
  boolean playMP3(const char * trackname);
  boolean playMP3(String trackname);
  boolean playFullFile(const char *trackname);
  
  boolean playTrack(uint8_t trackNo);
  
  void feedBuffer(void);
 
  boolean paused(void);
  boolean stopped(void);
  
  void pausePlaying(boolean pause);
  void stopPlaying(void);

 private:
  SDClass& sd;
  File currentTrack;

  boolean mp3LenSta;						//new
  uint32_t mp3LenCache;						//new

  boolean staFile = false;
  uint8_t numMusicFile = 0;  
};


//------------------------------------------------------------------------------
/*
 * Global Functions
 */
boolean isFnMusic(char*);

//------------------------------------------------------------------------------
/*
 * Global Unions
 */

/**
 * \brief A handler for accessing bytes of a word.
 *
 * Often individual bytes of a word are handled by bit shifting e.g.
 * \code
 * Mp3WriteRegister(SCI_MODE, ((MP3SCI_MODE >> 8) & 0xFF), (MP3SCI_MODE & 0xFF) );
 * \endcode
 * This may lead to excessive shifts and worse; signed carries of the second shift,
 * If the casting is not correct.
 * This union allows a more efficient and simpler method to directly access the
 * individual bytes of the word.
 * And is convienent for handeling Endian issues. Where byte[0..1] can be
 * assigned specific endian and the word (aka uint16_t) can be used.
 */
union twobyte {

/**
 * \brief whole word value
 *
 * allows access and handeling of whole uint16_t (aka word) value
 */
  uint16_t word;

/**
 * \brief individual bytes
 *
 * allows access and handeling of individual uint8_t (aka bytes),
 * either MSB or LSB byte of word
 */
  uint8_t  byte[2];
} ;


#endif // AUDIOPRO_VS1053_H
