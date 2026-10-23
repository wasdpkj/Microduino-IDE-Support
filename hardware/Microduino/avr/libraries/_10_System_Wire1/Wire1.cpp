/*
  Wire1.cpp - Second TWI/I2C bus (TWI1) for ATmega328PB
  改动说明（2026-10）：
  - 与 miniCore BSP 对齐：Wire1 对象类型统一为 TwoWire，由 Wire.h 的
    注入式构造函数绑定 TWI1 驱动（utility/twi1.c）。
  - 旧类型名 TwoWire1 以 using 别名形式保留（见 Wire1.h）。
  - 仅保留对象实例化；TwoWire 的方法实现由 Wire 库提供。
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  #include "utility/twi1.h"
}

#include "Wire1.h"

// C thunks (not lambdas): safe even before Wire1 construction finishes wiring
static void wire1_onReceiveThunk(uint8_t* v, int len) {
  Wire1.onReceiveService(v, len);
}

static void wire1_onRequestThunk(void) {
  Wire1.onRequestService();
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire1 = TwoWire(TWI1_BUFFER_SIZE,
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
