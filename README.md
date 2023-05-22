Microduino-IDE [![Build Status](https://travis-ci.org/wasdpkj/Microduino-IDE-Support.svg?branch=master)](https://travis-ci.org/wasdpkj/Microduino-IDE-Support) [![GitHub version](https://img.shields.io/github/release/wasdpkj/Microduino-IDE-Support.svg?branch=master)](https://github.com/wasdpkj/Microduino-IDE-Support/releases/latest)
========

- `2017-07-14：`
  - `Library进行了一次大更新，可能会和之前的程序有不兼容`
  - `如需下载更新前的老版本，可去OLD分支下载：`[**`OLD-Download`**](https://github.com/wasdpkj/Microduino-IDE-Support/tree/OLD)
  - `The libraries have been updated greatly and the new ones maybe incompatible with the previous programs`
  - `To download the old version before the update, please go to OLD branch：`[**`OLD-Download`**](https://github.com/wasdpkj/Microduino-IDE-Support/tree/OLD)


## Table of Contents

1. [Release](#release)
   1. [Latest Releases](#latest-releases)
   2. [Previous Releases](#previous-releases)
2. [Installation](#installation)
   1. [How to add the support package for Microduino](#how-to-add-the-support-package-for-microduino)
   2. [How to add the libraries for Microduino](#how-to-add-the-libraries-for-microduino)
3. [Drivers](#drivers)
   1. [Drivers for windows](#drivers-for-windows)
   2. [Drivers for macox](#drivers-for-macox)
4. [Core Pinmap](#core-pinmap)
   1. [Core](#core)
   2. [CorePlus](#core-plus)
   3. [CoreESP](#core-esp)
5. [Website](#website)


## Release

### Latest Releases
- https://github.com/wasdpkj/Microduino-IDE-Support/releases/latest

### Previous Releases
- https://github.com/wasdpkj/Microduino-IDE-Support/releases


## Installation

- We do a lot of work related to the libraries for better support of Microduino and mCookie Modules
- **The libraries are not included in the support package, so the libraries needs additional installation**
- 我们为了让Microduino和mCookie模块的库更好使用，做了很多工作
- **支持包中未包含库，库需要另外安装**

### How to add the support package for Microduino

- Add the URL for the package to `"File>Preferences>Additional Boards Manager URLs"`
  - URL:https://raw.githubusercontent.com/wasdpkj/Microduino-IDE-Support/master/_for_travis/package_microduino_index.json
- `"Tools>Board>Boards Manager"`...
- Select the board and the latest version you want to install
  - `"Microduino AVR Boards"`
  - `"Microduino ESP32 Boards"`
- Wait for it to download completely
- Close Boards Manager
- Select the board at `"Tools>Board"`
  - `"Microduino/mCookie-device"`
  - `"Microduino CoreESP32"`

### How to add the libraries for Microduino

- Clone this project or Download: [Microduino-IDE-Support-master.zip](https://github.com/wasdpkj/Microduino-IDE-Support/archive/master.zip)
- We recommend: first clear the Arduino installation directory libraries folder, to prevent the conflict between libraries: 
  - For example Servo, we have modified the code to support more cores
- Copy the libraries folder to the Arduino installation directory


## Drivers

### Drivers for windows

- [**Download**](https://github.com/wasdpkj/Microduino-IDE-Support/raw/master/_drivers/drivers%20for%20win-V1.zip)
- X64 Run:`dpinst-amd64.exe`
- X86 Run:`dpinst-x86.exe`

### Drivers for macox

- [**Download**](https://github.com/wasdpkj/Microduino-IDE-Support/raw/master/_drivers/drivers%20for%20mac-V1.zip)
- Support:  
  - CP210X Install: `mCenter`,`ideaBoard`,`CoreESP`
  - USBTTL-C Install: `USBTTL-C`,`mBattery`,`ideaBox`


## Core Pinmap

### Core

![Pin Functions](https://github.com/wasdpkj/Microduino-IDE-Support/raw/master/_doc/pinmap-core.png)

### Core Plus

![Pin Functions](https://github.com/wasdpkj/Microduino-IDE-Support/blob/master/_doc/pinmap-core%2B.png)

### Core ESP

![Pin Functions](https://github.com/wasdpkj/Microduino-IDE-Support/raw/master/_doc/pinmap-coreesp.png)


## Website

- [Microduino EN](http://www.microduino.cc)
- [Microduino CN](http://www.microduino.cn)

- [Microduino wiki EN](http://wiki.microduino.cc)
- [Microduino wiki CN](http://wiki.microduino.cn)

- E-mail:
  - Kejia Pan
  + pankejia@microduino.cc

- Weibo:
  - @老潘orz
