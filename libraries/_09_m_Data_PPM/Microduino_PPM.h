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

#ifndef _MICRODUINO_PPM_H
#define _MICRODUINO_PPM_H

#include <Arduino.h>

#define PIN_SET(pin) (*portOutputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin))
#define PIN_CLR(pin) (*portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin))

#define MS_TO_TICKS(_ms)  ((_ms) * 2)  // todo, use macro here

#define NBR_OF_CHANNELS  8
#define FIX_CHANNEL_PULSE -10	//fix
#define MIN_CHANNEL_PULSE 1000  // 1ms
#define MID_CHANNEL_PULSE 1500  // 1.5ms
#define MAX_CHANNEL_PULSE 2000  // 2 ms
#define INTER_CHAN_DELAY  0   // 200 microseconds
#define INTER_CHAN_DELAY_TICKS (uint16_t)(MS_TO_TICKS(INTER_CHAN_DELAY));
#define FRAME_RATE(X)	  (4000 + (X) * MAX_CHANNEL_PULSE) // 20 ms
#define FRAME_RATE_TICKS(X)  (uint16_t)(MS_TO_TICKS(FRAME_RATE(X)))
#define SYNC_PULSE_WIDTH(X) (FRAME_RATE(X) - (X) * (MID_CHANNEL_PULSE + INTER_CHAN_DELAY))

class PPM
{
	public:
		PPM();
		
        void PPMinterrupt();
		void beginRead(byte pin);
		void Read(uint16_t* _data);
		uint8_t getChannel();

		void beginWrite(byte pin,byte num = NBR_OF_CHANNELS);
		void setFix(int8_t fix);
		void Write(byte Channel, int Value);
	private:
		int8_t fixVal = FIX_CHANNEL_PULSE;
		int realRaw[NBR_OF_CHANNELS];
		uint8_t currentChannel;
		uint32_t lastms,diffms;
		bool sync;
		void ChannelStorePulseWidth(byte Channel, int Value);
};

#endif
