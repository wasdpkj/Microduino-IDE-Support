/**
\file AudioProConfig.h

\brief Hardware dependent configuration definitions
\remarks comments are implemented with Doxygen Markdown format

This AudioProConfig.h helps configure the AudioPro library for
various supported different base Arduino boards and shield's using the
VS10xx chip. It is possible this may support other VS10xx chips. But are
unverified at this time.
*/

#ifndef __AUDIOPRO_CONFIG_H__
#define __AUDIOPRO_CONFIG_H__

//------------------------------------------------------------------------------
/*
 * MP3 Player Shield pin mapping. See the appropiate schematic
 */

/**
 * \def MP3_XCS
 * \brief A macro to configure the XCS pin
 *
 * VS10xx's Control Chip Select Pin (for accessing SPI Control/Status registers)
 * as seen by the the Arduino
 *
 */

/**
 * \def MP3_XDCS
 * \brief A macro to configure the XDCS pin
 *
 * VS10xx's Data Chip Select Pin (for streaming data back and forth)
 * as seen by the the Arduino
 */

/**
 * \def MP3_DREQ
 * \brief A macro to configure the DREQ pin
 *
 * VS10xx's DREQ pin that indicates when it is clear to send more data.
 * aka Data REQuest.
 * as seen by the the Arduino
 */

/**
 * \def MP3_DREQINT
 * \brief A macro to configure the DREQINT pin
 *
 * The associated INT(X) pin name for the associated pin of DREQ, if used.
 * as seen by the the Arduino
 *
 * This may not be needed when USE_MP3_REFILL_MEANS is not equal to USE_MP3_REFILL_MEANS
 *
 * \sa USE_MP3_REFILL_MEANS
 */

/**
 * \def MP3_RESET
 * \brief A macro to configure the RESET pin
 *
 * VS10xx's RESET Pin
 * as seen by the the Arduino
 */

/**
 * \def SD_SEL
 * \brief A macro to configure the SdCard Chip Select for SFEMP3SHield library
 *
 * This is the pin of the Arduino that is connected to the SdCards Chip select pin.
 * This pin should be the same pin assigned in SdFat Library.
 * as seen by the the Arduino
 */

/**
 * \def PERF_MON_PIN
 * \brief A macro to configure a Pin to analyze performance
 *
 * The output of this pin will be low, during the refill of the VSdsp, allowing measurement of the CPU utilization, required to sustain playing.
 *
 * Set value to any available digital output, including A0-5...
 *
 * Set value to negative to disable.
 */
#define PERF_MON_PIN          -1 //  example of A5

#include <pins_arduino.h>


#define MP3_XCS             A3 //Control Chip Select Pin (for accessing SPI Control/Status registers)
#define MP3_XDCS            A2 //Data Chip Select / BSYNC Pin
#define MP3_DREQ            3 //Data Request Pin: Player asks for more data
#define MP3_DREQINT        	1 // There is no IRQ used on Seeduino
#define MP3_RESET           -1 //Reset is active low
#define SD_SEL              7 //select pin for SD card

//------------------------------------------------------------------------------
/**
 * \def MIDI_CHANNEL
 * \brief A macro used to specify the MIDI channel
 *
 * Where used in the SingleMIDInoteFile array for sending quick beeps with function SFEMP3Shield::SendSingleMIDInote()
 *
 * \note Where Ch9 is reserved for Percussion Instruments with single note
 */
#define MIDI_CHANNEL            0x09 

/**
 * \def MIDI_NOTE_NUMBER
 * \brief A macro used to specify the MIDI note
 *
 * Where used in the SingleMIDInoteFile array for sending quick beeps with function SFEMP3Shield::SendSingleMIDInote()
 *
 * \note So for Ch9's the note is GM Bank Percussion Instrument, not actual note. e.g 56 is cowbell. This removes the necassasity to send other commands.
 */
#define MIDI_NOTE_NUMBER        0x3A

/**
 * \def MIDI_NOTE_DURATION
 * \brief A macro used to specify the duration of the MIDI note
 *
 * Where used in the SingleMIDInoteFile array for sending quick beeps with function SFEMP3Shield::SendSingleMIDInote()
 *
 * \warning format is variable length, must keep it small. As not to break hardcoded header format
 */
#define MIDI_NOTE_DURATION       0x64

/**
 * \def MIDI_INTENSITY
 * \brief A macro used to specify the intensity of the MIDI note
 *
 * Value ranges from 0 to 127(full scale). Where used in the SingleMIDInoteFile array for sending both the ON and off of the quick beep with function SFEMP3Shield::SendSingleMIDInote()
 */
#define MIDI_INTENSITY         0x7F // Full scale.


#define PLAYING	2
#define PAUSE	1
#define END		0


#define VS1053_BANK_DEFAULT 	0x00
#define VS1053_BANK_DRUMS1 		0x78
#define VS1053_BANK_DRUMS2 		0x7F
#define VS1053_BANK_MELODY 		0x79


#define VS1053_GM1_ACOUSTIC_GRAND_PIANO 	0
#define VS1053_GM1_PIANO 					1
#define VS1053_GM1_ELECTRIC_GRAND_PIANO 	2
#define VS1053_GM1_HONKY_TONK_PIANO 		3
#define VS1053_GM1_RHODES_PIANO 			4
#define VS1053_GM1_CHORUSED_PIANO 			5
#define VS1053_GM1_OCARINA 					30
#define VS1053_GM1_OCARINB 					31
#define VS1053_GM1_OCARINC 					32

#define MIDI_NOTE_ON  		0x90
#define MIDI_NOTE_OFF 		0x80
#define MIDI_CHAN_MSG 		0xB0
#define MIDI_CHAN_BANK 		0x00
#define MIDI_CHAN_VOLUME 	0x07
#define MIDI_CHAN_PROGRAM 	0xC0
	

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


#endif  // __AUDIOPRO_CONFIG_H__
