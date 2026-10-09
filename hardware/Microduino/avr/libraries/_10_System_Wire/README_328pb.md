# Wire 库（328PB 相关说明）

本目录 Wire 库已升级为 [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) 同款（`libraries/Wire`），并带有 `library.properties`（`name=Wire`），便于与 IDE/Mixly 自带 Wire 区分优先级。

## 为何升级

328PB 存在 **TWI1** 硬件。MiniCore 在检测到 `TWI1_vect` 时，`TwoWire` 改为可注入底层驱动的实现。

- **328P / 644 等无 TWI1 的 MCU**：仍走旧版 `TwoWire` 静态缓冲区路径，与原先 API 兼容。
- **328PB**：TWI0 仍用本库 `Wire`；**TWI1 由独立库 `_10_System_Wire1`（`TwoWire1`）提供**，不依赖本库是否被 Mixly 选中。

若 Mixly 提示 “Wire.h 对应多个库” 且选用了 IDE 旧 Wire：仅影响 TWI0，**不影响 Wire1**。

## 引脚（TWI0 / `Wire`）

| 信号 | Arduino | MCU |
|------|---------|-----|
| SDA | A4 / D18 | PC4 |
| SCL | A5 / D19 | PC5 |

## 来源

- 上游：MiniCore `avr/libraries/Wire/src/`
- 拷贝日期：2026-06
