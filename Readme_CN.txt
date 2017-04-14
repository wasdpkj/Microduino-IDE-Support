Microduino Arduino IDE 配置包历史：

*2017年4月14日，更新WIFI库：_03_Microduino_WIFI_ESP8266，支持软串口
*2017年4月1日，更新GPS库：_05_Microduino_GPS，支持软串口
*2017年3月30日，更新Motor库：_04_Microduino_Motor，自动识别PWM接口
*2017年3月8日，更新AudioPro新库：_06_Microduino_AudioPro_VS1053
*2016年5月12日，给所有核心硬件支持包添加了digitalPinToInterrupt函数，Core核心库更新为1.6.9。
*2016年3月21日，更新了DotMatrix库，修正了几个BUG，支持了24bit的BMP显示。
*2016年2月03日，更新了AM2321库问题，支持负数温度值；在Adafruit_VS1053库中添加了playBuffer函数用以播放对应长度的音乐数据。
*2016年1月10日，添加了Blynk库，支持Microduino_Ethernet_ENC28J60、Microduino_Ethernet_W5500、Microduino_WIFI_CC3000、Microduino_WIFI_ESP8266。
*2016年1月20日，添加了Microduino官方的_01_Microduino_DotMatrix、_08_Shield_Microduino_Key、_04_Microduino_Motor库，来自老潘。
*2016年1月15日，atmega32u4添加支持USBTTL的bootloader，但因为bootloader大小4K限制，不能同时支持CDC下载，需要刷不同的bootloader并从菜单选择是USBTTL还是USB。
*2016年1月01日，整个Core核心包重新从arduino官方1.6.7移植更新，直接支持atmega128rfa1，提升编译速度。
*2015年2月14日，更新了atmega32u4的Analog I/O，扩增为10个：新增D8(A8),D9(A9),D3(A10),D4(A11)。
*2014年10月19日，更新了atmega32u4的bootloader文件，解决了D10管脚作为RX_LED引起的BUG。
*2014年10月17日，更新了HID.cpp文件，解决了atmega32u4不能作为鼠标、键盘的BUG。
*2014年10月10日，更新了因为缺少avr-libc文件夹导致atmega32u4的一些BUG问题。
*2014年10月7日，更新了1284熔丝位错误导致IO口不能使用的问题。
*2014年6月24日，更新了atmega32u4的使用AnglogRead直接读取“AX”模拟口问题，所有Core统一A6、A7引脚。
*2013年5月27日，正式升级到optiboot5.0，Microduino core core+全系列提供支持，问题反馈QQ：51728096。
*2013年5月16日,beta版本支持core+（ATmega1284p@8M,3.3V），提供beta版本下载
*2013年4月29日,升级到optiboot5.0版本，支持core+（ATmega1284p@16M,5V），提供beta版本下载
*2013年4月22日，[[用户:Tiki|Tiki]]修正core+软串口问题。
*2013年3月28日，修正对core+，analogRead(0)不能对应到A0。
*2013年2月20日，修正optiboot bug，彻底解决Microduino-Core+（Atmega644pa@16M，5V）Microduino-Core+（Atmega644PA@8M,3.3V）bootloader问题，请大家下载测试，有什么问题，及时反馈。
*2013年1月20日，修正Microduino-Core+（Atmega644pa@16M）bootloader问题，外部中断INT2，第二个串口（Serial1）均已可用，修正ADC转换端口对应问题。请Microduino-Core+（Atmega644pa@16M）用户重新下载支持包，并重新烧写bootloader。 
*2013年1月19日，修正Microduino-Core+外部中断INT2不可用问题。
*2013年1月13日，修正Microduino Core（Atmega168PA@8M,3.3V）熔丝位。根据16M改了没测试，结果不行，更正回已经测试版本。
*2013年1月4日，测试对microduino-core（Atmega168PA@16M，5V）进行烧写，及下载程序，成功！修正了熔丝位，使其更加合理。
*2012年12月29日，测试对Microduino Core（Atmega328P@8M,3.3V）进行烧写，及下载程序，成功！
*2012年12月28日，添加针对Atmega644PA原生的optibootloader。目前已知bug为重新烧写bootloader的时候需要把“XXXXX.build.mcu=atmega644”改为“XXXXX.build.mcu=atmega644p”，烧写完毕再改回来，然后重新启动IDE，才能正常下载。涉及硬件为：
**Microduino-Core+（Atmega644PA@16M,5V）（已支持，已测试）
**Microduino-Core+（Atmega644PA@8M,3.3V）（已支持，已测试）
*2012年12月27日，对于某些不够健壮的168pa芯片，采用8MHZ晶振，optiboot默认的115200波特率下载速度，会出现无法下载问题，这次更新把默认下载波特率调整为57600。如果仍然不能烧写，请把被烧写的模块输入电压调整为5V。涉及硬件为：
**Microduino Core（Atmega168PA@8M,3.3V）（已支持，已测试）
*2012年12月23日，添加对Microduino-Core+的支持。-Core+对应的4个硬件为：
**Microduino-Core+（Atmega1284P@16M,5V）（已支持，未测试）
**Microduino-Core+（Atmega1284P@8M,3.3V）（未支持）
**Microduino-Core+（Atmega644PA@16M,5V）（已支持，已测试）
**Microduino-Core+（Atmega644PA@8M,3.3V）（未支持）
*2012年12月18日，修正Microduino-Core （Atmega168PA@8M,3.3V）熔丝位错误导致不能正常下载。Microduino Core （Atmega328P@8M,3.3V）及Microduino Core （Atmega168PA@16M,5V）还未测试。
*2012年12月16日，添加Microduino-Core对应的4个硬件：
**Microduino Core（Atmega328P@16M,5V）（已支持，已测试） 
**Microduino Core（Atmega328P@8M,3.3V）（已支持，未测试）
**Microduino Core（Atmega168PA@16M,5V）（已支持，未测试）
**Microduino Core（Atmega168PA@8M,3.3V）（已支持，已测试）
**核心为Arduino IDE 1.03对应核心文件。