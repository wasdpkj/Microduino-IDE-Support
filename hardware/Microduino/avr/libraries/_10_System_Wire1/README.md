# Wire1（TWI1，328PB）

第二路 I2C，API 与 `Wire` 相同，全局对象 **`Wire1`**（类型为 **`TwoWire1`**）。

本库**自包含**，不 `#include <Wire.h>`，因此在 Mixly 等环境优先选用 IDE 自带旧版 Wire 时仍可正常编译。

## 引脚

| 信号 | Arduino | MCU |
|------|---------|-----|
| SDA1 | **D23** | PE0 |
| SCL1 | **D24** | PE1 |

## 用法

```cpp
#include <Wire.h>    // TWI0：可用 Mixly/IDE Wire，或本硬件包 _10_System_Wire
#include <Wire1.h>   // TWI1：本库 TwoWire1 / Wire1

void setup() {
  Wire.begin();
  Wire1.begin();
}
```

## 兼容说明

| 环境 | Wire (TWI0) | Wire1 (TWI1) |
|------|-------------|--------------|
| Arduino IDE 2 + 本硬件包 | 优先用 `_10_System_Wire` | `_10_System_Wire1` |
| Mixly（自带 `libraries/Wire`） | 常用 Mixly 旧 Wire | 仍可用本库（不依赖旧 Wire） |

不要注释 `onReceive`/`onRequest` 回调相关代码；从机回调已保留。

## 来源

- API 对齐：[MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) Wire1
- 底层：`utility/twi1.c`（TWI1 + `SDA1`/`SCL1`）
