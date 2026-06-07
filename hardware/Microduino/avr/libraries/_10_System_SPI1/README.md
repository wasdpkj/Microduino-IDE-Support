# SPI1（328PB 第二路 SPI）

全局对象 **`SPI1`**，API 与 `_10_System_SPI` 的 `SPI` 类似（`beginTransaction` / `transfer` 等）。

## 引脚（MiniCore / 328pb variant）

| 信号 | Arduino | MCU | 备注 |
|------|---------|-----|------|
| SS1 | D25 | PE2 | 与 **A6** 同脚 |
| MOSI1 | D26 | PE3 | 与 **A7** 同脚 |
| MISO1 | D14 | PC0 | A0 |
| SCK1 | D15 | PC1 | A1 |

使用 SPI1 时 D25/D26 作片选/ MOSI，不能同时当模拟输入 A6/A7。

## 示例

```cpp
#include <SPI1.h>

void setup() {
  SPI1.begin();
  SPI1.beginTransaction(SPI1Settings(4000000, MSBFIRST, SPI_MODE0));
  SPI1.transfer(0x00);
  SPI1.endTransaction();
}
```

## 来源

- 上游：MiniCore `avr/libraries/SPI1/`
- 仅 **含 SPI1 硬件** 的 MCU（如 ATmega328PB）可用
