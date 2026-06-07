# ATmega328PB Variant (328pb)

## 来源

引脚定义改编自 [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) 的 `variants/pb-variant/pins_arduino.h`。

## 328PB 专用引脚（相对 328P）

| Arduino | MCU 引脚 | 功能 |
|---------|----------|------|
| D11 | PB3 | **Serial1 TX**（TXD1） |
| D12 | PB4 | **Serial1 RX**（RXD1） |
| D23 | PE0 | **Wire1 SDA**（TWI1） |
| D24 | PE1 | **Wire1 SCL**（TWI1） |
| D25 | PE2 | **A6** |
| D26 | PE3 | **A7** |

- **Serial**（UART0）：D0=RX, D1=TX（与 328P 相同）
- **Serial1**（UART1）：D11=TX, D12=RX
- **Wire**（TWI0）：D18=SDA, D19=SCL（与 328P 相同）
- **Wire1**（TWI1）：D23=SDA, D24=SCL

## 相关 core 补丁

`cores/arduino/wiring_analog.c` 中对 `__AVR_ATmega328PB__` 的 TIMER3B PWM 处理（`PORTD |= _BV(PD2)`）来自 MiniCore 同款修复，解决 328PB TIMER3B 输出 errata。

- **Wire1**（TWI1）：D23=SDA, D24=SCL — 库 `_10_System_Wire1`
- **SPI1**：D25=SS, D26=MOSI, D14=MISO, D15=SCK — 库 `_10_System_SPI1`（与 A6/A7 引脚复用）

## 相关库

| 库目录 | 对象 | 总线 |
|--------|------|------|
| `_10_System_Wire` | `Wire` | TWI0（A4/A5） |
| `_10_System_Wire1` | `Wire1` | TWI1（D23/D24） |
| `_10_System_SPI` | `SPI` | SPI0（D10–D13） |
| `_10_System_SPI1` | `SPI1` | SPI1（见上表） |

`_10_System_Wire` 已升级为 MiniCore 版本以支持 Wire1（328P 仍兼容原 API）。

## Bootloader

见 `bootloaders/urboot/README_328pb.md`（urboot + urclock）。

- UART0 上传：串口接 **D0/D1**
- UART1 上传：串口接 **D12(RX)/D11(TX)**，勿与 **D23/D24（TWI1）** 混淆
