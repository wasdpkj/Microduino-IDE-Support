Microduino Arduino IDE package's history£º
*On February 14, 2015, Update hardware file, Makes atmega32u4 Analog I/O increased to 10: add D8 (A8), D9 (A9), D3 (A10), D4 (A11)
*On October 19, 2014, Update atmega32u4 bootloader file, fix bugs of that D10 pin doesn't work correctly when be configured as RX_LED signal
*On October 17, 2014, Update HID.cpp file, fix bugs of the atmega32u4 can not serve as the mouse and keyboard.
*On October 10, 2014, added avr-libc folder, it will lead to atmega32u4 bug if missed it.
*On October 7, 2014, fixed 1284 fuse wire issue which lead to IO port useless.
*On June 24, 2014, fixed the issue that atmega32u4 can't read "AX" port by AnglogRead, all core will use A6, A7. 
*On May 27, 2013, official upgraded to optiboot5.0£¬support all Microduino core core+, question feedback QQ£º51728096¡£
*On May 16, 2013, beta release supports core+ (ATmega1284p@8M,3.3V), supported beta release download
*On April 29, 2013, upgraded to optiboot5.0, supported core+(ATmega1284p@16M,5V), supported beta release download
*On April 22, 2013, [[user:Tiki|Tiki]] correct core+ soft serial port issue.
*On March 28, 2013, fixed issue for core+, analogRead(0) can't match to A0.
*On February 2013, fixed optiboot bug, resolved Microduino-Core+ (Atmega644pa@16M£¬5V), Microduino-Core+(Atmega644PA@8M,3.3V) bootloader issue.
*On January 2013, fixed Microduino-Core+ (Atmega644pa@16M)bootloader issue, external interrupt INT2, the second serial port (Serial1) are availabe, fixed ADC convert port issue. The Microduino-Core+(Atmega644pa@16M) user need download the latest package, and rewrite bootloader. 
*On January 19 2013, fixed Microduino-Core+ external interrupt INT2 unavailabe issue.
*On January 13, 2013, fixed Microduino Core£¨Atmega168PA@8M,3.3V) fuse bits. Changed according to 16M release, and didn't test, but it didn't work, so rollback to tested release.
*On January 4, 2013, tested download progaram and write bootload for microduino-core(Atmega168PA@16M£¬5V), success£¡
*On December 29, 2012, tested download progaram and write bootload for Microduino Core(Atmega328P@8M,3.3V), success£¡
*On December 28, 2012, add optibootloader for Atmega644PA. The known issue is that in order to rewrite bootloader, need change the "XXXXX.build.mcu=atmega644" to "XXXXX.build.mcu=atmega644p". After completed, need change back, then restart IDE to enable. Involved the hardware, as follows: 
**Microduino-Core+(Atmega644PA@16M,5V) (supported and tested)
**Microduino-Core+(Atmega644PA@8M,3.3V) (suppprted and tested)
*On December 27, 2012, for some unstabl 168pa chips, used 8MHZ crystal. Optiboot's default baud rate 115200 will lead to can't download, so changed the baud rate to 57600. If still didn't work, need use the 5V input voltage. Involved hardware, as follows£º
**Microduino Core(Atmega168PA@8M,3.3V)(supported and tested)
*On December 23, 2012£¬added support for Microduino-Core+. -Core+ has four hardwares£º
**Microduino-Core+(Atmega1284P@16M,5V) (supported and no test)
**Microduino-Core+(Atmega1284P@8M,3.3V) (doesn't support)
**Microduino-Core+(Atmega644PA@16M,5V) (supported and tested)
**Microduino-Core+(Atmega644PA@8M,3.3V)(doesn't support)
*On December 18, 2012£¬fix Microduino-Core (Atmega168PA@8M,3.3V) bug that can't download because of fuse bit error. Microduino Core (Atmega328P@8M,3.3V) and Microduino Core (Atmega168PA@16M,5V) haven't been tested.
*On December 16, 2012£¬added Microduino-Core's 4 hardwares£º
**Microduino Core(Atmega328P@16M,5V) (supported and tested) 
**Microduino Core(Atmega328P@8M,3.3V£© (supported and no test)
**Microduino Core(Atmega168PA@16M,5V£©(supported and no test)
**Microduino Core(Atmega168PA@8M,3.3V£©(supported and tested)
**Arduino IDE 1.03 core files.