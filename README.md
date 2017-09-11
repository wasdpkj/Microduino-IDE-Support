Microduino-IDE ![Build Status](https://travis-ci.org/wasdpkj/Microduino-IDE-Support.svg?branch=master)
========

### 2017-07-14：
- Library进行了一次大更新，可能会和之前的程序有不兼容
- 如需下载更新前的老版本，可去OLD分支下载：[OLD-Download](https://github.com/wasdpkj/Microduino-IDE-Support/tree/OLD)
- libraries have made a major update and may be incompatible with previous programs
- To download the old version before the update, go to OLD branch download：[OLD-Download](https://github.com/wasdpkj/Microduino-IDE-Support/tree/OLD)

#### We do a lot of libraries related work, to better support Microduino and mCookie Module
#### libraries are not included in the support package, You will need to install support packages and libraries separately
#### 支持包中未包含库，你需要分别安装支持包和库

How to add support packages for Microduino：
------------
- Add a URL for a package to "File>Preferences>Additional Boards Manager URLs"
  - URL:https://github.com/wasdpkj/Microduino-IDE-Support/raw/master/package_Microduino_index.json
- Tools>Board>Boards Manager...
- Select the board and version you want to install
  - "Microduino AVR Boards"
  - "Microduino ESP32 Boards"
- Wait for all downloads to complete
- Close Boards Manager
- Select the board at "Tools>Board"
  - "Microduino/mCookie-device"
  - "Microduino CoreESP32"

How to add support libraries for Microduino：
------------
- Clone this project or Download: [Microduino-IDE-Support-master.zip](https://github.com/wasdpkj/Microduino-IDE-Support/archive/master.zip)
- We recommend: first clear the Arduino installation directory libraries folder, to prevent some of the library conflict:
  - for example Servo, We have modified the code to support more cores
- Copy the libraries folder to the Arduino installation directory

Website:
------------
- [Microduino EN](http://www.microduino.cc)
- [Microduino CN](http://www.microduino.cn)

- [Microduino wiki EN](http://wiki.microduino.cc)
- [Microduino wiki CN](http://wiki.microduino.cn)

- E-mail:
  - Kejia Pan
  + pankejia@microduino.cc

- Weibo:
  - @老潘orz

Core Pinmap:
------------
![Pin Functions](_doc/pinmap-coreesp.jpg)

