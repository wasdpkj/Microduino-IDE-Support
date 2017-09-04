#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        14
#define NUM_ANALOG_INPUTS       14

#define analogInputToDigitalPin(p)  (((p)<20)?(esp32_adc2gpio[(p)]):-1)
#define digitalPinToInterrupt(p)    (((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         (p < 34)

static const uint8_t LED_BUILTIN = -1;//25;
#define BUILTIN_LED  LED_BUILTIN // backward compatibility

#define MTDO 15
#define MTDI 12
#define MTMS 14
#define MTCK 13

#define PIN_KEY 0
#define PIN_IR 15
#define PIN_BAT 36

static const uint8_t TX = 1;
static const uint8_t RX = 3;

static const uint8_t SDA = 22;//23;
static const uint8_t SCL = 21;//19;

static const uint8_t SDA1 = 33;
static const uint8_t SCL1 = 32;

static const uint8_t SS    = 5;
static const uint8_t MOSI  = 23;//16;
static const uint8_t MISO  = 19;//17;
static const uint8_t SCK   = 18;

static const uint8_t A0 = 35;//36;
static const uint8_t A1 = 34;//39;
static const uint8_t A2 = 39;//35;
static const uint8_t A3 = 38;//25;
static const uint8_t A6 = 37;//12;
static const uint8_t A7 = 36;//15;

static const uint8_t A8 = 4;
static const uint8_t A9 = 25;
static const uint8_t A10 = 26;
static const uint8_t A11 = 27;
static const uint8_t A12 = 12;
static const uint8_t A13 = 13;

static const uint8_t D0 = 3;//19;	
static const uint8_t D1 = 1;//23;
static const uint8_t D2 = 16;//18;
static const uint8_t D3 = 17;
static const uint8_t D4 = 4;//16;	ADC2_CH0
static const uint8_t D5 = 25;//5;	ADC2_CH8
static const uint8_t D6 = 26;//4;	ADC2_CH9
static const uint8_t D7 = 27;//		ADC2_CH7
static const uint8_t D8 = 12;//		ADC2_CH5
static const uint8_t D9 = 13;//		ADC2_CH4
static const uint8_t D10 = 5;
static const uint8_t D11 = 23;
static const uint8_t D12 = 19;
static const uint8_t D13 = 18;


static const uint8_t T0 = 19;
static const uint8_t T1 = 23;
static const uint8_t T2 = 18;
static const uint8_t T3 = 17;
static const uint8_t T4 = 16;
static const uint8_t T5 = 5;
static const uint8_t T6 = 4;

static const uint8_t DAC1 = 25;
static const uint8_t DAC2 = 26;

#endif /* Pins_Arduino_h */
