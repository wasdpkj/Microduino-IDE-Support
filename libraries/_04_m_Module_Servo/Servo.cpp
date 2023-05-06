/*
  Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
  Copyright (c) 2009 Michael Margolis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*

  A servo is activated by creating an instance of the Servo class passing the desired pin to the attach() method.
  The servos are pulsed in the background using the value most recently written using the write() method

  Note that analogWrite of PWM on pins associated with the timer are disabled when the first servo is attached.
  Timers are seized as needed in groups of 12 servos - 24 servos use two timers, 48 servos will use four.

  The methods are:

  Servo - Class for manipulating servo motors connected to Arduino pins.

  attach(pin )  - Attaches a servo motor to an i/o pin.
  attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
  default min is 544, max is 2400

  write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
  writeMicroseconds() - Sets the servo pulse width in microseconds
  read()      - Gets the last written servo pulse width as an angle between 0 and 180.
  readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
  attached()  - Returns true if there is a servo attached.
  detach()    - Stops an attached servos from pulsing its i/o pin.

  //修改记录：兼容ESP32, 2017-09-07，by CWJ  

*/
#include <Arduino.h>
#include "Servo.h"

#if defined(__AVR__)
#include <avr/interrupt.h>

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds

#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays // 12 August 2009

//#define NBR_TIMERS        (MAX_SERVOS / SERVOS_PER_TIMER)

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

uint8_t ServoCount = 0;                                     // the total number of attached servos

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo 

/************ static functions common to all instances ***********************/

static inline void handle_interrupts(timer16_Sequence_t timer, volatile uint16_t *TCNTn, volatile uint16_t* OCRnA)
{
  if ( Channel[timer] < 0 )
    *TCNTn = 0; // channel set to -1 indicated that refresh interval completed so reset the timer
  else {
    if ( SERVO_INDEX(timer, Channel[timer]) < ServoCount && SERVO(timer, Channel[timer]).Pin.isActive == true )
      digitalWrite( SERVO(timer, Channel[timer]).Pin.nbr, LOW); // pulse this channel low if activated
  }

  Channel[timer]++;    // increment to the next channel
  if ( SERVO_INDEX(timer, Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    *OCRnA = *TCNTn + SERVO(timer, Channel[timer]).ticks;
    if (SERVO(timer, Channel[timer]).Pin.isActive == true)   // check if activated
      digitalWrite( SERVO(timer, Channel[timer]).Pin.nbr, HIGH); // its an active channel so pulse it high
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    if ( ((unsigned)*TCNTn) + 4 < usToTicks(REFRESH_INTERVAL) ) // allow a few ticks to ensure the next OCR1A not missed
      *OCRnA = (unsigned int)usToTicks(REFRESH_INTERVAL);
    else
      *OCRnA = *TCNTn + 4;  // at least REFRESH_INTERVAL has elapsed
    Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
  }
}

#ifndef WIRING // Wiring pre-defines signal handlers so don't define any if compiling for the Wiring platform
// Interrupt handlers for Arduino
#if defined(_useTimer1)
ISR(TIMER1_COMPA_vect)
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif

#if defined(_useTimer3)
ISR(TIMER3_COMPA_vect)
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif

#if defined(_useTimer4)
ISR(TIMER4_COMPA_vect)
{
  handle_interrupts(_timer4, &TCNT4, &OCR4A);
}
#endif

#if defined(_useTimer5)
ISR(TIMER5_COMPA_vect)
{
  handle_interrupts(_timer5, &TCNT5, &OCR5A);
}
#endif

#elif defined WIRING
// Interrupt handlers for Wiring
#if defined(_useTimer1)
void Timer1Service()
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif
#if defined(_useTimer3)
void Timer3Service()
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif
#endif

static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer1)
  if (timer == _timer1) {
    TCCR1A = 0;             // normal counting mode
    TCCR1B = _BV(CS11);     // set prescaler of 8
    TCNT1 = 0;              // clear the timer count
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF1A);      // clear any pending interrupts;
    TIMSK |=  _BV(OCIE1A) ;  // enable the output compare interrupt
