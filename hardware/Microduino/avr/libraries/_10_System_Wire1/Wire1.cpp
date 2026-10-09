/*
  Wire1.cpp - Second TWI/I2C bus (TWI1) for ATmega328PB
  Self-contained: does not depend on the platform/IDE Wire library.
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  #include "utility/twi1.h"
}

#include "Wire1.h"

// Constructors ////////////////////////////////////////////////////////////////

TwoWire1::TwoWire1(int bufferLength,
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
                 void (*tw_attachSlaveTxEvent)( void (*onTransmit)(void) ),
                 void (*onTransmit)(void))
{
  this->bufferLength = bufferLength;
  rxBuffer = new uint8_t[bufferLength];
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txAddress = 0;
  txBuffer = new uint8_t[bufferLength];
  txBufferIndex = 0;
  txBufferLength = 0;

  transmitting = 0;
  user_onRequest = 0;
  user_onReceive = 0;
  this->tw_init = tw_init;
  this->tw_disable = tw_disable;
  this->tw_setAddress = tw_setAddress;
  this->tw_setFrequency = tw_setFrequency;
  this->tw_readFrom = tw_readFrom;
  this->tw_writeTo = tw_writeTo;
  this->tw_transmit = tw_transmit;
  this->tw_reply = tw_reply;
  this->tw_stop = tw_stop;
  this->tw_releaseBus = tw_releaseBus;
  #if defined(WIRE_TIMEOUT)
    this->tw_setTimeoutInMicros = tw_setTimeoutInMicros;
    this->tw_handleTimeout = tw_handleTimeout;
    this->tw_manageTimeoutFlag = tw_manageTimeoutFlag;
  #endif
  tw_attachSlaveRxEvent(onReceive);
  tw_attachSlaveTxEvent(onTransmit);
}

TwoWire1::~TwoWire1()
{
  delete[] rxBuffer;
  delete[] txBuffer;
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire1::begin(void)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  tw_init();
}

void TwoWire1::begin(uint8_t address)
{
  begin();
  tw_setAddress(address);
}

void TwoWire1::begin(int address)
{
  begin((uint8_t)address);
}

void TwoWire1::end(void)
{
  tw_disable();
}

void TwoWire1::setClock(uint32_t clock)
{
  tw_setFrequency(clock);
}

#if defined(WIRE_TIMEOUT)
void TwoWire1::setWireTimeout(uint32_t timeout, bool reset_with_timeout){
  tw_setTimeoutInMicros(timeout, reset_with_timeout);
}

bool TwoWire1::getWireTimeoutFlag(void){
  return(tw_manageTimeoutFlag(false));
}

void TwoWire1::clearWireTimeoutFlag(void){
  tw_manageTimeoutFlag(true);
}
#endif

uint8_t TwoWire1::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  if (isize > 0) {
  beginTransmission(address);

  if (isize > 3){
    isize = 3;
  }

  while (isize-- > 0)
    write((uint8_t)(iaddress >> (isize*8)));
  endTransmission(false);
  }

  if(quantity > bufferLength){
    quantity = bufferLength;
  }
  uint8_t read = tw_readFrom(address, rxBuffer, quantity, sendStop);
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

uint8_t TwoWire1::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire1::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire1::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire1::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoWire1::beginTransmission(uint8_t address)
{
  transmitting = 1;
  txAddress = address;
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoWire1::beginTransmission(int address)
{
  beginTransmission((uint8_t)address);
}

uint8_t TwoWire1::endTransmission(uint8_t sendStop)
{
  uint8_t ret = tw_writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
  txBufferIndex = 0;
  txBufferLength = 0;
  transmitting = 0;
  return ret;
}

uint8_t TwoWire1::endTransmission(void)
{
  return endTransmission(true);
}

size_t TwoWire1::write(uint8_t data)
{
  if(transmitting){
    if(txBufferLength >= bufferLength){
      setWriteError();
      return 0;
    }
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    txBufferLength = txBufferIndex;
  }else{
    tw_transmit(&data, 1);
  }
  return 1;
}

size_t TwoWire1::write(const uint8_t *data, size_t quantity)
{
  if(transmitting){
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  }else{
    tw_transmit(data, quantity);
  }
  return quantity;
}

int TwoWire1::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

int TwoWire1::read(void)
{
  int value = -1;

  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

int TwoWire1::peek(void)
{
  int value = -1;

  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

void TwoWire1::flush(void)
{
}

void TwoWire1::onReceiveService(uint8_t* inBytes, int numBytes)
{
  if(!user_onReceive){
    return;
  }
  if(rxBufferIndex < rxBufferLength){
    return;
  }
  for(uint8_t i = 0; i < numBytes; ++i){
    rxBuffer[i] = inBytes[i];
  }
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  user_onReceive(numBytes);
}

void TwoWire1::onRequestService(void)
{
  if(!user_onRequest){
    return;
  }
  txBufferIndex = 0;
  txBufferLength = 0;
  user_onRequest();
}

void TwoWire1::onReceive( void (*function)(int) )
{
  user_onReceive = function;
}

void TwoWire1::onRequest( void (*function)(void) )
{
  user_onRequest = function;
}

// C thunks (not lambdas): safe even before Wire1 construction finishes wiring
static void wire1_onReceiveThunk(uint8_t* v, int len) {
  Wire1.onReceiveService(v, len);
}

static void wire1_onRequestThunk(void) {
  Wire1.onRequestService();
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire1 Wire1 = TwoWire1(TWI1_BUFFER_SIZE,
                        twi_init1,
                        twi_disable1,
                        twi_setAddress1,
                        twi_setFrequency1,
                        twi_readFrom1,
                        twi_writeTo1,
                        twi_transmit1,
                        twi_reply1,
                        twi_stop1,
                        twi_releaseBus1,
                        #if defined(WIRE_TIMEOUT)
                          twi_setTimeoutInMicros1,
                          twi_handleTimeout1,
                          twi_manageTimeoutFlag1,
                        #endif
                        twi_attachSlaveRxEvent1,
                        wire1_onReceiveThunk,
                        twi_attachSlaveTxEvent1,
                        wire1_onRequestThunk);
