/*
  Wire1.h - Second TWI/I2C bus (TWI1) for ATmega328PB
  Standalone TwoWire1 class: does not include <Wire.h>, so it works
  even when Mixly/Arduino IDE picks the stock Wire library first.
*/

#ifndef TwoWire1_h
#define TwoWire1_h

#include <Arduino.h>
#include <inttypes.h>
#include "Stream.h"

// Match Microduino Wire timeout enablement
#ifndef WIRE_TIMEOUT
#define WIRE_TIMEOUT
#endif

#define WIRE1_HAS_END 1

class TwoWire1 : public Stream
{
  private:
    int bufferLength;
    uint8_t* rxBuffer;
    uint8_t rxBufferIndex;
    uint8_t rxBufferLength;

    uint8_t txAddress;
    uint8_t* txBuffer;
    uint8_t txBufferIndex;
    uint8_t txBufferLength;

    uint8_t transmitting;
    void (*user_onRequest)(void);
    void (*user_onReceive)(int);
    void (*tw_init)(void);
    void (*tw_disable)(void);
    void (*tw_setAddress)(uint8_t);
    void (*tw_setFrequency)(uint32_t);
    uint8_t (*tw_readFrom)(uint8_t, uint8_t*, uint8_t, uint8_t);
    uint8_t (*tw_writeTo)(uint8_t, uint8_t*, uint8_t, uint8_t, uint8_t);
    uint8_t (*tw_transmit)(const uint8_t*, uint8_t);
    void (*tw_reply)(uint8_t);
    void (*tw_stop)(void);
    void (*tw_releaseBus)(void);
    #if defined(WIRE_TIMEOUT)
      void (*tw_setTimeoutInMicros)(uint32_t, bool);
      void (*tw_handleTimeout)(bool);
      bool (*tw_manageTimeoutFlag)(bool);
    #endif

  public:
    TwoWire1(int bufferLength,
            void (*tw_init)(void),
            void (*tw_disable)(void),
            void (*tw_setAddress)(uint8_t),
            void (*tw_setFrequency)(uint32_t),
            uint8_t (*tw_readFrom)(uint8_t, uint8_t*, uint8_t, uint8_t),
            uint8_t (*tw_writeTo)(uint8_t, uint8_t*, uint8_t, uint8_t, uint8_t),
            uint8_t (*tw_transmit)(const uint8_t*, uint8_t),
            void (*tw_reply)(uint8_t),
            void (*tw_stop)(void),
            void (*tw_releaseBus)(void),
            #if defined(WIRE_TIMEOUT)
              void (*tw_setTimeoutInMicros)(uint32_t, bool),
              void (*tw_handleTimeout)(bool),
              bool (*tw_manageTimeoutFlag)(bool),
            #endif
            void (*tw_attachSlaveRxEvent)( void (*onReceive)(uint8_t*, int) ),
            void (*onReceive)(uint8_t*, int),
            void (*tw_attachSlaveTxEvent)( void (*onTrasmit)(void) ),
            void (*onTrasmit)(void));
    ~TwoWire1();
    void begin();
    void begin(uint8_t);
    void begin(int);
    void end();
    void setClock(uint32_t);
    #if defined(WIRE_TIMEOUT)
      void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false);
      bool getWireTimeoutFlag(void);
      void clearWireTimeoutFlag(void);
    #endif
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );

    void onRequestService(void);
    void onReceiveService(uint8_t*, int);

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

extern TwoWire1 Wire1;

#endif