#else
    // here if not ATmega8 or ATmega128
    TIFR1 |= _BV(OCF1A);     // clear any pending interrupts;
    TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER1OUTCOMPAREA_INT, Timer1Service);
#endif
  }
#endif

#if defined (_useTimer3)
  if (timer == _timer3) {
    TCCR3A = 0;             // normal counting mode
    TCCR3B = _BV(CS31);     // set prescaler of 8
    TCNT3 = 0;              // clear the timer count
#if defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF3A);     // clear any pending interrupts;
    ETIMSK |= _BV(OCIE3A);  // enable the output compare interrupt
#else
    TIFR3 = _BV(OCF3A);     // clear any pending interrupts;
    TIMSK3 =  _BV(OCIE3A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER3OUTCOMPAREA_INT, Timer3Service);  // for Wiring platform only
#endif
  }
#endif

#if defined (_useTimer4)
  if (timer == _timer4) {
    TCCR4A = 0;             // normal counting mode
    TCCR4B = _BV(CS41);     // set prescaler of 8
    TCNT4 = 0;              // clear the timer count
    TIFR4 = _BV(OCF4A);     // clear any pending interrupts;
    TIMSK4 =  _BV(OCIE4A) ; // enable the output compare interrupt
  }
#endif

#if defined (_useTimer5)
  if (timer == _timer5) {
    TCCR5A = 0;             // normal counting mode
    TCCR5B = _BV(CS51);     // set prescaler of 8
    TCNT5 = 0;              // clear the timer count
    TIFR5 = _BV(OCF5A);     // clear any pending interrupts;
    TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt
  }
#endif
}

static void finISR(timer16_Sequence_t timer)
{
  //disable use of the given timer
#if defined WIRING   // Wiring
  if (timer == _timer1) {
#if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK1 &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
#else
    TIMSK &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
#endif
    timerDetach(TIMER1OUTCOMPAREA_INT);
  }
  else if (timer == _timer3) {
#if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK3 &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
#else
    ETIMSK &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
#endif
    timerDetach(TIMER3OUTCOMPAREA_INT);
  }
#else
  //For arduino - in future: call here to a currently undefined function to reset the timer
#endif
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for (uint8_t channel = 0; channel < SERVOS_PER_TIMER; channel++) {
    if (SERVO(timer, channel).Pin.isActive == true)
      return true;
  }
  return false;
}

#endif


#if defined (ESP32)
static uint8_t servoNum = 0;
#define SERVO_MIN() (MIN_PULSE_WIDTH - (this->min*1.22))  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - (this->max*1.22))  // maximum value in uS for this servo 
#endif


#if defined(LE501X)

// convenience macros
#define SERVO_MIN() (MIN_PULSE_WIDTH - (this->min * 1.0))  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - (this->max * 1.0))  // maximum value in uS for this servo 

#define usToTicks(_us)    (_us)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#define ticksToUs(_ticks) (_ticks) // converts from ticks back to microseconds

uint8_t ServoCount = 0;                                     // the total number of attached servos

static servo_t servos[MAX_SERVOS];             // static array of servo structures

static boolean isTimerActive(void)
{
  // returns true if any servo is active on this timer
  for (uint8_t channel = 0; channel < MAX_SERVOS; channel++) 
  {
    if (servos[channel].Pin.isActive == true)
      return true;
  }
  return false;
}

#if defined(_useSoftTimer)


static SW_TIM_HandleTypeDef softTimHandle = {.init{.period = -1, .timer = -1,}, .channel = SOFTTIMER_CHANNEL_SERVO, .period = 0, .periodCalc = 0, .number = -1};

static servo_swtimer_t servo_swtimer[MAX_SERVOS];

