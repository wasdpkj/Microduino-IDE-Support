/*
 * IRremote
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 */

#include "IRremote.h"
#include "IRremoteInt.h"

// Provides ISR
#if defined (__AVR__)
#include <avr/interrupt.h>
#elif defined (ESP32)
void IRTimer();
#elif defined(LE501X)

uint8_t IRoutCount = 0;
static irout_t irout[MAX_IROUT];

static inline void handle_interrupts(void);

static SW_TIM_HandleTypeDef IRSoftTimHandle = {
    .init{
        .period = -1,
        .timer = -1,
    },
    .channel = SOFTTIMER_CHANNEL_IR,
    .period = 0,
    .periodCalc = 0,
    .number = -1};
#define SOFT_IR_PERIOD_US (IRSoftTimHandle.period)

static void initISR(void)
{
    IRSoftTimHandle.init.period = -1;
    IRSoftTimHandle.init.timer = -1;

    IRSoftTimHandle.channel = SOFTTIMER_CHANNEL_IR;

    if (IRSoftTimHandle.init.period <= 0)
    {
        IRSoftTimHandle.period = DEFAULT_SWTIM_PERIOD;
    }
    else
    {
        IRSoftTimHandle.period = IRSoftTimHandle.init.period;
    }

    IRSoftTimHandle.periodCalc = 0;
    IRSoftTimHandle.number = -1;
    softTimerAttachInterrupt(&IRSoftTimHandle, handle_interrupts);
}

/*
static void finISR(void)
{
  // disable use of the given timer
  softTimerDetachInterrupt(&IRSoftTimHandle);
}
*/

static boolean isTimerActive(void)
{
  // returns true if any servo is active on this timer
  for (uint8_t channel = 0; channel < MAX_IROUT; channel++) 
  {
    if (irout[channel].Pin.isActive == true)
      return true;
  }
  return false;
}

static int32_t getperiod(uint8_t fre)
{
  if (TIM_FRE == fre)
  {
    return TIM_PERIOD;
  }
  return (SDK_HCLK_MHZ * 1000000 / (HARDTIM_PRESCALER + 1) / (fre * 1000) - 1);
}

#endif

volatile irparams_t irparams;

// These versions of MATCH, MATCH_MARK, and MATCH_SPACE are only for debugging.
// To use them, set DEBUG in IRremoteInt.h
// Normally macros are used for efficiency
#ifdef DEBUG
int MATCH(int measured, int desired) {
  Serial.print("Testing: ");
  Serial.print(TICKS_LOW(desired), DEC);
  Serial.print(" <= ");
  Serial.print(measured, DEC);
  Serial.print(" <= ");
  Serial.println(TICKS_HIGH(desired), DEC);
  return measured >= TICKS_LOW(desired) && measured <= TICKS_HIGH(desired);
}

int MATCH_MARK(int measured_ticks, int desired_us) {
  Serial.print("Testing mark ");
  Serial.print(measured_ticks * USECPERTICK, DEC);
  Serial.print(" vs ");
  Serial.print(desired_us, DEC);
  Serial.print(": ");
  Serial.print(TICKS_LOW(desired_us + MARK_EXCESS), DEC);
  Serial.print(" <= ");
  Serial.print(measured_ticks, DEC);
  Serial.print(" <= ");
  Serial.println(TICKS_HIGH(desired_us + MARK_EXCESS), DEC);
  return measured_ticks >= TICKS_LOW(desired_us - MARK_EXCESS) && measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS);
}

int MATCH_SPACE(int measured_ticks, int desired_us) {
  Serial.print("Testing space ");
  Serial.print(measured_ticks * USECPERTICK, DEC);
  Serial.print(" vs ");
  Serial.print(desired_us, DEC);
  Serial.print(": ");
  Serial.print(TICKS_LOW(desired_us - MARK_EXCESS), DEC);
  Serial.print(" <= ");
  Serial.print(measured_ticks, DEC);
  Serial.print(" <= ");
  Serial.println(TICKS_HIGH(desired_us - MARK_EXCESS), DEC);
  return measured_ticks >= TICKS_LOW(desired_us - MARK_EXCESS) && measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS);
}
#endif

