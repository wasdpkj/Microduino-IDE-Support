// These are the pins used for the breakout example
#define VS_XRESET  -1      // VS1053 reset pin (output)
#define VS_XCS     A3     // VS1053 chip select pin (output)
#define VS_XDCS    A2      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 7     // Card chip select pin
#define VS_DREQ 3    //A1   // VS1053 Data request, ideally an Interrupt pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt


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

//software patch for MIDI Play
//const unsigned short  MIDIPatch[] = { /*if you don't let GPIO1 = H,please send this patch by spi*/
static const uint16_t  MIDIPatch[] PROGMEM = { /*if you don't let GPIO1 = H,please send this patch by spi*/
  0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /* 0 */
  0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /* 8 */
  0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /* 10 */
  0x0200, 0x000a, 0x0001, 0x0050
};

//software patch for Recording
static const uint16_t recPlugin[]PROGMEM = { /* Compressed plugin for recording*/
  0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /*    0 */
  0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /*    8 */
  0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /*   10 */
  0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /*   18 */
  0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e
};
