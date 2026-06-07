# ATmega328PB Bootloader (optiboot_flash) — 备用

本目录 hex **当前未用于 `md328pb` 板卡**。板卡已改用 **urboot + urclock**，见 `../urboot/README_328pb.md`。

保留这些文件供手动烧录、或在不支持 urclock 的旧环境中使用 **arduino（STK500v1）** 协议上传。


| 项目 | 说明 |
|------|------|
| [MCUdude/optiboot_flash](https://github.com/MCUdude/optiboot_flash) | Bootloader 源码与预编译 hex 仓库 |
| [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) | 本包 hex 的拷贝来源（`hardware/miniCore/avr/bootloaders/optiboot_flash/`） |

预编译 hex 取自 MiniCore 内 `optiboot_flash/bootloaders/atmega328pb/{F_CPU}/` 目录，构建记录见：

`../optiboot_flash/bootloaders/atmega328pb/atmega328pb_build_info.txt`

## 源码位置（本仓库）

可重新编译 ATmega328PB bootloader 的源码在：

```
hardware/Microduino/avr/bootloaders/optiboot_flash/
```

构建示例（需 avr-gcc、make）：

```bash
cd hardware/Microduino/avr/bootloaders/optiboot_flash
make atmega328pb AVR_FREQ=16000000L BAUD_RATE=115200 LED=B5 LED_START_FLASHES=2 UART=0
make atmega328pb AVR_FREQ=16000000L BAUD_RATE=115200 LED=B5 LED_START_FLASHES=2 UART=1
```

## 本目录 hex 与上游文件名对照

板卡通过 `bootloader.file=optiboot/optiboot_micro_328pb_{build.bootloader_clock}_{build.bootloader_uart}.hex` 选择文件。

| 本仓库文件 | 上游原始文件 | F_CPU | UART | 默认波特率 |
|------------|--------------|-------|------|------------|
| `optiboot_micro_328pb_8m_uart0.hex` | `optiboot_flash_atmega328pb_UART0_57600_8000000L_B5.hex` | 8 MHz | UART0 (D0/D1) | 57600 |
| `optiboot_micro_328pb_8m_uart1.hex` | `optiboot_flash_atmega328pb_UART1_57600_8000000L_B5.hex` | 8 MHz | UART1 (D11=TX, D12=RX) | 57600 |
| `optiboot_micro_328pb_12m_uart0.hex` | `optiboot_flash_atmega328pb_UART0_57600_12000000L_B5.hex` | 12 MHz | UART0 | 57600 |
| `optiboot_micro_328pb_12m_uart1.hex` | `optiboot_flash_atmega328pb_UART1_57600_12000000L_B5.hex` | 12 MHz | UART1 | 57600 |
| `optiboot_micro_328pb_16m_uart0.hex` | `optiboot_flash_atmega328pb_UART0_115200_16000000L_B5.hex` | 16 MHz | UART0 | 115200 |
| `optiboot_micro_328pb_16m_uart1.hex` | `optiboot_flash_atmega328pb_UART1_115200_16000000L_B5.hex` | 16 MHz | UART1 | 115200 |
| `optiboot_micro_328pb_20m_uart0.hex` | `optiboot_flash_atmega328pb_UART0_115200_20000000L_B5.hex` | 20 MHz | UART0 | 115200 |
| `optiboot_micro_328pb_20m_uart1.hex` | `optiboot_flash_atmega328pb_UART1_115200_20000000L_B5.hex` | 20 MHz | UART1 | 115200 |

公共参数：LED = **PB5**，LED 闪烁 2 次，上传协议 **arduino**（STK500v1）。

## 相关 328PB 支持文件

| 路径 | 来源 | 说明 |
|------|------|------|
| `variants/328pb/pins_arduino.h` | MiniCore `pb-variant` | 引脚映射（含 A6/A7 → D25/D26） |
| `cores/arduino/wiring_analog.c` | MiniCore 同款补丁 | TIMER3B PWM 硅片 errata 修复 |
| `../urboot/README_328pb.md` | MiniCore / urboot.hex | 备用 urboot autobaud hex（当前板卡未使用） |

## 许可证

optiboot_flash 遵循上游仓库许可证（见 `../optiboot_flash/README.md`）。