// #define DEBUGIO  PA13
#define SOFT_PWM_PERIOD_US  (softTimHandle.period)

static inline void handle_interrupts(void)
{
#ifdef DEBUGIO  
  fastSetPin(DEBUGIO);  
#endif

  for(uint8_t i = 0;i < MAX_SERVOS;i++)
  {
    if(servos[i].Pin.isActive)
    {
      servo_swtimer[i].ticks += usToTicks(SOFT_PWM_PERIOD_US);
      if(!servo_swtimer[i].waitout)
      {
        if(servo_swtimer[i].ticks >= servos[i].ticks)
        {
          fastClrPin(servos[i].Pin.nbr); // its an active channel so pulse it low
          servo_swtimer[i].waitout = 1;
        }
      }
      else if(servo_swtimer[i].ticks >= usToTicks(REFRESH_INTERVAL) )
      {
          fastSetPin(servos[i].Pin.nbr); // its an active channel so pulse it high

          servo_swtimer[i].waitout = 0;
          servo_swtimer[i].ticks = 0;
      }
    }
  }

#ifdef DEBUGIO  
  fastClrPin(DEBUGIO);
#endif
}


static void initISR(void)
{
  softTimHandle.init.period = -1;      
  softTimHandle.init.timer = -1;      

  softTimHandle.channel = SOFTTIMER_CHANNEL_SERVO;

  if(softTimHandle.init.period <= 0)
  {
    softTimHandle.period = DEFAULT_SWTIM_PERIOD;
  }
  else
  {
    softTimHandle.period = softTimHandle.init.period;
  }

  softTimHandle.periodCalc = 0;
  softTimHandle.number = -1;

#ifdef DEBUGIO  
  pinMode(DEBUGIO, OUTPUT);
#endif

  softTimerAttachInterrupt(&softTimHandle, handle_interrupts); 
}

static void finISR(void)
{
  //disable use of the given timer
  softTimerDetachInterrupt(&softTimHandle);
}

#elif defined(_useHardTimer)


#endif

#endif




/****************** end of static functions ******************************/

