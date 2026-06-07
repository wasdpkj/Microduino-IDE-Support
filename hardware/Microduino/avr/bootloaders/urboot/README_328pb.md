# ATmega328PB Bootloader (urboot + urclock)

板卡 **Microduino/mCookie-Core (328PB)**（`md328pb`）使用 **urboot autobaud** + **urclock** 上传协议。

- **UART0 / UART1 各一份 hex**，8/12/16/20 MHz 外部晶振共用（运行时 autobaud 测速）
- 上传协议：`urclock`（需 Arduino IDE 2.x / avrdude 7+）
- 最大 sketch 空间：**32384** 字节（vector bootloader）

## 文件路径（boards.txt 模板）

```
urboot/atmega328pb/watchdog_1_s/autobaud/{upload.uartpins}/led+b5/urboot_atmega328pb_pr_ee_ce.hex
```

| UART | `{upload.uartpins}` | 硬件引脚 |
|------|---------------------|----------|
| UART0 | `uart0_rxd0_txd1` | D0=RX, D1=TX |
| UART1 | `uart1_rxb4_txb3` | D12=RX (PB4), D11=TX (PB3) |

## 上游来源

| 项目 | 说明 |
|------|------|
| [stefanrueger/urboot](https://github.com/stefanrueger/urboot) | Bootloader 源码 |
| [stefanrueger/urboot.hex](https://github.com/stefanrueger/urboot.hex) | 预编译 hex |
| [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore) | hex 拷贝来源 |

特性标志：`weU-jPrac`（EEPROM、autobaud、vector bootloader、chip erase 等），详见各 UART 目录下 `README.md`。

## 备用：optiboot_flash（arduino 协议）

若需使用经典 **STK500v1 / arduino** 协议，见 `../optiboot/README_328pb.md` 中按频率分文件的 optiboot hex（需手动烧录并改 boards 配置）。

## 相关 328PB 支持文件

| 路径 | 说明 |
|------|------|
| `variants/328pb/` | 引脚映射（MiniCore pb-variant） |
| `cores/arduino/wiring_analog.c` | TIMER3B PWM errata 修复 |
| `../optiboot_flash/` | optiboot 源码（备用） |