#if defined (ESP32)
IRsend::IRsend(int sendpin)
{
  ledcAttachPin(sendpin, LEDC_channel_IR); 
}
#elif defined (LE501X)
IRsend::IRsend(int sendpin, uint8_t timerindex)
{
  switch (timerindex)
  {
  case TIMER_3:
    maxindex_ir = 4;
    htimerindex = TIMER_3;
    break;
  case TIMER_2:
    maxindex_ir = 2;
    htimerindex = TIMER_2;
    break;
  case TIMER_1:
    maxindex_ir = 4;
    htimerindex = TIMER_1;
    break;
  case TIMER_0:
    maxindex_ir = 4;
    htimerindex = TIMER_0;
    break;
  default:
    maxindex_ir = MAX_IROUT;
    htimerindex = HWTIMER_FOR_IROUT;
    break;
  }

  if (IRoutCount < maxindex_ir) {
    this->irIndex = IRoutCount;
    IRoutCount++;
    irout[this->irIndex].Pin.nbr = sendpin;
  }
  else {
    this->irIndex = INVALID_IROUT;
  }
}
#endif

void IRsend::sendMedia(unsigned char *data, int length)
{
  enableIROut(38);
  mark(4400);
  space(4400);
  for(int i = 0; i < length; i++){
	for(int j = 0; j < 8; j++){
		if((*(data+i))&(0x80>>j)){
			mark(540);
			space(1620);
		}
		else{
			mark(540);
			space(540);
		}
	}
  }
  mark(540);
  space(5220);
  mark(4400);
  space(4400);
  for(int i = 0; i < length; i++){
	for(int j = 0; j < 8; j++){
		if((*(data+i))&(0x80>>j)){
			mark(540);
			space(1620);
		}
		else{
			mark(540);
			space(540);
		}
	}
  }
  mark(540);
  space(0);
}

void IRsend::sendNEC(unsigned long data, int nbits)
{
  enableIROut(38);
  mark(NEC_HDR_MARK);
  space(NEC_HDR_SPACE);
  for (int i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      mark(NEC_BIT_MARK);
      space(NEC_ONE_SPACE);
    } 
    else {
      mark(NEC_BIT_MARK);
      space(NEC_ZERO_SPACE);
    }
    data <<= 1;
  }
  mark(NEC_BIT_MARK);
  space(0);
}

void IRsend::sendSony(unsigned long data, int nbits) {
  enableIROut(40);
  mark(SONY_HDR_MARK);
  space(SONY_HDR_SPACE);
  data = data << (32 - nbits);
  for (int i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      mark(SONY_ONE_MARK);
      space(SONY_HDR_SPACE);
    } 
    else {
      mark(SONY_ZERO_MARK);
      space(SONY_HDR_SPACE);
    }
    data <<= 1;
  }
}

void IRsend::sendRaw(unsigned int buf[], int len, int hz)
{
  enableIROut(hz);
  for (int i = 0; i < len; i++) {
    if (i & 1) {
      space(buf[i]);
    } 
    else {
      mark(buf[i]);
    }
  }
  space(0); // Just to be sure
}

// Note: first bit must be a one (start bit)
void IRsend::sendRC5(unsigned long data, int nbits)
{
  enableIROut(36);
  data = data << (32 - nbits);
  mark(RC5_T1); // First start bit
  space(RC5_T1); // Second start bit
  mark(RC5_T1); // Second start bit
  for (int i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      space(RC5_T1); // 1 is space, then mark
      mark(RC5_T1);
    } 
    else {
      mark(RC5_T1);
      space(RC5_T1);
    }
    data <<= 1;
  }
  space(0); // Turn off at end
}