Servo::Servo()
{
#if defined (ESP32)
  if (servoNum < MAX_SERVOS) {
    this->servoChannel = servoNum + 8;
    this->servoIndex = servoNum;
    servoNum++;
  }
  else {
    this->servoChannel = INVALID_SERVO ;  // too many servos
    this->servoIndex = INVALID_SERVO;
  }
#endif

#if defined(__AVR__)
  if ( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos
#endif

#if defined(LE501X)
#if defined(_useSoftTimer)
  if ( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos
#elif defined(_useHardTimer)
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount;
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
    ServoCount++;
  }
  else {
    this->servoIndex = INVALID_SERVO;
  }
#endif
#endif
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
#if defined (ESP32)
  if ( this->servoIndex < MAX_SERVOS) {
    this->servoPin = pin;
    this->min  = (MIN_PULSE_WIDTH - min)/1.22; //resolution of min/max is 1.22 uS
    this->max  = (MAX_PULSE_WIDTH - max)/1.22;
    ledcSetup(this->servoChannel, LEDC_SERVO_FREQ, LEDC_SERVO_BIT);
    ledcAttachPin(this->servoPin, this->servoChannel);
    this->attachFlag = true;
  }
  return this->servoIndex;
#endif

#if defined(_useSoftTimer)    
  if (this->servoIndex < MAX_SERVOS ) {
    servos[this->servoIndex].Pin.nbr = pin;
    this->min  = (MIN_PULSE_WIDTH - min) / 1.0;
    this->max  = (MAX_PULSE_WIDTH - max) / 1.0;

    pinMode( pin, OUTPUT) ;                                   // set servo pin to output
    // initialize the timer if it has not already been initialized
    if (isTimerActive() == false)
    {
      initISR();
    }
    servo_swtimer[this->servoIndex].waitout = 0;
    servo_swtimer[this->servoIndex].ticks = 0;

    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex ;
#elif defined(_useHardTimer)
  if (this->servoIndex < MAX_SERVOS ) {
    servos[this->servoIndex].Pin.nbr = pin;
    this->min  = (MIN_PULSE_WIDTH - min) / 1.0;
    this->max  = (MAX_PULSE_WIDTH - max) / 1.0;

    // initialize the timer if it has not already been initialized
    if (isTimerActive() == false)
    {
      pwmInit(HWTIMER_FOR_SERVO, REFRESH_INTERVAL);
    }
    pwmAttachPin(pin, this->servoIndex);

    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex ;
#endif

#if defined(__AVR__)
  if (this->servoIndex < MAX_SERVOS ) {
    pinMode( pin, OUTPUT) ;                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min) / 4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max) / 4;
    // initialize the timer if it has not already been initialized
    timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false)
      initISR(timer);
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex ;
#endif
}

void Servo::detach()
{
#if defined (ESP32)
   this->attachFlag = false;
   ledcDetachPin(this->servoPin);
#endif

#if defined(_useSoftTimer)
  servos[this->servoIndex].Pin.isActive = false;

  servo_swtimer[this->servoIndex].waitout = 0;
  servo_swtimer[this->servoIndex].ticks = 0;
  if (isTimerActive() == false) 
  {
    finISR();
  }
#elif defined(_useHardTimer)
  servos[this->servoIndex].Pin.isActive = false;

  pwmDetachPin(this->servoIndex);
  if (isTimerActive() == false)
  {
    pwmDeinit();
  }
#endif

#if defined(__AVR__)
  servos[this->servoIndex].Pin.isActive = false;
  timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if (isTimerActive(timer) == false) {
    finISR(timer);
  }
#endif

}

void Servo::write(int value)
{
  if (value < MIN_PULSE_WIDTH)
  { // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if (value < 0) value = 0;
    if (value > 180) value = 180;
    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
  }
  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int _value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if ( (channel < MAX_SERVOS) )  // ensure channel is valid
  {
    if ( _value < SERVO_MIN() )         // ensure pulse width is valid
      _value = SERVO_MIN();
    else if ( _value > SERVO_MAX() )
      _value = SERVO_MAX();
      
#if defined (ESP32)
    ledcWrite(this->servoChannel, int(_value/1.22));
    this->lastValue = _value;
#endif

#if defined(_useSoftTimer)
    servos[channel].ticks = _value;
#elif defined(_useHardTimer)
    pwmUpdata(this->servoIndex, (uint32_t)_value);
    servos[channel].ticks = _value;
#endif

#if defined(__AVR__)
    _value = _value - TRIM_DURATION;
    _value = usToTicks(_value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009

    uint8_t oldSREG = SREG;
    cli();
    servos[channel].ticks = _value;
    SREG = oldSREG;
#endif

  }
}

int Servo::read() // return the value as degrees
{
#if defined (ESP32) || defined (LE501X)
  return  map( this->readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
#endif

#if defined(__AVR__)
  return  map( this->readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
#endif
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
#if defined (ESP32)
  if ( this->servoIndex != INVALID_SERVO )
    pulsewidth  = this->lastValue;
  else
    pulsewidth  = 0;
#endif

#if defined(_useSoftTimer) || defined(_useHardTimer)
  if ( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks) ;
  else
    pulsewidth  = 0;
#endif

#if defined(__AVR__)
  if ( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else
    pulsewidth  = 0;
#endif
  return pulsewidth;
}

bool Servo::attached()
{
#if defined (ESP32)
  return this->attachFlag;
#endif

#if defined(_useSoftTimer) || defined(_useHardTimer)
  return servos[this->servoIndex].Pin.isActive ;
#endif

#if defined(__AVR__)
  return servos[this->servoIndex].Pin.isActive ;
#endif
}
