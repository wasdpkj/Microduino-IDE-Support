/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 */

#ifndef IRremote_h
#define IRremote_h

#include <Arduino.h>

#if defined(LE501X)
#include "le501x-hal-timer.h"

#define INVALID_IROUT 255 // flag indicating an invalid servo index

#define HWTIMER_FOR_IROUT TIMER_2

#if (HWTIMER_FOR_IROUT == TIMER_0)
#define MAX_IROUT (4)
#elif (HWTIMER_FOR_IROUT == TIMER_1)
#define MAX_IROUT (4)
#elif (HWTIMER_FOR_IROUT == TIMER_2)
#define MAX_IROUT (2)
#elif (HWTIMER_FOR_IROUT == TIMER_3)
#define MAX_IROUT (4)
#endif

#define TIM_FRE 38
#define TIM_PERIOD (SDK_HCLK_MHZ * 1000000 / (HARDTIM_PRESCALER + 1) / (TIM_FRE*1000) - 1) /* Period Value  */

typedef struct
{
  uint8_t nbr : 6;      // a pin number from 0 to 63
  uint8_t isActive : 1; // true if this channel is enabled, pin not pulsed if false
} IRPin_t;

typedef struct
{
  IRPin_t Pin;
  uint8_t ticks;
  uint32_t Period;
} irout_t;

#endif /* LE501X */

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

// Results returned from the decoder
class decode_results {
public:
  int decode_type; // NEC, SONY, RC5, UNKNOWN
  unsigned long value; // Decoded value
  int bits; // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
};

// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

// main class for receiving IR
class IRrecv
{
public:
  IRrecv(int recvpin);
  int decode(decode_results *results);
  void enableIRIn();
  void resume();
private:
  // These are called by decode
  int getRClevel(decode_results *results, int *offset, int *used, int t1);
  long decodeNEC(decode_results *results);
  long decodeSony(decode_results *results);
  long decodeRC5(decode_results *results);
  long decodeRC6(decode_results *results);
};

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

class IRsend
{
public:
#if defined(ESP32)
  IRsend(int sendpin = D3);
#elif defined(LE501X)
  IRsend(int sendpin = D2, uint8_t timerindex = HWTIMER_FOR_IROUT);
#else
  IRsend() {}
#endif
  void sendMedia(unsigned char *data, int length);
  void sendNEC(unsigned long data, int nbits);
  void sendSony(unsigned long data, int nbits);
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendRC5(unsigned long data, int nbits);
  void sendRC6(unsigned long data, int nbits);
  // private:
  void enableIROut(int khz);
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
#if defined (LE501X)
  uint8_t irIndex;               // index into the channel data for this IR
  uint8_t htimerindex;
  uint8_t maxindex_ir;
#endif
};
// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 76 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#endif
