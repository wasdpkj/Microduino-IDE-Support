# Wire1（TWI1，328PB）

第二路 I2C，API 与 `Wire` 相同，全局对象 **`Wire1`**（类型为 **`TwoWire`**，与 miniCore BSP 一致）。

自 1.2.0 起与 miniCore 对齐：`Wire1.h` 仅 `#include <Wire.h>` 并声明 `extern TwoWire Wire1;`，
旧类型名 `TwoWire1` 以 `using TwoWire1 = TwoWire;` 别名保留，现有代码无需改动。

## 引脚

| 信号 | Arduino | MCU |
|------|---------|-----|
| SDA1 | **D23** | PE0 |
| SCL1 | **D24** | PE1 |

## 用法

```cpp
#include <Wire.h>    // TWI0：本硬件包 _10_System_Wire 库（注入式 TwoWire）
#include <Wire1.h>   // TWI1：本库，Wire1 类型为 TwoWire

void setup() {
  Wire.begin();
  Wire1.begin();
}
```

第三方库（如 MAMA_QMI8658）只需依赖 `TwoWire`，即可同时兼容两个 BSP：

```cpp
QMI8658 imu(&Wire1);   // Microduino 与 miniCore 下均可用
```

## 兼容说明

| 环境 | Wire (TWI0) | Wire1 (TWI1) |
|------|-------------|--------------|
| Arduino IDE + 本硬件包 | `_10_System_Wire`（注入式 TwoWire） | `_10_System_Wire1`（TwoWire） |
| Mixly + 本硬件包 | 取决于 `<Wire.h>` 解析结果 | `_10_System_Wire1`（TwoWire） |
| miniCore BSP | miniCore `Wire` | miniCore `Wire1` |

## 来源

- API 对齐：[MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) Wire1
- 底层：`utility/twi1.c`（TWI1 + `SDA1`/`SCL1`）