// Caller needs to take care of flipping the toggle bit
void IRsend::sendRC6(unsigned long data, int nbits)
{
  enableIROut(36);
  data = data << (32 - nbits);
  mark(RC6_HDR_MARK);
  space(RC6_HDR_SPACE);
  mark(RC6_T1); // start bit
  space(RC6_T1);
  int t;
  for (int i = 0; i < nbits; i++) {
    if (i == 3) {
      // double-wide trailer bit
      t = 2 * RC6_T1;
    } 
    else {
      t = RC6_T1;
    }
    if (data & TOPBIT) {
      mark(t);
      space(t);
    } 
    else {
      space(t);
      mark(t);
    }

    data <<= 1;
  }
  space(0); // Turn off at end
}

void IRsend::mark(int time) {
  // Sends an IR mark for the specified number of microseconds.
  // The mark output is modulated at the PWM frequency.
#if defined (ESP32)
  ledcWrite(LEDC_channel_IR, 100);
#elif defined (LE501X)
  pwmUpdata(this->irIndex, irout[this->irIndex].Period / 3);
#elif defined (__AVR_ATmega32U4__)
  TCCR4A |= _BV(COM4A1); 
#elif defined(__AVR_ATmega128RFA1__)
  TCCR3A |= _BV(COM3B1);
#else
  TCCR2A |= _BV(COM2B1); // Enable pin 3 PWM output
#endif
  delayMicroseconds(time);

}

/* Leave pin off for time (given in microseconds) */
void IRsend::space(int time) {
  // Sends an IR space for the specified number of microseconds.
  // A space is no output, so the PWM output is disabled.
#if defined (ESP32)
  ledcWrite(LEDC_channel_IR, 0); 
#elif defined (LE501X)
  pwmUpdata(this->irIndex, 0);
#elif defined (__AVR_ATmega32U4__)
  TCCR4A &= ~(_BV(COM4A1));
#elif defined(__AVR_ATmega128RFA1__)
  TCCR3A &= ~(_BV(COM3B1));
#else
  TCCR2A &= ~(_BV(COM2B1)); // Disable pin 3 PWM output
#endif
  delayMicroseconds(time);
}

void IRsend::enableIROut(int khz) {
  // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
  // The IR output will be on pin 3 (OC2B).
  // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
  // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
  // TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
  // controlling the duty cycle.
  // There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
  // To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
  // A few hours staring at the ATmega documentation and this will all make sense.
  // See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.

  
  // Disable the Timer2 Interrupt (which is used for receiving IR)
#if defined (ESP32)
  ledcSetup(LEDC_channel_IR, khz*1000, LEDC_TIMER_8BIT);
#elif defined (LE501X)
  if (isTimerActive() == false)
  {
    irout[this->irIndex].Pin.isActive = true; // this must be set after the check for isTimerActive
    if (irout[this->irIndex].ticks != khz)
    {
      irout[this->irIndex].ticks = khz;
      irout[this->irIndex].Period = (uint32_t)getperiod(irout[this->irIndex].ticks);
      pwmDeinit();
      pwmInit(htimerindex, irout[this->irIndex].Period);
      pwmAttachPin(irout[this->irIndex].Pin.nbr, this->irIndex);
    }
  }
#elif defined (__AVR_ATmega32U4__)
	TIMSK4 &= ~_BV(TOIE4);
#elif defined(__AVR_ATmega128RFA1__)
	TIMSK3 &= ~_BV(TOIE3); //Timer3 Overflow Interrupt
#else
	TIMSK2 &= ~_BV(TOIE2); //Timer2 Overflow Interrupt
#endif
 
#if defined (ESP32)

#elif defined (LE501X)

#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // When not sending PWM, we want it low
#elif defined(__AVR_ATmega128RFA1__)
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
#elif defined(__AVR_ATmega32U4__)
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
#else 
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW); // When not sending PWM, we want it low
#endif
 
  
  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2 = 000: no prescaling
