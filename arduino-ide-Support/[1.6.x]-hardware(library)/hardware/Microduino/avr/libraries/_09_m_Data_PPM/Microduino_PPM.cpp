// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// Microduino IDE Support：
// https://github.com/wasdpkj/Microduino-IDE-Support/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

// ==============
// E-mail:
// Kejia Pan
// pankejia@microduino.cc

// ==============
// Weibo:
// @老潘orz

#include "Microduino_PPM.h"

uint8_t numChannel_R = NBR_OF_CHANNELS;

// only one instance allowed - some hack here
static PPM *pclass = 0;
void ppmInterrupt() {// dummy function
  if (pclass)
    pclass->PPMinterrupt();
}

PPM::PPM() {
} //PPMint

/*
PPM Read
*/
void PPM::beginRead(byte pin) {
  realRaw[NBR_OF_CHANNELS];
  currentChannel = 0;
  lastms = 0;
  sync = false;

  pinMode(pin, INPUT);
  pclass = this; //create an pointer to func
  attachInterrupt(digitalPinToInterrupt(pin), ppmInterrupt, RISING);
  while (!sync) {
    delay(100);
  }
}


void PPM::PPMinterrupt() { //here real work to come ;)
  uint32_t nowms = micros();
  diffms = nowms - lastms;
  if (lastms > 0) {
    if (diffms > 3900) {
      sync = true;
	  numChannel_R = currentChannel;
      currentChannel = 0;
    }
    else {
      if (sync) {
        if (diffms <= 2000 && diffms >= 1000) {
          realRaw[currentChannel] = diffms;
        }
        currentChannel++;
      }
    }
  }
  lastms = nowms;
}

void PPM::Read(uint16_t* _data) {
  for (uint8_t a = 0; a < getChannel(); a++) {
    _data[a] = realRaw[a];
  }
}

uint8_t PPM::getChannel(){
  return numChannel_R;	
}

/*
PPM Write
*/
uint8_t numChannel_W = NBR_OF_CHANNELS;

volatile  byte Channel = 0;  // the channel being pulsed
static byte OutputPin;   // the digital pin to use

enum pulseStates {stateDISABLED, stateHIGH, stateLOW};
static byte pulseState = stateDISABLED;

typedef struct {
  uint16_t ticks;  // we use 16 bit timer here so just store value to be compared as int
} Channel_t;

Channel_t Channels[NBR_OF_CHANNELS + 1];  // last entry is for sync pulse delay
uint32_t RunningPulseWidth = 0;

ISR(TIMER1_COMPA_vect) {
  if ( pulseState == stateLOW ) {
    PIN_CLR(OutputPin);	//change to HIGH for invert was LOW
    OCR1A = Channels[Channel].ticks;

    pulseState = stateHIGH;
  }

  else if (pulseState == stateHIGH)
  {
    OCR1A = MS_TO_TICKS(INTER_CHAN_DELAY);
    if ( Channel < numChannel_W)
    {
      RunningPulseWidth += Channels[Channel].ticks + INTER_CHAN_DELAY_TICKS;
	  PIN_SET(OutputPin);	//change to LOW for invert was HIGH
    }
    pulseState = stateLOW;

    Channel++;
    if (Channel == numChannel_W)
    {
      Channels[numChannel_W].ticks = (uint16_t)(FRAME_RATE_TICKS(numChannel_W) - RunningPulseWidth);
      RunningPulseWidth = 0;
    }

    if (Channel > numChannel_W) { // note that numChannel_W+1 is the sync pulse
      Channel = 0;
	  PIN_SET(OutputPin);	//change to LOW for invert was HIGH
    }
  }
}

// private functions
// -----------------------------------------------------------------------------
// convert microseconds to timer cycles + ticks, and store in the Channels array
void PPM::ChannelStorePulseWidth(byte Channel, int Value) {
  cli();
  Channels[Channel].ticks = MS_TO_TICKS(Value) ;
  sei();     // enable interrupts
/*
  Serial.print(Channel, DEC);
  Serial.print("=\t");
  Serial.print(Channels[Channel].ticks, DEC);
  Serial.print(" RunningPulseWidth= ");
  Serial.print(RunningPulseWidth);
  Serial.print("\r\n");
*/
}

void PPM::setFix(int8_t fix){
	fixVal = fix;
}

// user api
// -----------------------------------------------------------------------------
// turns on a Channels pulse of the specified length, on the specified pin
void PPM::Write(byte Channel, int Value) {
  /*
      if ( Value > MAX_CHANNEL_PULSE ) {
       Value = MAX_CHANNEL_PULSE;
      }
      else if ( Value < MIN_CHANNEL_PULSE ) {
       Value = MIN_CHANNEL_PULSE;
      }
  */
  ChannelStorePulseWidth(Channel, Value + fixVal);
}

// start the encoder with output on the given pin
void PPM::beginWrite(byte pin,byte num) {
  numChannel_W = num;
  fixVal = FIX_CHANNEL_PULSE;
  Channel = 0;
  pulseState = stateLOW; // flag ISR we are ready to pulse the channels

  OutputPin = pin;
  pinMode(OutputPin, OUTPUT);

  // initialize pulse width data in Channel array.
  for (uint8_t i = 0; i < numChannel_W; ++i){
    ChannelStorePulseWidth(i, MID_CHANNEL_PULSE + fixVal);
  }

  ChannelStorePulseWidth(numChannel_W, SYNC_PULSE_WIDTH(numChannel_W));  // store sync pulse width

  TIMSK1 |= (1 << OCIE1A); //Enable compare interrupt
  TCCR1A = _BV(WGM10) | _BV(WGM11);   //Timer 1 is Phase-correct 10-bit PWM.
  TCCR1A |= _BV(COM1A1);      //Clear OC1A on compare match when up-counting, set OC1A on compare match when down-counting.

  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11); /* div 8 clock prescaler */
}
