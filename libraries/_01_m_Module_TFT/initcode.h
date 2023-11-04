/*!
 * @file Adafruit_TFT_INIT.h
 *
 * This is the documentation for ST7735/ST7789 driver for the
 * Arduino platform.
 *
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 *
 * This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
 * Adafruit_GFX</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef _INITCODE_
#define _INITCODE_

#include "Arduino.h"

#include "Adafruit_GFX.h"


static const uint8_t PROGMEM
    cmd_initcode_ST7735_128x160[] = {             // Init commands for 7789 screens
        21,                        // 21 commands in list:
        TFT_SWRESET, ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
        150,                       //     150 ms delay
        TFT_SLPOUT, ST_CMD_DELAY,  //  2: Out of sleep mode, 0 args, w/delay
        255,                       //     500 ms delay
        ST7735_FRMCTR1, 3,         //  3: Frame rate ctrl - normal mode, 3 args:
        0x01, 0x2C, 0x2D,          //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR2, 3,         //  4: Frame rate control - idle mode, 3 args:
        0x01, 0x2C, 0x2D,          //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR3, 6,         //  5: Frame rate ctrl - partial mode, 6 args:
        0x01, 0x2C, 0x2D,          //     Dot inversion mode
        0x01, 0x2C, 0x2D,          //     Line inversion mode
        ST7735_INVCTR, 1,          //  6: Display inversion ctrl, 1 arg, no delay:
        0x07,                      //     No inversion
        ST7735_PWCTR1, 3,          //  7: Power control, 3 args, no delay:
        0xA2,
        0x02,             //     -4.6V
        0x84,             //     AUTO mode
        ST7735_PWCTR2, 1, //  8: Power control, 1 arg, no delay:
        0xC5,             //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
        ST7735_PWCTR3, 2, //  9: Power control, 2 args, no delay:
        0x0A,             //     Opamp current small
        0x00,             //     Boost frequency
        ST7735_PWCTR4, 2, // 10: Power control, 2 args, no delay:
        0x8A,             //     BCLK/2, Opamp current small & Medium low
        0x2A,
        ST7735_PWCTR5, 2, // 11: Power control, 2 args, no delay:
        0x8A, 0xEE,
        ST7735_VMCTR1, 1, // 12: Power control, 1 arg, no delay:
        0x0E,
        TFT_INVOFF, 0, // 13: Don't invert display, no args, no delay
        TFT_MADCTL, 1, // 14: Memory access control (directions), 1 arg:
        0xC0,          //     row addr/col addr, bottom to top refresh
        TFT_COLMOD, 1, // 15: set color mode, 1 arg, no delay:
        0x05,
        TFT_CASET, 4, //  1: Column addr set, 4 args, no delay:
        0x00,
        0, //     XSTART = 0
        (128 + 0) >> 8,
        (128 + 0) & 0xFF, //     XEND = 128
        TFT_RASET, 4,                          //  2: Row addr set, 4 args, no delay:
        0x00,
        0, //     YSTART = 0
        (160 + 0) >> 8,
        (160 + 0) & 0xFF, //     YEND = 160
        TFT_GMCTRP1, 16,                        //  1: Magical unicorn dust, 16 args, no delay:
        0x02, 0x1c, 0x07, 0x12,
        0x37, 0x32, 0x29, 0x2d,
        0x29, 0x25, 0x2B, 0x39,
        0x00, 0x01, 0x03, 0x10,
        TFT_GMCTRN1, 16, //  2: Sparkles and rainbows, 16 args, no delay:
        0x03, 0x1d, 0x07, 0x06,
        0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F,
        0x00, 0x00, 0x02, 0x10,
        TFT_NORON, ST_CMD_DELAY,  //  3: Normal display on, no args, w/delay
        10,                       //     10 ms delay
        TFT_DISPON, ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
        100};
		
static const uint8_t PROGMEM
    cmd_initcode_ST7789_240x240[] = {
        // Init commands for 7789 screens
        19, //  9 commands in list:
        // TFT_SWRESET,   ST_CMD_DELAY,	 	//  1: Software reset, no args, w/delay
        //  150,                         	//    150 ms delay
        TFT_SLPOUT, ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
        150,                      //     150 ms delay
        ST7789_PWCTR1, 2,         // Power control
        0xA4, 0xA1,               // AVDD=6.8V,AVCL=-4.8V,VDDS=2.3V
        ST7789_GCCTL, 1,
        0x35,              // VGH=13.26V,VGL=-10.43
        ST7789_VCMOSET, 1, // VCM control
        0x32,              // Vcom=1.35V
        ST7789_VDVVRHEN, 1,
        0x01,
        ST7789_VRHSET, 1,
        0x19, // GVDD=4.8V
        ST7789_VDVSET, 1,
        0x20,                         // VDV=0v
        TFT_COLMOD, 1 + ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
        0x55,                         //     16-bit color
        10,                           //     10 ms delay
        TFT_MADCTL, 1,                //  4: Mem access ctrl (directions), 1 arg:
        0x00,                         //     Row/col addr, bottom-top refresh
        ST7789_PORCTL, 5,
        0x0C, 0x0C, 0x00, 0x33, 0x33,
        ST7789_FRMCTR2, 1, // In Normal Mode
        0x0F,              // 0x0F:60Hz

        TFT_GAMMASET, 1, // Gamma curve selected
        0x01,
        TFT_GMCTRP1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x05, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_GMCTRN1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x15, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_CASET, 4, //  5: Column addr set, 4 args, no delay:
        0x00,
        0, //     XSTART = 0
        (240 + 0) >> 8,	//TFT_TFTWIDTH + TFT_XSTART
        (240 + 0) & 0xFF, //     XEND = 240
        TFT_RASET, 4,                       //  6: Row addr set, 4 args, no delay:
        0x00,
        80, //     YSTART = 80
        (240 + 80) >> 8,	//TFT_TFTHEIGHT + TFT_YSTART
        (240 + 80) & 0xFF, //     YEND = 320
        TFT_INVON, ST_CMD_DELAY,               //  7: hack
        10,
        TFT_NORON, ST_CMD_DELAY,  //  8: Normal display on, no args, w/delay
        10,                       //     10 ms delay
        TFT_DISPON, ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
        100                       //     255 = max (500 ms) delay
};

static const uint8_t PROGMEM
    cmd_initcode_ST7789_240x320[] = {
        // Init commands for 7789 screens
        19, //  9 commands in list:
        // TFT_SWRESET,   ST_CMD_DELAY,	 	//  1: Software reset, no args, w/delay
        //  150,                         	//    150 ms delay
        TFT_SLPOUT, ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
        150,                      //     150 ms delay
        ST7789_PWCTR1, 2,         // Power control
        0xA4, 0xA1,               // AVDD=6.8V,AVCL=-4.8V,VDDS=2.3V
        ST7789_GCCTL, 1,
        0x35,              // VGH=13.26V,VGL=-10.43
        ST7789_VCMOSET, 1, // VCM control
        0x32,              // Vcom=1.35V
        ST7789_VDVVRHEN, 1,
        0x01,
        ST7789_VRHSET, 1,
        0x19, // GVDD=4.8V
        ST7789_VDVSET, 1,
        0x20,                         // VDV=0v
        TFT_COLMOD, 1 + ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
        0x55,                         //     16-bit color
        10,                           //     10 ms delay
        TFT_MADCTL, 1,                //  4: Mem access ctrl (directions), 1 arg:
        0x00,                         //     Row/col addr, bottom-top refresh
        ST7789_PORCTL, 5,
        0x0C, 0x0C, 0x00, 0x33, 0x33,
        ST7789_FRMCTR2, 1, // In Normal Mode
        0x0F,              // 0x0F:60Hz

        TFT_GAMMASET, 1, // Gamma curve selected
        0x01,
        TFT_GMCTRP1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x05, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_GMCTRN1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x15, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_CASET, 4, //  5: Column addr set, 4 args, no delay:
        0x00,
        0, //     XSTART = 0
        (240 + 0) >> 8,
        (240 + 0) & 0xFF, //     XEND = 240
        TFT_RASET, 4,                       //  6: Row addr set, 4 args, no delay:
        0x00,
        0, //     YSTART = 0
        (320 + 0) >> 8,
        (320 + 0) & 0xFF, //     YEND = 320
        TFT_INVON, ST_CMD_DELAY,               //  7: hack
        10,
        TFT_NORON, ST_CMD_DELAY,  //  8: Normal display on, no args, w/delay
        10,                       //     10 ms delay
        TFT_DISPON, ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
        100                       //     255 = max (500 ms) delay
};

static const uint8_t PROGMEM
    cmd_initcode_ST7789_172x320[] = {
        // Init commands for 7789 screens
        19, //  9 commands in list:
        // TFT_SWRESET,   ST_CMD_DELAY,	 	//  1: Software reset, no args, w/delay
        //  150,                         	//    150 ms delay
        TFT_SLPOUT, ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
        255,                      //     500 ms delay
        ST7789_PWCTR1, 2,         // Power control
        0xA4, 0xA1,               // AVDD=6.8V,AVCL=-4.8V,VDDS=2.3V
        ST7789_GCCTL, 1,
        0x35,              // VGH=13.26V,VGL=-10.43
        ST7789_VCMOSET, 1, // VCM control
        0x32,              // Vcom=1.35V
        ST7789_VDVVRHEN, 1,
        0x01,
        ST7789_VRHSET, 1,
        0x19, // GVDD=4.8V
        ST7789_VDVSET, 1,
        0x20,                         // VDV=0v
        TFT_COLMOD, 1 + ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
        0x55,                         //     16-bit color
        10,                           //     10 ms delay
        TFT_MADCTL, 1,                //  4: Mem access ctrl (directions), 1 arg:
        0x00,                         //     Row/col addr, bottom-top refresh
        ST7789_PORCTL, 5,
        0x0C, 0x0C, 0x00, 0x33, 0x33,
        ST7789_FRMCTR2, 1, // In Normal Mode
        0x0F,              // 0x0F:60Hz

        TFT_GAMMASET, 1, // Gamma curve selected
        0x01,
        TFT_GMCTRP1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x05, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_GMCTRN1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x15, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_CASET, 4, //  5: Column addr set, 4 args, no delay:
        0x00,
        35, //     XSTART = 35
        (170 + 35) >> 8,
        (170 + 35) & 0xFF, //     XEND = 205
        TFT_RASET, 4,                       //  6: Row addr set, 4 args, no delay:
        0x00,
        0, //     YSTART = 0
        (320 + 0) >> 8,
        (320 + 0) & 0xFF, //     YEND = 320
        TFT_INVON, ST_CMD_DELAY,               //  7: hack
        10,
        TFT_NORON, ST_CMD_DELAY,  //  8: Normal display on, no args, w/delay
        10,                       //     10 ms delay
        TFT_DISPON, ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
        255                       //     255 = max (500 ms) delay
};


static const uint8_t PROGMEM
    cmd_initcode_ST7789_135x240[] = {
        // Init commands for 7789 screens
        19, //  9 commands in list:
        // TFT_SWRESET,   ST_CMD_DELAY,	 	//  1: Software reset, no args, w/delay
        //  150,                         	//    150 ms delay
        TFT_SLPOUT, ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
        255,                      //     500 ms delay
        ST7789_PWCTR1, 2,         // Power control
        0xA4, 0xA1,               // AVDD=6.8V,AVCL=-4.8V,VDDS=2.3V
        ST7789_GCCTL, 1,
        0x35,              // VGH=13.26V,VGL=-10.43
        ST7789_VCMOSET, 1, // VCM control
        0x32,              // Vcom=1.35V
        ST7789_VDVVRHEN, 1,
        0x01,
        ST7789_VRHSET, 1,
        0x19, // GVDD=4.8V
        ST7789_VDVSET, 1,
        0x20,                         // VDV=0v
        TFT_COLMOD, 1 + ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
        0x55,                         //     16-bit color
        10,                           //     10 ms delay
        TFT_MADCTL, 1,                //  4: Mem access ctrl (directions), 1 arg:
        0x00,                         //     Row/col addr, bottom-top refresh
        ST7789_PORCTL, 5,
        0x0C, 0x0C, 0x00, 0x33, 0x33,
        ST7789_FRMCTR2, 1, // In Normal Mode
        0x0F,              // 0x0F:60Hz

        TFT_GAMMASET, 1, // Gamma curve selected
        0x01,
        TFT_GMCTRP1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x05, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_GMCTRN1, 14, // Set Gamma
        0xD0, 0x08, 0x0E, 0x09,
        0x09, 0x15, 0x31, 0x33,
        0x48, 0x17, 0x14, 0x15,
        0x31, 0x34,
        TFT_CASET, 4, //  5: Column addr set, 4 args, no delay:
        0x00,
        52, //     XSTART = 52
        (135 + 52) >> 8,
        (135 + 52) & 0xFF, //     XEND = 187
        TFT_RASET, 4,                       //  6: Row addr set, 4 args, no delay:
        0x00,
        80, //     YSTART = 80
        (240 + 80) >> 8,
        (240 + 80) & 0xFF, //     YEND = 320
        TFT_INVON, ST_CMD_DELAY,               //  7: hack
        10,
        TFT_NORON, ST_CMD_DELAY,  //  8: Normal display on, no args, w/delay
        10,                       //     10 ms delay
        TFT_DISPON, ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
        255                       //     255 = max (500 ms) delay
};

#endif