#if defined (ESP32)
  
#elif defined (LE501X)

#elif defined (__AVR_ATmega32U4__)
  TCCR4B = _BV(PWM4X) | _BV(CS40);
  TCCR4D = _BV(WGM40);
  
  OCR4A = SYSCLOCK / 2 / khz / 1000;
  OCR4B = OCR4A / 3;
#elif defined(__AVR_ATmega128RFA1__)
  TCCR3A = _BV(WGM30);
  TCCR3B = _BV(WGM32) | _BV(CS30);

  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  OCR3A = SYSCLOCK / 2 / khz / 1000;
  OCR3B = OCR3A / 3; // 33% duty cycle 
#else  
  TCCR2A = _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);

  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  OCR2A = SYSCLOCK / 2 / khz / 1000;
  OCR2B = OCR2A / 3; // 33% duty cycle
#endif 
}

IRrecv::IRrecv(int recvpin)
{
  irparams.recvpin = recvpin;
}

// initialization
void IRrecv::enableIRIn() {
  // setup pulse clock timer interrupt
#if defined (ESP32)
  hw_timer_t *timer;
  timer = timerBegin(1, 80, 1); 
  timerAttachInterrupt(timer, &IRTimer, 1); 
  // every 50ns, autoreload = true 
  timerAlarmWrite(timer, USECPERTICK, true); 
  timerAlarmEnable(timer);  
  
#elif defined (__AVR_ATmega32U4__)
  sbi(TCCR4B,PWM4X);

  cbi(TCCR4B,CS43);
  sbi(TCCR4B,CS42);
  cbi(TCCR4B,CS41);
  cbi(TCCR4B,CS40);
  
  cbi(TCCR4D,WGM41);
  cbi(TCCR4D,WGM40);
  sbi(TIMSK4, TOIE4);
  
  RESET_TIMER4;
  sei();  // enable interrupts
#elif defined(__AVR_ATmega128RFA1__)
  TCCR3A = 0;  // normal mode

  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  cbi(TCCR3B,CS32);
  sbi(TCCR3B,CS31);
  cbi(TCCR3B,CS30);

  //Timer3 Overflow Interrupt Enable
  sbi(TIMSK3,TOIE3);

  RESET_TIMER3;
  sei();  // enable interrupts
#elif defined(LE501X)
  initISR();
#else
  TCCR2A = 0;  // normal mode

  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  cbi(TCCR2B,CS22);
  sbi(TCCR2B,CS21);
  cbi(TCCR2B,CS20);

  //Timer2 Overflow Interrupt Enable
  sbi(TIMSK2,TOIE2);

  RESET_TIMER2;
  sei();  // enable interrupts
#endif
 
  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // set pin modes
  pinMode(irparams.recvpin, INPUT);
}


