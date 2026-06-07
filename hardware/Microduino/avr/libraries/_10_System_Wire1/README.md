# Wire1（TWI1，328PB）

第二路 I2C，API 与 `Wire` 相同，全局对象 **`Wire1`**。

## 引脚

| 信号 | Arduino | MCU |
|------|---------|-----|
| SDA1 | **D23** | PE0 |
| SCL1 | **D24** | PE1 |

## 依赖

- 必须先使用升级后的 `_10_System_Wire`（MiniCore 版 `TwoWire`）。
- 仅 **ATmega328PB**（及含 TWI1 的 AVR）有效；328P 工程请勿 `#include <Wire1.h>`。

## 示例

```cpp
#include <Wire.h>
#include <Wire1.h>

void setup() {
  Wire.begin();
  Wire1.begin();
}
```

## 来源

- 上游：[MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) `avr/libraries/Wire1/`
- 底层：`utility/twi1.c`（TWI1 寄存器 + `SDA1`/`SCL1` 引脚）
