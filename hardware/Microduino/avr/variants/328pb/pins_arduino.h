/*
  pins_arduino.h - Pin definition functions for ATmega328PB
  Based on MiniCore pb-variant (MCUdude), adapted for Microduino AVR Boards.
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            (27)

#if defined(__AVR_ATmega328PB__)
#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1 || (p) == 2 || (p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

#if !defined(LED_BUILTIN)
  #define LED_BUILTIN (13)
#endif
static const uint8_t LED = LED_BUILTIN;

#define PIN_A0 (14)
#define PIN_A1 (15)
#define PIN_A2 (16)
#define PIN_A3 (17)
#define PIN_A4 (18)
#define PIN_A5 (19)
#define PIN_A6 (25)
#define PIN_A7 (26)
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
#define NUM_ANALOG_INPUTS           (8)
#define analogInputToDigitalPin(p)  (((p) < 6) ? ((p) + 14) : (((p) < 8) ? ((p) + 19) : -1))
#define analogPinToChannel(p)       ((p) < NUM_ANALOG_INPUTS ? (p) : ((p) >= 14 && (p) < 25) ? (p) - 14 : ((p) >= 25) ? (p) - 19 : -1)

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
#if defined(__AVR_ATmega328PB__)
  #define PIN_SPI_SS0   PIN_SPI_SS
  #define PIN_SPI_MOSI0 PIN_SPI_MOSI
  #define PIN_SPI_MISO0 PIN_SPI_MISO
  #define PIN_SPI_SCK0  PIN_SPI_SCK
  #define PIN_SPI_SS1   (25)
  #define PIN_SPI_MOSI1 (26)
  #define PIN_SPI_MISO1 (14)
  #define PIN_SPI_SCK1  (15)
  static const uint8_t SS0   = PIN_SPI_SS0;
  static const uint8_t MOSI0 = PIN_SPI_MOSI0;
  static const uint8_t MISO0 = PIN_SPI_MISO0;
  static const uint8_t SCK0  = PIN_SPI_SCK0;
  static const uint8_t SS1   = PIN_SPI_SS1;
  static const uint8_t MOSI1 = PIN_SPI_MOSI1;
  static const uint8_t MISO1 = PIN_SPI_MISO1;
  static const uint8_t SCK1  = PIN_SPI_SCK1;
#endif

#define PIN_WIRE_SDA  (18)
#define PIN_WIRE_SCL  (19)
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
#if defined(__AVR_ATmega328PB__)
  #define WIRE_INTERFACES_COUNT (2)
  #define PIN_WIRE_SDA0 PIN_WIRE_SDA
  #define PIN_WIRE_SCL0 PIN_WIRE_SCL
  #define PIN_WIRE_SDA1 (23)
  #define PIN_WIRE_SCL1 (24)
  static const uint8_t SDA0  = PIN_WIRE_SDA0;
  static const uint8_t SCL0  = PIN_WIRE_SCL0;
  static const uint8_t SDA1  = PIN_WIRE_SDA1;
  static const uint8_t SCL1  = PIN_WIRE_SCL1;
#endif

#define EXTERNAL_NUM_INTERRUPTS     (2)
#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#if defined(__AVR_ATmega328PB__)
#define digitalPinToPCICR(p)     (((p) >= 0 && (p) <= 26) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p)  (((p) <= 7) ? 2 : (((p) <= 13 || (p) == 20 || (p) == 21) ? 0 : (((p) <= 22) ? 1 : 3)))
#define digitalPinToPCMSK(p)     (((p) <= 7) ? (&PCMSK2) : (((p) <= 13 || (p) == 20 || (p) == 21) ? (&PCMSK0) : (((p) <= 22) ? (&PCMSK1) : (((p) <= 26) ? (&PCMSK3) : ((uint8_t *)0)))))
#define digitalPinToPCMSKbit(p)  (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) <= 21) ? ((p) - 14) : ((p) == 22) ? ((p) - 16) : ((p) - 23)))
#endif

#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, PD, PD, PD, PD, PD, PD, PD,
	PB, PB, PB, PB, PB, PB,
	PC, PC, PC, PC, PC, PC,
	PB, PB, PC, PE, PE, PE, PE,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5),
	_BV(6), _BV(7), _BV(6), _BV(0), _BV(1), _BV(2), _BV(3),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	TIMER3A, TIMER4A, TIMER4B,
	TIMER2B, NOT_ON_TIMER, TIMER0B, TIMER0A, NOT_ON_TIMER,
	NOT_ON_TIMER, TIMER1A, TIMER1B, TIMER2A, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
};

#endif // ARDUINO_MAIN

#if defined(__AVR_ATmega328PB__)
  #define SPCR SPCR0
  #define SPSR SPSR0
  #define SPDR SPDR0
  #define SPI_STC_vect SPI0_STC_vect
  #define SPI_STC_vect_num SPI0_STC_vect_num
  #define TWBR TWBR0
  #define TWSR TWSR0
  #define TWAR TWAR0
  #define TWDR TWDR0
  #define TWCR TWCR0
  #define TWAMR TWAMR0
  #define TWI_vect TWI0_vect
  #define TWI_vect_num TWI0_vect_num
  #define USART_RX_vect USART0_RX_vect
  #define USART_RX_vect_num USART0_RX_vect_num
  #define USART_UDRE_vect USART0_UDRE_vect
  #define USART_UDRE_vect_num USART0_UDRE_vect_num
  #define USART_TX_vect USART0_TX_vect
  #define USART_TX_vect_num USART0_TX_vect_num
#endif

#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial

#if defined(__AVR_ATmega328PB__)
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1
#endif

#endif // Pins_Arduino_h