// TIMER2 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts
#if defined (ESP32)
void IRTimer()
{
#elif defined (__AVR_ATmega32U4__)
ISR(TIMER4_OVF_vect)
{
  RESET_TIMER4;
#elif defined(__AVR_ATmega128RFA1__)
ISR(TIMER3_OVF_vect)
{
  RESET_TIMER3;
#elif defined(LE501X)
static inline void handle_interrupts(void)
{
#else
ISR(TIMER2_OVF_vect)
{
  RESET_TIMER2;
#endif
  uint8_t irdata = (uint8_t)digitalRead(irparams.recvpin);

  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= RAWBUF) {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }
  switch(irparams.rcvstate) {
  case STATE_IDLE: // In the middle of a gap
    if (irdata == MARK) {
#if defined(LE501X)
      irparams.timer = irparams.timer * SOFT_IR_PERIOD_US / USECPERTICK;
#endif
      if (irparams.timer < GAP_TICKS) {
        // Not big enough to be a gap.
        irparams.timer = 0;
      } 
      else {
        // gap just ended, record duration and start recording transmission
        irparams.rawlen = 0;
        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
        irparams.timer = 0;
        irparams.rcvstate = STATE_MARK;
      }
    }
    break;
  case STATE_MARK: // timing MARK
    if (irdata == SPACE) {   // MARK ended, record time
#if defined(LE501X)
      irparams.timer = irparams.timer * SOFT_IR_PERIOD_US / USECPERTICK;
#endif
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_SPACE;
    }
    break;
  case STATE_SPACE: // timing SPACE
    if (irdata == MARK) { // SPACE just ended, record it
#if defined(LE501X)
      irparams.timer = irparams.timer * SOFT_IR_PERIOD_US / USECPERTICK;
#endif
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_MARK;
    }
    else { // SPACE
#if defined(LE501X)
      if (irparams.timer > (unsigned int)(GAP_TICKS * USECPERTICK / SOFT_IR_PERIOD_US)) {
#else
      if (irparams.timer > GAP_TICKS) {
#endif
        // big SPACE, indicates gap between codes
        // Mark current code as ready for processing
        // Switch to STOP
        // Don't reset timer; keep counting space width
        irparams.rcvstate = STATE_STOP;
      }
    }
    break;
  case STATE_STOP: // waiting, measuring gap
    if (irdata == MARK) { // reset gap timer
      irparams.timer = 0;
    }
    break;
  }
}

void IRrecv::resume() {
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}



// Decodes the received IR message
// Returns 0 if no data ready, 1 if data ready.
// Results of decoding are stored in results
int IRrecv::decode(decode_results *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;

  if (irparams.rcvstate != STATE_STOP) {
    return ERR;
  }
#ifdef DEBUG
  Serial.println("Attempting NEC decode");
#endif
  if (decodeNEC(results)) {
    return DECODED;
  }
#ifdef DEBUG
  Serial.println("Attempting Sony decode");
#endif
  if (decodeSony(results)) {
    return DECODED;
  }
#ifdef DEBUG
  Serial.println("Attempting RC5 decode");
#endif  
  if (decodeRC5(results)) {
    return DECODED;
  }
#ifdef DEBUG
  Serial.println("Attempting RC6 decode");
#endif 
  if (decodeRC6(results)) {
    return DECODED;
  }
  if (results->rawlen >= 6) {
    // Only return raw buffer if at least 6 bits
    results->decode_type = UNKNOWN;
    results->bits = 0;
    results->value = 0;
    return DECODED;
  }
  // Throw away and start over
  resume();
  return ERR;
}

long IRrecv::decodeNEC(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK)) {
    return ERR;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], NEC_RPT_SPACE) &&
    MATCH_MARK(results->rawbuf[offset+1], NEC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = NEC;
    return DECODED;
  }
  if (irparams.rawlen < 2 * NEC_BITS + 4) {
    return ERR;
  }
  // Initial space  
  if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  for (int i = 0; i < NEC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK)) {
      return ERR;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE)) {
      data <<= 1;
    } 
    else {
      return ERR;
    }
    offset++;
  }
  // Success
  results->bits = NEC_BITS;
  results->value = data;
  results->decode_type = NEC;
  return DECODED;
}

long IRrecv::decodeSony(decode_results *results) {
  long data = 0;
  if (irparams.rawlen < 2 * SONY_BITS + 2) {
    return ERR;
  }
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], SONY_HDR_MARK)) {
    return ERR;
  }
  offset++;

  while (offset + 1 < irparams.rawlen) {
    if (!MATCH_SPACE(results->rawbuf[offset], SONY_HDR_SPACE)) {
      break;
    }
    offset++;
    if (MATCH_MARK(results->rawbuf[offset], SONY_ONE_MARK)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_MARK(results->rawbuf[offset], SONY_ZERO_MARK)) {
      data <<= 1;
    } 
    else {
      return ERR;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < 12) {
    results->bits = 0;
    return ERR;
  }
  results->value = data;
  results->decode_type = SONY;
  return DECODED;
}

