/*
|| @author         Brett Hagman <bhagman@wiring.org.co>
|| @url            http://wiring.org.co/
|| @url            http://roguerobotics.com/
||
|| @description
|| | A Software PWM Library
|| |
|| | Written by Brett Hagman
|| | http://www.roguerobotics.com/
|| | bhagman@roguerobotics.com, bhagman@wiring.org.co
|| |
|| | A Wiring (and Arduino) Library, for Atmel AVR8 bit series microcontrollers,
|| | to produce PWM signals on any arbitrary pin.
|| |
|| | It was originally designed for controlling the brightness of LEDs, but
|| | could be adapted to control servos and other low frequency PWM controlled
|| | devices as well.
|| |
|| | It uses a single hardware timer (Timer 2) on the Atmel microcontroller to
|| | generate up to 20 PWM channels (your mileage may vary).
|| |
|| #
||
|| @license Please see the accompanying LICENSE.txt file for this project.
||
|| @name Software PWM Library
|| @type Library
|| @target Atmel AVR 8 Bit, Linkedsemi LE501X
||
|| @version 1.0.2
||
*/

#ifndef SOFTPWM_H
#define SOFTPWM_H

#define SOFTPWM_VERSION 10000

#include <stdint.h>

#define SOFTPWM_MAXCHANNELS 16
#define SOFTPWM_PWMDEFAULT 0x00

#define SOFTPWM_NORMAL 0
#define SOFTPWM_INVERTED 1

#define ALL -1

#if defined (__AVR__)
#define PIN_SET(pin) (*portOutputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin))
#define PIN_CLR(pin) (*portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin))
#elif defined (LE501X)
#define PIN_SET(pin) (fastSetPin(pin))
#define PIN_CLR(pin) (fastClrPin(pin))
#elif defined (ESP32)
#define PIN_SET(pin) (digitalWrite(pin, HIGH))
#define PIN_CLR(pin) (digitalWrite(pin, LOW))
#endif

void SoftPWMBegin(uint8_t defaultPolarity = SOFTPWM_NORMAL);
void SoftPWMSet(int8_t pin, uint8_t value, uint8_t hardset = 0);
void SoftPWMSetPercent(int8_t pin, uint8_t percent, uint8_t hardset = 0);
void SoftPWMEnd(int8_t pin);
void SoftPWMSetFadeTime(int8_t pin, uint16_t fadeUpTime, uint16_t fadeDownTime);
void SoftPWMSetPolarity(int8_t pin, uint8_t polarity);

void SoftPWMInit(uint8_t defaultPolarity = SOFTPWM_NORMAL);
void SoftPWMDeinit(void);

#endif

