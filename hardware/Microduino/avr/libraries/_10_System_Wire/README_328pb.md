# Wire 库（328PB 相关说明）

本目录 Wire 库已升级为 [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) 同款（`libraries/Wire`）。

## 为何升级

328PB 存在 **TWI1** 硬件。MiniCore 在检测到 `TWI1_vect` 时，`TwoWire` 改为可注入底层驱动的实现，供 **Wire1** 库复用同一套 API。

- **328P / 644 等无 TWI1 的 MCU**：仍走旧版 `TwoWire` 静态缓冲区路径，与原先 API 兼容。
- **328PB**：走新版 `TwoWire`，需配合 `_10_System_Wire1` 使用第二路 I2C。

## 引脚（TWI0 / `Wire`）

| 信号 | Arduino | MCU |
|------|---------|-----|
| SDA | A4 / D18 | PC4 |
| SCL | A5 / D19 | PC5 |

## 来源

- 上游：MiniCore `avr/libraries/Wire/src/`
- 拷贝日期：2026-06