// Gets one undecoded level at a time from the raw buffer.
// The RC5/6 decoding is easier if the data is broken into time intervals.
// E.g. if the buffer has MARK for 2 time intervals and SPACE for 1,
// successive calls to getRClevel will return MARK, MARK, SPACE.
// offset and used are updated to keep track of the current position.
// t1 is the time interval for a single bit in microseconds.
// Returns -1 for error (measured time interval is not a multiple of t1).
int IRrecv::getRClevel(decode_results *results, int *offset, int *used, int t1) {
  if (*offset >= results->rawlen) {
    // After end of recorded buffer, assume SPACE.
    return SPACE;
  }
  int width = results->rawbuf[*offset];
  int val = ((*offset) % 2) ? MARK : SPACE;
  int correction = (val == MARK) ? MARK_EXCESS : - MARK_EXCESS;

  int avail;
  if (MATCH(width, t1 + correction)) {
    avail = 1;
  } 
  else if (MATCH(width, 2*t1 + correction)) {
    avail = 2;
  } 
  else if (MATCH(width, 3*t1 + correction)) {
    avail = 3;
  } 
  else {
    return -1;
  }

  (*used)++;
  if (*used >= avail) {
    *used = 0;
    (*offset)++;
  }
#ifdef DEBUG
  if (val == MARK) {
    Serial.println("MARK");
  } 
  else {
    Serial.println("SPACE");
  }
#endif
  return val;   
}

long IRrecv::decodeRC5(decode_results *results) {
  if (irparams.rawlen < MIN_RC5_SAMPLES + 2) {
    return ERR;
  }
  int offset = 1; // Skip gap space
  long data = 0;
  int used = 0;
  // Get start bits
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return ERR;
  if (getRClevel(results, &offset, &used, RC5_T1) != SPACE) return ERR;
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return ERR;
  int nbits;
  for (nbits = 0; offset < irparams.rawlen; nbits++) {
    int levelA = getRClevel(results, &offset, &used, RC5_T1); 
    int levelB = getRClevel(results, &offset, &used, RC5_T1);
    if (levelA == SPACE && levelB == MARK) {
      // 1 bit
      data = (data << 1) | 1;
    } 
    else if (levelA == MARK && levelB == SPACE) {
      // zero bit
      data <<= 1;
    } 
    else {
      return ERR;
    } 
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = RC5;
  return DECODED;
}

long IRrecv::decodeRC6(decode_results *results) {
  if (results->rawlen < MIN_RC6_SAMPLES) {
    return ERR;
  }
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], RC6_HDR_MARK)) {
    return ERR;
  }
  offset++;
  if (!MATCH_SPACE(results->rawbuf[offset], RC6_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  long data = 0;
  int used = 0;
  // Get start bit (1)
  if (getRClevel(results, &offset, &used, RC6_T1) != MARK) return ERR;
  if (getRClevel(results, &offset, &used, RC6_T1) != SPACE) return ERR;
  int nbits;
  for (nbits = 0; offset < results->rawlen; nbits++) {
    int levelA, levelB; // Next two levels
    levelA = getRClevel(results, &offset, &used, RC6_T1); 
    if (nbits == 3) {
      // T bit is double wide; make sure second half matches
      if (levelA != getRClevel(results, &offset, &used, RC6_T1)) return ERR;
    } 
    levelB = getRClevel(results, &offset, &used, RC6_T1);
    if (nbits == 3) {
      // T bit is double wide; make sure second half matches
      if (levelB != getRClevel(results, &offset, &used, RC6_T1)) return ERR;
    } 
    if (levelA == MARK && levelB == SPACE) { // reversed compared to RC5
      // 1 bit
      data = (data << 1) | 1;
    } 
    else if (levelA == SPACE && levelB == MARK) {
      // zero bit
      data <<= 1;
    } 
    else {
      return ERR; // Error
    } 
  }
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = RC6;
  return DECODED;
}
