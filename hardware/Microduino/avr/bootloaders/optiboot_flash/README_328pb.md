# optiboot_flash — ATmega328PB 构建源码

本目录为 [MCUdude/optiboot_flash](https://github.com/MCUdude/optiboot_flash) 源码拷贝，用于追溯与重新编译 **ATmega328PB** bootloader。

## 拷贝来源

- **仓库**： [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore)
- **路径**： `avr/bootloaders/optiboot_flash/`
- **拷贝日期**：2026-06（随 Microduino AVR 328PB 支持一并引入）

未包含全部 8976 个预编译 hex，仅保留源码及 `bootloaders/atmega328pb/atmega328pb_build_info.txt` 构建日志。

## 重新编译 328PB

```bash
# UART0 @ 16 MHz, 115200（与 md328pb 默认配置一致）
make atmega328pb AVR_FREQ=16000000L BAUD_RATE=115200 LED=B5 LED_START_FLASHES=2 UART=0

# UART1 @ 8 MHz, 57600
make atmega328pb AVR_FREQ=8000000L BAUD_RATE=57600 LED=B5 LED_START_FLASHES=2 UART=1
```

输出 hex 位于 `bootloaders/atmega328pb/{AVR_FREQ}/`，可按 `../optiboot/README_328pb.md` 中的命名规则重命名后放入 `../optiboot/`。

## 预编译 hex（本包实际使用）

已重命名并部署至：`../optiboot/optiboot_micro_328pb_*.hex`

详见：`../optiboot/README_328pb.md`
