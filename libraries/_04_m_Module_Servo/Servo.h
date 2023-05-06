/*
  Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
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
  The sequence used to sieze timers is defined in timers.h

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
   //修改记录：兼容LE501X, 2023-05-06，by PKJ 
*/

#ifndef Servo_h
#define Servo_h

#if defined(__AVR__)
#include <inttypes.h>
#endif
/*
   Defines for 16 bit timers used with  Servo library

   If _useTimerX is defined then TimerX is a 16 bit timer on the curent board
   timer16_Sequence_t enumerates the sequence that the timers should be allocated
   _Nbr_16timers indicates how many 16 bit timers are available.

*/

#if defined(__AVR__)
// Say which 16 bit timers can be used and in what order
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define _useTimer5
#define _useTimer1
#define _useTimer3
#define _useTimer4
typedef enum { _timer5, _timer1, _timer3, _timer4, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_ATmega32U4__)
#define _useTimer1
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define _useTimer3
#define _useTimer1
typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
#define _useTimer3
#define _useTimer1
typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t ;

#else  // everything else
#define _useTimer1
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;
#endif

#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer 
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

typedef struct  {
  uint8_t nbr        : 6 ;            // a pin number from 0 to 63
  uint8_t isActive   : 1 ;            // true if this channel is enabled, pin not pulsed if false
} ServoPin_t   ;

typedef struct {
  ServoPin_t Pin;
  unsigned int ticks;
} servo_t;
#endif


#if defined(LE501X)
#include "le501x-hal-timer.h"

#define _useSoftTimer
// #define _useHardTimer


typedef struct  {
  uint8_t nbr        : 6 ;            // a pin number from 0 to 63
  uint8_t isActive   : 1 ;            // true if this channel is enabled, pin not pulsed if false
} ServoPin_t   ;

typedef struct {
  ServoPin_t Pin;
  unsigned int ticks;
} servo_t;

#if defined(_useSoftTimer)
#define MAX_SERVOS   (6)

typedef struct {
  bool waitout;
  unsigned int ticks;
} servo_swtimer_t;

#elif defined(_useHardTimer)
#define HWTIMER_FOR_SERVO   TIMER_0

#if (HWTIMER_FOR_SERVO == TIMER_0)
#define MAX_SERVOS   (4)
#elif (HWTIMER_FOR_SERVO == TIMER_1)
#define MAX_SERVOS   (4)
#elif (HWTIMER_FOR_SERVO == TIMER_2)
#define MAX_SERVOS   (2)
#elif (HWTIMER_FOR_SERVO == TIMER_3)
#define MAX_SERVOS   (4)
#endif

#endif  /* _useHardTimer */

#endif /* LE501X */


#if defined (ESP32)
#define LEDC_SERVO_BIT     14
#define LEDC_SERVO_FREQ    50
//#define MIN_PULSE_WIDTH    445     //  
//#define MAX_PULSE_WIDTH    1966     //
#define MAX_SERVOS         8
#endif

#define Servo_VERSION           2      // software version of this library
#define MIN_PULSE_WIDTH         544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH         2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH     1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL        20000     // minumim time to refresh servos in microseconds 
#define INVALID_SERVO           255     // flag indicating an invalid servo index

class Servo
{
  public:
    Servo();
    uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
    uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes.
    void detach();
    void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds
    void writeMicroseconds(int value); // Write pulse width in microseconds
    int read();                        // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached();                   // return true if this servo is attached, otherwise false
  private:
    uint8_t servoIndex;               // index into the channel data for this servo
    int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
    int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH
#if defined (ESP32)
    uint8_t servoChannel;
    int lastValue;
    bool attachFlag;
    uint8_t servoPin;
#endif
};

#endif
