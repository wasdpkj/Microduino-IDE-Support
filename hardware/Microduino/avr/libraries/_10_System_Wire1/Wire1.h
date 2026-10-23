/*
Wire1.h - Second TWI/I2C bus (TWI1) for ATmega328PB
改动说明（2026-10）：
- 与 miniCore BSP 对齐：Wire1 对象类型统一为 TwoWire。
- 通过 `using TwoWire1 = TwoWire;` 保留旧类型名，避免破坏已使用
  TwoWire1 类型的用户代码（例如 `TwoWire1 *p = &Wire1;`）。
- 依赖 Wire.h 提供的 TwoWire 类定义（TWI1_vect 存在时，Wire.h 走
  注入式构造函数分支，方法签名与 TWI1 一致）。
*/

#ifndef TwoWire1_h
#define TwoWire1_h

#include <Wire.h>   // 提供 TwoWire 类定义

// —— 向后兼容 ——
// 旧代码中的 `TwoWire1` 类型名继续可用，指向同一个 TwoWire 类型
using TwoWire1 = TwoWire;

// TWI1 总线对象；类型与 miniCore BSP 保持一致
extern TwoWire Wire1;

// 保持旧宏，供外部代码探测“Wire1 支持 end()”
#ifndef WIRE1_HAS_END
#define WIRE1_HAS_END 1
#endif

#endif // TwoWire1_h
