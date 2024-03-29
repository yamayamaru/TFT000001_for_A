// TFT000001.h
// Version : 0.0.4
//
//    ILI9486, ILI9341 LCD Graphics Library for Raspberry Pi Pico Arduino and Teensy 4.x Arduino
//                                         https://twitter.com/yama23238
//
//    今まで作ったものを利用しやすい形にまとめてライブラリにしてみました。
//    まだ試作段階ですのでご了承ください。
//    一応動いたので公開しました。
//    試行錯誤で制作しているので、今後、作り直しや互換性に問題が出ることがあり得ます。
//    
//    yama_2_GFX_H_beta and yama_2_GFX_H_beta_Canvas is based on Adafruit GFX.
//    Copyright (c) 2012 Adafruit Industries.
//    Released under the BSD License
//    https://github.com/adafruit/Adafruit-GFX-Library/blob/master/license.txt
//
//    init_ILI9341,pixel_write16,pixel_write,fill_color16,fillRect16,
//    drawPixel16,drawRGBBitmap16,drawFastVLine16,drawFastVLine24,drawFastHLine16,
//    drawFastHLine24,drawBitmap16,drawBitmap24 in the TFT000001 class is:
//    Copyright (c) 2016 Hiroshi Narimatsu
//    Released under the MIT License
//    https://github.com/h-nari/Humblesoft_ILI9341/blob/master/LICENSE
//
//    set_rotation_ILI9486,set_rotation_ILI9341,init_ILI9486,setAddrWindow,
//    setWindow, pioinit in the TFT000001 class 
//    and pio_8bit_parallel.pio is :
//    Copyright (c) 2020 Bodmer (https://github.com/Bodmer)
//    Software License Agreement (FreeBSD License)
//    https://github.com/Bodmer/TFT_eSPI/blob/master/license.txt
//
//      これらのプログラムの使用に当たってはご自分の責任において使用してください
//      これらのプログラムで発生したいかなる損害、データの消失、金銭等の責任は一切負いません。
//
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TFT000001_config.h"

#if defined(TFT000001_ARDUINO_UNO_R4_SPI)
#define TFT000001_ARDUINO_SPI
#endif

#ifdef TFT000001_RP2040_PIO_8BIT_PARALELL
  #include "hardware/pio.h"
  #include "pio_8bit_parallel.pio.h"
#endif

#if defined(TFT000001_RP2040_ARDUINO_SPI) || defined(TFT000001_TEENSY4x_ARDUINO_SPI) || defined(TFT000001_ARDUINO_SPI)
  #include "SPI.h"
#endif

#if  defined(TFT000001_RP2040_ARDUINO_SPI)
  #if defined(TFT000001_RP2040_FAST_SPI_MODE)
      #define TFT000001_RP2040_SPI_DRAW02_MODE
  #endif
#endif

#if defined(TFT000001_RP2040_PIO_8BIT_PARALELL)
  #define TFT000001_RP2040_SPI_DRAW02_MODE
#endif

#ifndef _YAMA_2_GFX_H_BETA_H
#define _YAMA_2_GFX_H_BETA_H
 
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_BLACK      0xd7     ///< 0x0000    0,   0,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_NAVY       0xda     ///< 0x0010    0,   0, 128
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_DARKGREEN  0xdc     ///< 0x0400    0, 128,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_DARKCYAN   0xdb     ///< 0x0410    0, 125, 123
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_MAROON     0xdf     ///< 0x8000  128,   0,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_PURPLE     0xde     ///< 0x8010  128,   0, 128
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_OLIVE      0xdd     ///< 0x8400  128, 128,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_LIGHTGREY  0xd9     ///< 0xc618  198, 195, 198
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_DARKGREY   0xd8     ///< 0x8410  128, 128, 128
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_BLUE       0xd2     ///< 0x001f    0,   0, 255
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_GREEN      0xd2     ///< 0x07e0    0, 255,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_CYAN       0xb4     ///< 0x07ff    0, 255, 255
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_RED        0x23     ///< 0xf800  255,   0,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_MAGENTA    0x1e     ///< 0xf81f  255,   0, 255
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_YELLOW     0x05     ///< 0xffe0  255, 255,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_WHITE      0x00     ///< 0xffff  255, 255, 255
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_ORANGE     0x11     ///< 0xfcc0  255, 152,   0
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_GREENYELLOW  0x4c   ///< 0x9fe6  152, 255,  48
#define YAMA_2_GFX_H_BETA_WEB216_PALETTE_PINK       0x13     ///< 0xfb59  255, 104, 200


#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_BLACK      0x00     ///< 0x0000    0,   0,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_NAVY       0x04     ///< 0x0010    0,   0, 128
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_DARKGREEN  0x02     ///< 0x0400    0, 128,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_DARKCYAN   0x06     ///< 0x0410    0, 125, 123
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_MAROON     0x01     ///< 0x8000  128,   0,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_PURPLE     0x05     ///< 0x8010  128,   0, 128
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_OLIVE      0x03     ///< 0x8400  128, 128,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_LIGHTGREY  0x07     ///< 0xc618  198, 195, 198
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_DARKGREY   0xf8     ///< 0x8410  128, 128, 128
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_BLUE       0xfc     ///< 0x001f    0,   0, 255
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_GREEN      0xfa     ///< 0x07e0    0, 255,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_CYAN       0xfe     ///< 0x07ff    0, 255, 255
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_RED        0xf9     ///< 0xf800  255,   0,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_MAGENTA    0xfd     ///< 0xf81f  255,   0, 255
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_YELLOW     0xfb     ///< 0xffe0  255, 255,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_WHITE      0xff     ///< 0xffff  255, 255, 255
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_ORANGE     0xc6     ///< 0xfce0  255, 156,   0
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_GREENYELLOW  0x8d   ///< 0xafea  168, 255,  80
#define YAMA_2_GFX_H_BETA_DEFAULT_PALETTE_PINK       0xc0     ///< 0xfaf5  255,  92, 168

#define YAMA_2_GFX_H_BETA_16BIT_COLOR_BLACK 0x0000       ///<   0,   0,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_NAVY 0x000F        ///<   0,   0, 123
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_DARKGREEN 0x03E0   ///<   0, 125,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_DARKCYAN 0x03EF    ///<   0, 125, 123
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_MAROON 0x7800      ///< 123,   0,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_PURPLE 0x780F      ///< 123,   0, 123
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_OLIVE 0x7BE0       ///< 123, 125,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_LIGHTGREY 0xC618   ///< 198, 195, 198
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_DARKGREY 0x7BEF    ///< 123, 125, 123
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_BLUE 0x001F        ///<   0,   0, 255
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_GREEN 0x07E0       ///<   0, 255,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_CYAN 0x07FF        ///<   0, 255, 255
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_RED 0xF800         ///< 255,   0,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_MAGENTA 0xF81F     ///< 255,   0, 255
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_YELLOW 0xFFE0      ///< 255, 255,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_WHITE 0xFFFF       ///< 255, 255, 255
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_ORANGE 0xFD20      ///< 255, 165,   0
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define YAMA_2_GFX_H_BETA_16BIT_COLOR_PINK 0xFC18        ///< 255, 130, 198

#define YAMA_2_GFX_H_BETA_24BIT_COLOR_BLACK      0x000000 ///<   0,   0,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_NAVY       0x000080 ///<   0,   0, 128
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_DARKGREEN  0x008000 ///<   0, 128,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_DARKCYAN   0x008080 ///<   0, 128, 128
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_MAROON     0x800000 ///< 128,   0,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_PURPLE     0x800080 ///< 128,   0, 128
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_OLIVE      0x808000 ///< 128, 128,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_LIGHTGREY  0xC6C3C6 ///< 198, 195, 198
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_DARKGREY   0x808080 ///< 128, 128, 128
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_BLUE       0x0000FF ///<   0,   0, 255
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_GREEN      0x00FF00 ///<   0, 255,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_CYAN       0x00FFFF ///<   0, 255, 255
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_RED        0xFF0000 ///< 255,   0,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_MAGENTA    0xFF00FF ///< 255,   0, 255
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_YELLOW     0xFFFF00 ///< 255, 255,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_WHITE      0xFFFFFF ///< 255, 255, 255
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_ORANGE     0xFFA500 ///< 255, 165,   0
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_GREENYELLO 0xADFF29 ///< 173, 255,  41
#define YAMA_2_GFX_H_BETA_24BIT_COLOR_PINK       0xFF82C6 ///< 255, 130, 198

#define yama_2_GFX_H_beta_min(a, b) (((a) < (b)) ? (a) : (b))
#define yama_2_GFX_H_beta_swap_int32_t(a, b)                                                    \
{                                                                              \
    int32_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
}
#define yama_2_GFX_H_beta_abs(a)    ((a >= 0) ? a : (-(a)))


struct yama_2_GFX_H_beta_RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class yama_2_GFX_H_beta {
protected:
    int32_t  WIDTH;
    int32_t  HEIGHT;
    int32_t  _width;
    int32_t  _height;
    uint8_t  rotation = 0;


    inline int bit_test(const uint8_t *data, int n) {
        return (data[n >> 3] >> (7 - (n & 7))) & 1;
    }


public:
    yama_2_GFX_H_beta(int32_t w, int32_t h);
    void init_yama_2_GFX_beta(int32_t w, int32_t h, int32_t colorMode);
    virtual void drawPixel(int32_t x, int32_t y, uint32_t color) = 0;
    virtual void startWrite(void);
    virtual void writePixel(int32_t x, int32_t y, uint32_t color);
    virtual void writeFillRect(int32_t x, int32_t y, int32_t w, int32_t h,
                             uint32_t color);
    virtual void writeFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color);
    virtual void writeFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
    virtual void writeLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                         uint32_t color);
    virtual void endWrite(void);
    virtual void setRotation(uint8_t r);
    virtual void invertDisplay(bool i);
    virtual void drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color);
    virtual void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
    virtual void fillRect(int32_t x, int32_t y, int32_t w, int32_t h,
                        uint32_t color);
    virtual void fillScreen(uint32_t color);
    virtual void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                        uint32_t color);
    virtual void drawRect(int32_t x, int32_t y, int32_t w, int32_t h,
                        uint32_t color);
    void drawCircle(int32_t x0, int32_t y0, int32_t r,
                                  uint32_t color);
    void drawCircleHelper(int32_t x0, int32_t y0, int32_t r,
                                        uint8_t cornername, uint32_t color);
    void fillCircle(int32_t x0, int32_t y0, int32_t r,
                                  uint32_t color);
    void fillCircleHelper(int32_t x0, int32_t y0, int32_t r,
                                        uint8_t corners, int32_t delta,
                                        uint32_t color);
    void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h,
                                     int32_t r, uint32_t color);
    void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h,
                                     int32_t r, uint32_t color);
    void drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                                    int32_t x2, int32_t y2, uint32_t color);
    void fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                                    int32_t x2, int32_t y2, uint32_t color);
    void drawBitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                  int32_t w, int32_t h, uint32_t color);
    void drawBitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                  int32_t w, int32_t h, uint32_t color,
                                  uint32_t bg);
    void drawBitmap(int32_t x, int32_t y, uint8_t *bitmap, int32_t w,
                                  int32_t h, uint32_t color);
    void drawBitmap(int32_t x, int32_t y, uint8_t *bitmap, int32_t w,
                                  int32_t h, uint32_t color, uint32_t bg);


    void drawBitmap(int32_t x, int32_t y, const uint8_t *bitmap01, const uint8_t *bitmap_mask01,
                                  int32_t width, int32_t height,
                                  uint32_t fg, uint32_t bg);
    void drawBitmap(int32_t x, int32_t y, uint8_t *bitmap01, uint8_t *bitmap_mask01,
                                  int32_t width, int32_t height,
                                  uint32_t fg, uint32_t bg);


    void drawXBitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                   int32_t w, int32_t h, uint32_t color);
    void drawGrayscaleBitmap8(int32_t x, int32_t y,
                                           const uint8_t bitmap[], int32_t w,
                                           int32_t h);
    void drawGrayscaleBitmap8(int32_t x, int32_t y, uint8_t *bitmap,
                                           int32_t w, int32_t h);
    void drawGrayscaleBitmap8(int32_t x, int32_t y,
                                           const uint8_t bitmap[],
                                           const uint8_t mask[], int32_t w,
                                           int32_t h);
    void drawGrayscaleBitmap8(int32_t x, int32_t y, uint8_t *bitmap,
                                           uint8_t *mask, int32_t w, int32_t h);


    void drawRGBBitmap8(int32_t x, int32_t y,
                                           const uint8_t bitmap[], int32_t w,
                                           int32_t h);
    void drawRGBBitmap8(int32_t x, int32_t y, uint8_t *bitmap,
                                           int32_t w, int32_t h);
    void drawRGBBitmap8(int32_t x, int32_t y,
                                           const uint8_t bitmap[],
                                           const uint8_t mask[], int32_t w,
                                           int32_t h);
    void drawRGBBitmap8(int32_t x, int32_t y, uint8_t *bitmap,
                                           uint8_t *mask, int32_t w, int32_t h);


    void setColorMode8(void);
    void setColorMode16(void);
    void setColorMode24(void);
    int32_t getColorMode(void);

    void drawRGBBitmap16(int32_t x, int32_t y, const uint8_t _bitmap[],
                                     int32_t w, int32_t h);
    void drawRGBBitmap16(int32_t x, int32_t y, uint8_t *_bitmap,
                                     int32_t w, int32_t h);
    void drawRGBBitmap16(int32_t x, int32_t y, const uint8_t _bitmap[],
                                     const uint8_t mask[], int32_t w, int32_t h);
    void drawRGBBitmap16(int32_t x, int32_t y, uint8_t *_bitmap,
                                     uint8_t *mask, int32_t w, int32_t h);

    void drawRGBBitmap24(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t w, int32_t h);
    void drawRGBBitmap24(int32_t x, int32_t y, const uint8_t bitmap[],
                                     const uint8_t mask[], int32_t w, int32_t h);

    void drawPalette256Bitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t width, int32_t height);
    void drawPalette256Bitmap(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t width, int32_t height);
    void drawPalette256Bitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t width, int32_t height, void *palette_address);
    void drawPalette256Bitmap(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t width, int32_t height, void *palette_address);

    void drawPalette256Bitmap16(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t width, int32_t height, uint16_t *palette_address = NULL);
    void drawPalette256Bitmap16(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t width, int32_t height, uint16_t *palette_address = NULL);
    void drawPalette256Bitmap24(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t width, int32_t height, uint32_t *palette_address = NULL);
    void drawPalette256Bitmap24(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t width, int32_t height, uint32_t *palette_address = NULL);

    void drawGrayscaleBitmap16(int32_t x, int32_t y,
                       const uint8_t bitmap[],
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
    void drawGrayscaleBitmap16(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[], 
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
    void drawGrayscaleBitmap24(int32_t x, int32_t y,
                       const uint8_t bitmap[],
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
    void drawGrayscaleBitmap24(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[], 
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);

    void setDefaultPalette256();
    void setWeb216Palette256();
    void setPalette256(uint8_t palette_num, uint32_t color);
    uint32_t getPalette256(uint8_t palette_num);
    void setPalette256(uint8_t palette_num, uint32_t color, int32_t colorMode);
    uint32_t getPalette256(uint8_t palette_num, int32_t colorMode);

    void setDefaultPalette256_16();
    void setWeb216Palette256_16();
    void setPalette256_16(uint8_t palette_num, uint16_t color);
    uint32_t getPalette256_16(uint8_t palette_num);

    void setDefaultPalette256_24();
    void setWeb216Palette256_24();
    void setPalette256_24(uint8_t palette_num, uint32_t color);
    uint32_t getPalette256_24(uint8_t palette_num);

    void *getPaletteTable256();
    void *getPaletteTable256(int32_t colorMode);
    uint16_t *getPaletteTable256_16();
    uint32_t *getPaletteTable256_24();

    uint32_t width(void);
    uint32_t height(void);
 
    uint16_t color565(uint32_t colr, uint32_t colg, uint32_t colb);
    uint16_t color565(uint32_t color);
    uint32_t colorRGB24(uint32_t colr, uint32_t colg, uint32_t colb);
    uint32_t color(uint32_t colr, uint32_t colg, uint32_t colb);
    uint32_t color(uint32_t color);
    uint32_t color16(uint16_t color);
    uint32_t color16to24(uint16_t color);
    uint16_t color24to16(uint32_t color);

protected:
    int32_t colorModeflag;
    static const int palette_number = 256;
    uint16_t __attribute__ ((aligned(4))) palette256_table[palette_number];
    uint32_t __attribute__ ((aligned(4))) palette256_24_table[palette_number];
    static const uint16_t default_palette256_data[];
    static const uint16_t web216_palette256_data[];
    static const uint8_t default_palette256_24_data[];
    static const uint8_t web216_palette256_24_data[];
};
 
#endif // _YAMA_2_GFX_H_BETA_H




#ifndef _TFT000001_H
#define _TFT000001_H

#ifdef TFT000001_RP2040_PIO_8BIT_PARALELL
    #define TFT000001_RP2040_PIO_INTERFACE
    #define TFT000001_DIV_UNITS 1
    #define TFT000001_DIV_FRACT 0
#endif

#define TFT000001_HARDWARE_SPI_NO       10
#define TFT000001_RP2040_PIO_8BIT_PARALELL_NO  11
#define TFT000001_PIO_SPI_NO            12

#define TFT000001_ILI9486_TFTWIDTH  320
#define TFT000001_ILI9486_TFTHEIGHT 480

#define TFT000001_ILI9341_BLACK 0x0000       ///<   0,   0,   0
#define TFT000001_ILI9341_NAVY 0x000F        ///<   0,   0, 123
#define TFT000001_ILI9341_DARKGREEN 0x03E0   ///<   0, 125,   0
#define TFT000001_ILI9341_DARKCYAN 0x03EF    ///<   0, 125, 123
#define TFT000001_ILI9341_MAROON 0x7800      ///< 123,   0,   0
#define TFT000001_ILI9341_PURPLE 0x780F      ///< 123,   0, 123
#define TFT000001_ILI9341_OLIVE 0x7BE0       ///< 123, 125,   0
#define TFT000001_ILI9341_LIGHTGREY 0xC618   ///< 198, 195, 198
#define TFT000001_ILI9341_DARKGREY 0x7BEF    ///< 123, 125, 123
#define TFT000001_ILI9341_BLUE 0x001F        ///<   0,   0, 255
#define TFT000001_ILI9341_GREEN 0x07E0       ///<   0, 255,   0
#define TFT000001_ILI9341_CYAN 0x07FF        ///<   0, 255, 255
#define TFT000001_ILI9341_RED 0xF800         ///< 255,   0,   0
#define TFT000001_ILI9341_MAGENTA 0xF81F     ///< 255,   0, 255
#define TFT000001_ILI9341_YELLOW 0xFFE0      ///< 255, 255,   0
#define TFT000001_ILI9341_WHITE 0xFFFF       ///< 255, 255, 255
#define TFT000001_ILI9341_ORANGE 0xFD20      ///< 255, 165,   0
#define TFT000001_ILI9341_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define TFT000001_ILI9341_PINK 0xFC18        ///< 255, 130, 198

#define TFT000001_WEB216_PALETTE_BLACK      0xd7     ///< 0x0000    0,   0,   0
#define TFT000001_WEB216_PALETTE_NAVY       0xda     ///< 0x0010    0,   0, 128
#define TFT000001_WEB216_PALETTE_DARKGREEN  0xdc     ///< 0x0400    0, 128,   0
#define TFT000001_WEB216_PALETTE_DARKCYAN   0xdb     ///< 0x0410    0, 125, 123
#define TFT000001_WEB216_PALETTE_MAROON     0xdf     ///< 0x8000  128,   0,   0
#define TFT000001_WEB216_PALETTE_PURPLE     0xde     ///< 0x8010  128,   0, 128
#define TFT000001_WEB216_PALETTE_OLIVE      0xdd     ///< 0x8400  128, 128,   0
#define TFT000001_WEB216_PALETTE_LIGHTGREY  0xd9     ///< 0xc618  198, 195, 198
#define TFT000001_WEB216_PALETTE_DARKGREY   0xd8     ///< 0x8410  128, 128, 128
#define TFT000001_WEB216_PALETTE_BLUE       0xd2     ///< 0x001f    0,   0, 255
#define TFT000001_WEB216_PALETTE_GREEN      0xd2     ///< 0x07e0    0, 255,   0
#define TFT000001_WEB216_PALETTE_CYAN       0xb4     ///< 0x07ff    0, 255, 255
#define TFT000001_WEB216_PALETTE_RED        0x23     ///< 0xf800  255,   0,   0
#define TFT000001_WEB216_PALETTE_MAGENTA    0x1e     ///< 0xf81f  255,   0, 255
#define TFT000001_WEB216_PALETTE_YELLOW     0x05     ///< 0xffe0  255, 255,   0
#define TFT000001_WEB216_PALETTE_WHITE      0x00     ///< 0xffff  255, 255, 255
#define TFT000001_WEB216_PALETTE_ORANGE     0x11     ///< 0xfcc0  255, 152,   0
#define TFT000001_WEB216_PALETTE_GREENYELLOW  0x4c   ///< 0x9fe6  152, 255,  48
#define TFT000001_WEB216_PALETTE_PINK       0x13     ///< 0xfb59  255, 104, 200


#define TFT000001_DEFAULT_PALETTE_BLACK      0x00     ///< 0x0000    0,   0,   0
#define TFT000001_DEFAULT_PALETTE_NAVY       0x04     ///< 0x0010    0,   0, 128
#define TFT000001_DEFAULT_PALETTE_DARKGREEN  0x02     ///< 0x0400    0, 128,   0
#define TFT000001_DEFAULT_PALETTE_DARKCYAN   0x06     ///< 0x0410    0, 125, 123
#define TFT000001_DEFAULT_PALETTE_MAROON     0x01     ///< 0x8000  128,   0,   0
#define TFT000001_DEFAULT_PALETTE_PURPLE     0x05     ///< 0x8010  128,   0, 128
#define TFT000001_DEFAULT_PALETTE_OLIVE      0x03     ///< 0x8400  128, 128,   0
#define TFT000001_DEFAULT_PALETTE_LIGHTGREY  0x07     ///< 0xc618  198, 195, 198
#define TFT000001_DEFAULT_PALETTE_DARKGREY   0xf8     ///< 0x8410  128, 128, 128
#define TFT000001_DEFAULT_PALETTE_BLUE       0xfc     ///< 0x001f    0,   0, 255
#define TFT000001_DEFAULT_PALETTE_GREEN      0xfa     ///< 0x07e0    0, 255,   0
#define TFT000001_DEFAULT_PALETTE_CYAN       0xfe     ///< 0x07ff    0, 255, 255
#define TFT000001_DEFAULT_PALETTE_RED        0xf9     ///< 0xf800  255,   0,   0
#define TFT000001_DEFAULT_PALETTE_MAGENTA    0xfd     ///< 0xf81f  255,   0, 255
#define TFT000001_DEFAULT_PALETTE_YELLOW     0xfb     ///< 0xffe0  255, 255,   0
#define TFT000001_DEFAULT_PALETTE_WHITE      0xff     ///< 0xffff  255, 255, 255
#define TFT000001_DEFAULT_PALETTE_ORANGE     0xc6     ///< 0xfce0  255, 156,   0
#define TFT000001_DEFAULT_PALETTE_GREENYELLOW  0x8d   ///< 0xafea  168, 255,  80
#define TFT000001_DEFAULT_PALETTE_PINK       0xc0     ///< 0xfaf5  255,  92, 168

#define TFT000001_16BIT_COLOR_BLACK 0x0000       ///<   0,   0,   0
#define TFT000001_16BIT_COLOR_NAVY 0x000F        ///<   0,   0, 123
#define TFT000001_16BIT_COLOR_DARKGREEN 0x03E0   ///<   0, 125,   0
#define TFT000001_16BIT_COLOR_DARKCYAN 0x03EF    ///<   0, 125, 123
#define TFT000001_16BIT_COLOR_MAROON 0x7800      ///< 123,   0,   0
#define TFT000001_16BIT_COLOR_PURPLE 0x780F      ///< 123,   0, 123
#define TFT000001_16BIT_COLOR_OLIVE 0x7BE0       ///< 123, 125,   0
#define TFT000001_16BIT_COLOR_LIGHTGREY 0xC618   ///< 198, 195, 198
#define TFT000001_16BIT_COLOR_DARKGREY 0x7BEF    ///< 123, 125, 123
#define TFT000001_16BIT_COLOR_BLUE 0x001F        ///<   0,   0, 255
#define TFT000001_16BIT_COLOR_GREEN 0x07E0       ///<   0, 255,   0
#define TFT000001_16BIT_COLOR_CYAN 0x07FF        ///<   0, 255, 255
#define TFT000001_16BIT_COLOR_RED 0xF800         ///< 255,   0,   0
#define TFT000001_16BIT_COLOR_MAGENTA 0xF81F     ///< 255,   0, 255
#define TFT000001_16BIT_COLOR_YELLOW 0xFFE0      ///< 255, 255,   0
#define TFT000001_16BIT_COLOR_WHITE 0xFFFF       ///< 255, 255, 255
#define TFT000001_16BIT_COLOR_ORANGE 0xFD20      ///< 255, 165,   0
#define TFT000001_16BIT_COLOR_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define TFT000001_16BIT_COLOR_PINK 0xFC18        ///< 255, 130, 198

#define TFT000001_24BIT_COLOR_BLACK      0x000000 ///<   0,   0,   0
#define TFT000001_24BIT_COLOR_NAVY       0x000080 ///<   0,   0, 128
#define TFT000001_24BIT_COLOR_DARKGREEN  0x008000 ///<   0, 128,   0
#define TFT000001_24BIT_COLOR_DARKCYAN   0x008080 ///<   0, 128, 128
#define TFT000001_24BIT_COLOR_MAROON     0x800000 ///< 128,   0,   0
#define TFT000001_24BIT_COLOR_PURPLE     0x800080 ///< 128,   0, 128
#define TFT000001_24BIT_COLOR_OLIVE      0x808000 ///< 128, 128,   0
#define TFT000001_24BIT_COLOR_LIGHTGREY  0xC6C3C6 ///< 198, 195, 198
#define TFT000001_24BIT_COLOR_DARKGREY   0x808080 ///< 128, 128, 128
#define TFT000001_24BIT_COLOR_BLUE       0x0000FF ///<   0,   0, 255
#define TFT000001_24BIT_COLOR_GREEN      0x00FF00 ///<   0, 255,   0
#define TFT000001_24BIT_COLOR_CYAN       0x00FFFF ///<   0, 255, 255
#define TFT000001_24BIT_COLOR_RED        0xFF0000 ///< 255,   0,   0
#define TFT000001_24BIT_COLOR_MAGENTA    0xFF00FF ///< 255,   0, 255
#define TFT000001_24BIT_COLOR_YELLOW     0xFFFF00 ///< 255, 255,   0
#define TFT000001_24BIT_COLOR_WHITE      0xFFFFFF ///< 255, 255, 255
#define TFT000001_24BIT_COLOR_ORANGE     0xFFA500 ///< 255, 165,   0
#define TFT000001_24BIT_COLOR_GREENYELLO 0xADFF29 ///< 173, 255,  41
#define TFT000001_24BIT_COLOR_PINK       0xFF82C6 ///< 255, 130, 198


// Delay between some initialisation commands
#define TFT000001_ILI9486_INIT_DELAY 0x80 // Not used unless commandlist invoked
// Generic commands used by TFT000001_ILI9486_eSPI.cpp
#define TFT000001_ILI9486_NOP     0x00
#define TFT000001_ILI9486_SWRST   0x01

#define TFT000001_ILI9486_SLPIN   0x10
#define TFT000001_ILI9486_SLPOUT  0x11

#define TFT000001_ILI9486_INVOFF  0x20
#define TFT000001_ILI9486_INVON   0x21

#define TFT000001_ILI9486_DISPOFF 0x28
#define TFT000001_ILI9486_DISPON  0x29

#define TFT000001_ILI9486_CASET   0x2A
#define TFT000001_ILI9486_PASET   0x2B
#define TFT000001_ILI9486_RAMWR   0x2C
#define TFT000001_ILI9486_RAMRD   0x2E

#define TFT000001_ILI9486_MADCTL  0x36

#define TFT000001_ILI9486_MAD_MY  0x80
#define TFT000001_ILI9486_MAD_MX  0x40
#define TFT000001_ILI9486_MAD_MV  0x20
#define TFT000001_ILI9486_MAD_ML  0x10
#define TFT000001_ILI9486_MAD_RGB 0x00
#define TFT000001_ILI9486_MAD_BGR 0x08
#define TFT000001_ILI9486_MAD_MH  0x04
#define TFT000001_ILI9486_MAD_SS  0x02
#define TFT000001_ILI9486_MAD_GS  0x01

#define TFT000001_ILI9486_DRIVER  0




#define TFT000001_ILI9341_TFTWIDTH  240
#define TFT000001_ILI9341_TFTHEIGHT 320
 
#define TFT000001_ILI9341_NOP     0x00
#define TFT000001_ILI9341_SWRESET 0x01
#define TFT000001_ILI9341_RDDID   0x04
#define TFT000001_ILI9341_RDDST   0x09
 
#define TFT000001_ILI9341_SLPIN   0x10
#define TFT000001_ILI9341_SLPOUT  0x11
#define TFT000001_ILI9341_PTLON   0x12
#define TFT000001_ILI9341_NORON   0x13
 
#define TFT000001_ILI9341_RDMODE  0x0A
#define TFT000001_ILI9341_RDMADCTL  0x0B
#define TFT000001_ILI9341_RDPIXFMT  0x0C
#define TFT000001_ILI9341_RDIMGFMT  0x0D
#define TFT000001_ILI9341_RDSELFDIAG  0x0F
 
#define TFT000001_ILI9341_INVOFF  0x20
#define TFT000001_ILI9341_INVON   0x21
#define TFT000001_ILI9341_GAMMASET 0x26
#define TFT000001_ILI9341_DISPOFF 0x28
#define TFT000001_ILI9341_DISPON  0x29
 
#define TFT000001_ILI9341_CASET   0x2A
#define TFT000001_ILI9341_PASET   0x2B
#define TFT000001_ILI9341_RAMWR   0x2C
#define TFT000001_ILI9341_RAMRD   0x2E
 
#define TFT000001_ILI9341_PTLAR   0x30
#define TFT000001_ILI9341_MADCTL  0x36
#define TFT000001_ILI9341_PIXFMT  0x3A
 
#define TFT000001_ILI9341_FRMCTR1 0xB1
#define TFT000001_ILI9341_FRMCTR2 0xB2
#define TFT000001_ILI9341_FRMCTR3 0xB3
#define TFT000001_ILI9341_INVCTR  0xB4
#define TFT000001_ILI9341_DFUNCTR 0xB6
 
#define TFT000001_ILI9341_PWCTR1  0xC0
#define TFT000001_ILI9341_PWCTR2  0xC1
#define TFT000001_ILI9341_PWCTR3  0xC2
#define TFT000001_ILI9341_PWCTR4  0xC3
#define TFT000001_ILI9341_PWCTR5  0xC4
#define TFT000001_ILI9341_VMCTR1  0xC5
#define TFT000001_ILI9341_VMCTR2  0xC7
 
#define TFT000001_ILI9341_RDID1   0xDA
#define TFT000001_ILI9341_RDID2   0xDB
#define TFT000001_ILI9341_RDID3   0xDC
#define TFT000001_ILI9341_RDID4   0xDD
 
#define TFT000001_ILI9341_GMCTRP1 0xE0
#define TFT000001_ILI9341_GMCTRN1 0xE1

#define TFT000001_MADCTL_MY  0x80
#define TFT000001_MADCTL_MX  0x40
#define TFT000001_MADCTL_MV  0x20
#define TFT000001_MADCTL_ML  0x10
#define TFT000001_MADCTL_RGB 0x00
#define TFT000001_MADCTL_BGR 0x08
#define TFT000001_MADCTL_MH  0x04

#define TFT000001_ILI9341_DRIVER  1


#ifdef TFT000001_RP2040_ARDUINO_SPI
    #define TFT000001_ILI9341_DEFAULT_SPI_PORT SPI
    #define TFT000001_ILI9341_DEFAULT_SPI_PICO_SDK_PORT spi0
#endif

#ifdef TFT000001_TEENSY4x_ARDUINO_SPI
    #define TFT000001_ILI9341_DEFAULT_SPI_PORT SPI
#endif

#ifdef TFT000001_ARDUINO_SPI
    #define TFT000001_ILI9341_DEFAULT_SPI_PORT SPI
#endif

#ifdef TFT000001_ILI9486_DISPLAY
    #define TFT000001_TFT_WIDTH   TFT000001_ILI9486_TFTWIDTH
    #define TFT000001_TFT_HEIGHT  TFT000001_ILI9486_TFTHEIGHT
#else
    #define TFT000001_TFT_WIDTH   TFT000001_ILI9341_TFTWIDTH
    #define TFT000001_TFT_HEIGHT  TFT000001_ILI9341_TFTHEIGHT
#endif



class TFT000001 : public yama_2_GFX_H_beta {
public:

#if defined(TFT000001_RP2040_PIO_8BIT_PARALELL) || defined(TFT000001_TEENSY4x_8BIT_PARALELL)
    TFT000001(int32_t d0=6, int32_t wr=-1, int32_t dc=-1, int32_t rst = -1, int32_t cs = -1);
#endif

#if defined(TFT000001_TEENSY4x_ARDUINO_SPI)
    TFT000001(SPISettings spi_settings, SPIClass *inst = &TFT000001_ILI9341_DEFAULT_SPI_PORT, int32_t dc=-1, 
                  int32_t rst = -1, int32_t cs = -1);
#endif                  

#if defined(TFT000001_RP2040_ARDUINO_SPI)
    #ifdef ARDUINO_ARCH_MBED
        #ifdef TFT000001_RP2040_SPI_DRAW02_MODE
            TFT000001(SPISettings spi_settings, arduino::MbedSPI *inst = &TFT000001_ILI9341_DEFAULT_SPI_PORT,
                      spi_inst_t *spi_pico_sdk_port = TFT000001_ILI9341_DEFAULT_SPI_PICO_SDK_PORT,
                      int32_t dc=-1, int32_t rst = -1, int32_t cs = -1);
        #else
            TFT000001(SPISettings spi_settings, arduino::MbedSPI *inst = &TFT000001_ILI9341_DEFAULT_SPI_PORT, int32_t dc=-1, 
                      int32_t rst = -1, int32_t cs = -1);
        #endif
    #else
        #ifdef TFT000001_RP2040_SPI_DRAW02_MODE
            TFT000001(SPISettings spi_settings, SPIClass *inst = &TFT000001_ILI9341_DEFAULT_SPI_PORT,
                      spi_inst_t *spi_pico_sdk_port = TFT000001_ILI9341_DEFAULT_SPI_PICO_SDK_PORT,
                      int32_t dc=-1, int32_t rst = -1, int32_t cs = -1);
        #else
            TFT000001(SPISettings spi_settings, SPIClass *inst = &TFT000001_ILI9341_DEFAULT_SPI_PORT, int32_t dc=-1, 
                      int32_t rst = -1, int32_t cs = -1);
        #endif
    #endif
#endif                  

#if defined(TFT000001_ARDUINO_SPI)
    TFT000001(SPISettings spi_settings, SPIClass *inst = &TFT000001_ILI9341_DEFAULT_SPI_PORT, int32_t dc=-1, 
                  int32_t rst = -1, int32_t cs = -1);
#endif

    inline void gpio_dc_on(void);
    inline void gpio_dc_off(void);
    inline void gpio_cs_on(void);
    inline void gpio_cs_off(void);
    inline void gpio_rst_on(void);
    inline void gpio_rst_off(void);
    inline void spi_write(uint8_t byte);
    inline void spi_write16_big(uint16_t word);
    inline void spi_write24_big(uint32_t word);
    inline void TFT000001_digitalWrite(uint8_t port, uint8_t byte);
    void spi_begin(void);
    void spi_end(void);
    inline void writedata(uint8_t byte);
    void writecommand(uint8_t byte);
    void writedata01(uint8_t byte);

    inline void INIT_TFT_DATA_BUS() {
#ifdef TFT000001_RP2040_PIO_8BIT_PARALELL
        pioinit(TFT000001_DIV_UNITS, TFT000001_DIV_FRACT);
#endif
    }

#ifdef TFT000001_RP2040_PIO_8BIT_PARALELL
    void pioinit(uint16_t clock_div, uint16_t fract_div);
#endif

protected:
#ifdef TFT000001_RP2040_PIO_INTERFACE
// This writes 8 bits, then switches back to 16 bit mode automatically
// Have already waited for pio stalled (last data write complete) when DC switched to command mode
// The wait for stall allows DC to be changed immediately afterwards
// Community RP2040 board package by Earle Philhower
    PIO pio = pio0;     // Code will try both pio's to find a free SM
    int8_t pio_sm = 0;  // pioinit will claim a free one
// Updated later with the loading offset of the PIO program.
    uint32_t program_offset  = 0;

// SM stalled mask
    uint32_t pull_stall_mask = 0;

// SM jump instructions to change SM behaviour
    uint32_t pio_instr_jmp16  = 0;
    uint32_t pio_instr_fill  = 0;
    uint32_t pio_instr_addr  = 0;

// SM "set" instructions to control DC control signal
    uint32_t pio_instr_set_dc = 0;
    uint32_t pio_instr_clr_dc = 0;


// Wait for the PIO to stall (SM pull request finds no data in TX FIFO)
// This is used to detect when the SM is idle and hence ready for a jump instruction
#define TFT000001_WAIT_FOR_STALL  pio->fdebug = pull_stall_mask; while (!(pio->fdebug & pull_stall_mask))

// Wait until at least "S" locations free
#define TFT000001_WAIT_FOR_FIFO_FREE(S) while (((pio->flevel >> (pio_sm * 8)) & 0x000F) > (8-S)){}

// Wait until at least 5 locations free
#define TFT000001_WAIT_FOR_FIFO_5_FREE while ((pio->flevel) & (0x000c << (pio_sm * 8))){}

// Wait until at least 1 location free
#define TFT000001_WAIT_FOR_FIFO_1_FREE while ((pio->flevel) & (0x0008 << (pio_sm * 8))){}

// Wait for FIFO to empty (use before swapping to 8 bits)
#define TFT000001_WAIT_FOR_FIFO_EMPTY  while(!(pio->fstat & (1u << (PIO_FSTAT_TXEMPTY_LSB + pio_sm))))

// The write register of the TX FIFO.
#define TFT000001_TX_FIFO  pio->txf[pio_sm]

// Temporary - to be deleted
#define TFT000001_dir_mask 0



#define TFT000001_ILI9486_write_16(C)      pio->sm[pio_sm].instr = pio_instr_jmp16; TFT000001_TX_FIFO = (C); TFT000001_WAIT_FOR_STALL

// Note: the following macros do not wait for the end of transmission

#define TFT000001_ILI9486_write_8(C)     TFT000001_WAIT_FOR_FIFO_FREE(1); TFT000001_TX_FIFO = (C)

/*
#define TFT000001_ILI9486_write_16N(C)    TFT000001_WAIT_FOR_FIFO_FREE(1); TFT000001_TX_FIFO = (C)

#define TFT000001_ILI9486_write_16S(C)    TFT000001_WAIT_FOR_FIFO_FREE(1); TFT000001_TX_FIFO = ((C)<<8) | ((C)>>8)

#define TFT000001_ILI9486_write_32(C)     TFT000001_WAIT_FOR_FIFO_FREE(2); TFT000001_TX_FIFO = ((C)>>16); TFT000001_TX_FIFO = (C)

#define TFT000001_ILI9486_write_32C(C,D)  TFT000001_WAIT_FOR_FIFO_FREE(2); TFT000001_TX_FIFO = (C); TFT000001_TX_FIFO = (D)

#define TFT000001_ILI9486_write_32D(C)    TFT000001_WAIT_FOR_FIFO_FREE(2); TFT000001_TX_FIFO = (C); TFT000001_TX_FIFO = (C)
*/
#endif // TFT000001_RP2040_PIO_INTERFACE



#ifdef TFT000001_TEENSY4x_8BIT_PARALELL
#include <core_pins.h>

inline void T000001_teensy4x_write_8bit_parallel_a_inline(uint8_t c);
inline void T000001_teensy4x_write_8bit_parallel_b_inline(uint8_t c);
void T000001_teensy4x_write_8bit_parallel_a(uint8_t c);
void T000001_teensy4x_write_8bit_parallel_a2(uint16_t c);
void T000001_teensy4x_write_8bit_parallel_a3(uint32_t c);
void T000001_teensy4x_setup_8bit_parallel_a();
void T000001_teensy4x_write_8bit_parallel_b(uint8_t c);
void T000001_teensy4x_write_8bit_parallel_b2(uint16_t c);
void T000001_teensy4x_write_8bit_parallel_b3(uint32_t c);
void T000001_teensy4x_setup_8bit_parallel_b();
void T000001_teensy4x_write_8bit_parallel_null(uint8_t c);
void T000001_teensy4x_write_8bit_parallel_null2(uint16_t c);
void T000001_teensy4x_write_8bit_parallel_null3(uint32_t c);

void (TFT000001::*T000001_teensy4x_write_8bit_parallel)(uint8_t c);
void (TFT000001::*T000001_teensy4x_write_8bit_parallel2)(uint16_t c);
void (TFT000001::*T000001_teensy4x_write_8bit_parallel3)(uint32_t c);
void T000001_teensy4x_setup_8bit_parallel();
#endif  // TFT000001_TEENSY4x_8BIT_PARALELL




#if defined(TFT000001_ARDUINO_UNO_R4_SPI)
R_PORT0_Type *r_port_n_number_cs, *r_port_n_number_dc;
uint32_t r_port_n_pin_mask_cs, r_port_n_pin_mask_not_cs, r_port_n_pin_mask_dc, r_port_n_pin_mask_not_dc;
__IOM uint16_t *r_port_n_podr01_cs, *r_port_n_podr01_dc;
R_PORT0_Type *r_port_n[10] = {R_PORT0, R_PORT1, R_PORT2, R_PORT3, R_PORT4, R_PORT5, R_PORT6, R_PORT7, R_PORT8, R_PORT9};

void digitalWrite_01(uint8_t pin_no, uint8_t value);
void set_pin_no_cs(uint8_t pin_no);
void set_pin_no_dc(uint8_t pin_no);
inline void pin_set_cs(void);
inline void pin_clear_cs(void);
inline void pin_set_dc(void);
inline void pin_clear_dc(void);



#define SPI_WRITE_BUFFER_SIZE_ARDUINO_UNO_R4 1024

uint8_t spi_write_buffer01[SPI_WRITE_BUFFER_SIZE_ARDUINO_UNO_R4];
int32_t spi_write_buffer_pos01 = 0;

inline int32_t spi_buffer_num_arduino_uno_r4();
inline void spi_buffer_write_arduino_uno_r4(uint8_t byte);
void spi_write_buffer_flush_arduino_uno_r4();

#endif





public:
void set_rotation(int rotation);
void set_rotation_ILI9486(int rotation);
void set_rotation_ILI9341(int rotation);
void setRotation(uint8_t mode);
void init(uint32_t bit18_flag = 0, int32_t display_driver = -1);
void init_ILI9486(uint32_t bit18_flag = 0);
void init_ILI9341(void);
void setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
inline void pixel_write16(uint16_t color);
void pixel_write(int32_t x, int32_t y, uint32_t color);
void fill_color16(uint16_t color, uint32_t len);
void fill_color24(uint32_t color, uint32_t len);
void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
void fillRect16(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
void fillRect24(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
void drawPixel(int32_t x, int32_t y, uint32_t color);
void drawPixel16(int32_t x, int32_t y, uint32_t color);
void drawPixel16a(int32_t x, int32_t y, uint32_t color);
void drawRGBBitmap16to16(int32_t x, int32_t y, const uint8_t bitmap[], int32_t w, int32_t h);
void drawPixel24(int32_t x, int32_t y, uint32_t color);
void drawPixel24a(int32_t x, int32_t y, uint32_t color);
void pixel_write24(int32_t x, int32_t y, uint32_t color);
void drawRGBBitmap24to24(int32_t x, int32_t y, const uint8_t bitmap[], int32_t w, int32_t h);

void fillScreenCanvas8(uint8_t color);
void drawPixelCanvas8(int32_t x, int32_t y, uint8_t color);
void drawBitmapCanvas8(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[], 
                       int32_t w, int32_t h, int rotation);
void drawPalette256Bitmap16(int32_t x, int32_t y, const uint8_t bitmap[], uint32_t w, uint32_t h, uint16_t *palette_address = NULL);
void drawPalette256Bitmap24(int32_t x, int32_t y, const uint8_t bitmap[], uint32_t w, uint32_t h, uint32_t *palette_address = NULL);

void invertDisplay(bool i);
void drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color);
void drawFastVLine16(int32_t x, int32_t y, int32_t h, uint32_t color);
void drawFastVLine24(int32_t x, int32_t y, int32_t h, uint32_t color);
void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
void drawFastHLine16(int32_t x, int32_t y, int32_t w, uint32_t color);
void drawFastHLine24(int32_t x, int32_t y, int32_t w, uint32_t color);
void drawBitmap16(int32_t x, int32_t y,const uint8_t bitmap[],
                    int32_t w, int32_t h,
                    uint32_t color, uint32_t bg);
void drawBitmap24(int32_t x, int32_t y,const uint8_t bitmap[],
                    int32_t w, int32_t h,
                    uint32_t color, uint32_t bg);
void drawGrayscaleBitmap(int32_t x, int32_t y,
                       const uint8_t bitmap[],
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
void drawGrayscaleBitmap(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[], 
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
void drawGrayscaleBitmap16(int32_t x, int32_t y,
                       const uint8_t bitmap[],
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
void drawGrayscaleBitmap24(int32_t x, int32_t y,
                       const uint8_t bitmap[],
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
void drawGrayscaleBitmap16(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[], 
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
void drawGrayscaleBitmap24(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[], 
                       int32_t w, int32_t h, bool red_flag, bool green_flag, bool blue_flag, bool invert);
void drawRGBBitmap16to16(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[],
                       int32_t w, int32_t h);
void drawRGBBitmap24to24(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[],
                       int32_t w, int32_t h);

void drawBitmap(int32_t x, int32_t y,const uint8_t bitmap[],
                    int32_t w, int32_t h,
                    uint32_t color, uint32_t bg);
void drawBitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                       int32_t w, int32_t h, uint32_t color);
void drawBitmap(int32_t x, int32_t y, const uint8_t *bitmap01, const uint8_t *bitmap_mask01,
                                  int32_t width, int32_t height,
                                  uint32_t fg, uint32_t bg);

void drawBitmap16(int32_t x, int32_t y, const uint8_t bitmap[],
                       int32_t w, int32_t h, uint32_t color);
void drawBitmap24(int32_t x, int32_t y, const uint8_t bitmap[],
                       int32_t w, int32_t h, uint32_t color);
void drawBitmap16(int32_t x, int32_t y, const uint8_t *bitmap01, const uint8_t *bitmap_mask01,
                                  int32_t width, int32_t height,
                                  uint32_t fg, uint32_t bg);
void drawBitmap24(int32_t x, int32_t y, const uint8_t *bitmap01, const uint8_t *bitmap_mask01,
                                  int32_t width, int32_t height,
                                  uint32_t fg, uint32_t bg);

void drawPalette256Bitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t width, int32_t height);
void drawPalette256Bitmap(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t width, int32_t height);
void drawPalette256Bitmap(int32_t x, int32_t y, const uint8_t bitmap[],
                                     int32_t w, int32_t h, void *palette_address);
void drawPalette256Bitmap(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t w, int32_t h, void *palette_address);

void drawPalette256Bitmap16(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t w, int32_t h, uint16_t *palette_address = NULL);
void drawPalette256Bitmap24(int32_t x, int32_t y, 
                                     const uint8_t bitmap[], const uint8_t bitmap_mask[],
                                     int32_t w, int32_t h, uint32_t *palette_address = NULL);

void drawRGBBitmap16(int32_t x, int32_t y, const uint8_t bitmap[], int32_t w, int32_t h);
void drawRGBBitmap24(int32_t x, int32_t y, const uint8_t bitmap[], int32_t w, int32_t h);
void drawRGBBitmap16(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[],
                       int32_t w, int32_t h);
void drawRGBBitmap24(int32_t x, int32_t y,
                       const uint8_t bitmap[], const uint8_t bitmap_mask[],
                       int32_t w, int32_t h);

void drawRGBBitmap24to16(int32_t x, int32_t y, const uint8_t bitmap[], int32_t w, int32_t h);
void drawRGBBitmap16to24(int32_t x, int32_t y, const uint8_t bitmap[], int32_t w, int32_t h);
void drawRGBBitmap24to16(int32_t x, int32_t y, const uint8_t bitmap[],
                        const uint8_t bitmap_mask[], int32_t w, int32_t h);
void drawRGBBitmap16to24(int32_t x, int32_t y, const uint8_t bitmap[],
                        const uint8_t bitmap_mask[], int32_t w, int32_t h);

inline uint32_t color16to24inline(uint16_t color) {
    uint32_t colr, colg, colb, col;
    colr = ((((uint32_t)color >> 11) & 0x1f) << 3);
    colg = ((((uint32_t)color >> 5) & 0x3f) << 2);
    colb = (((uint32_t)color & 0x1f) << 3);
    colr = (colr != 0) ? (colr | 0x07) : colr;
    colg = (colg != 0) ? (colg | 0x03) : colg;
    colb = (colb != 0) ? (colb | 0x07) : colb;
    col = (colr << 16) | (colg << 8) | colb;
    return col;
}

int32_t createCanvas8(int32_t w, int32_t h);
void deleteCanvas8();

int32_t widthCanvas8();
int32_t heightCanvas8();
uint8_t *getFrameBuffer();

uint32_t get_bit18_flag();

protected:
#if defined(TFT000001_TEENSY4x_ARDUINO_SPI)
    SPIClass *_spi;
    SPISettings _spi_settings;
#endif

#if defined(TFT000001_RP2040_ARDUINO_SPI)
    #ifdef ARDUINO_ARCH_MBED
      arduino::MbedSPI *_spi;
      SPISettings _spi_settings;
    #else
      SPIClass *_spi;
      SPISettings _spi_settings;
    #endif
    #ifdef TFT000001_RP2040_SPI_DRAW02_MODE
      spi_inst_t *_spi_pico_sdk_port;
    #endif
#endif

#if defined(TFT000001_ARDUINO_SPI)
    SPIClass *_spi;
    SPISettings _spi_settings;
#endif

int32_t _tft_dc  = -1;
int32_t _tft_rst = -1;
int32_t _tft_cs  = -1;
int32_t _tft_wr  = -1;
int32_t _tft_d0  =  6;
int32_t _interface;

int32_t _init_width, _init_height;
int32_t _display_driver = 0;
int32_t _width_Canvas8;
int32_t _height_Canvas8;
uint8_t *frame_buffer01 = NULL;
uint32_t _bit18_flag = 0;

};


#endif  // _TFT000001_H





#ifndef _YAMA_2_GFX_H_BETA_GFXCANVAS_H
#define _YAMA_2_GFX_H_BETA_GFXCANVAS_H
 
/// A GFX 1-bit canvas context for graphics
class yama_2_GFX_H_beta_Canvas1 : public yama_2_GFX_H_beta {
public:
  yama_2_GFX_H_beta_Canvas1(uint32_t w, uint32_t h);
  ~yama_2_GFX_H_beta_Canvas1(void);
  void drawPixel(int32_t x, int32_t y, uint32_t color);
  void fillScreen(uint32_t color);
  bool getPixel(int32_t x, int32_t y) const;
  /**********************************************************************/
  /*!
    @brief    Get a pointer to the internal buffer memory
    @returns  A pointer to the allocated buffer
  */
  /**********************************************************************/
  uint8_t *getBuffer(void) const { return buffer; }
 
protected:
  bool getRawPixel(int32_t x, int32_t y) const;
 
private:
  uint8_t *buffer;
 
};
 
/// A GFX 8-bit canvas context for graphics
class yama_2_GFX_H_beta_Canvas8 : public yama_2_GFX_H_beta {
public:
  yama_2_GFX_H_beta_Canvas8(uint32_t w, uint32_t h);
  ~yama_2_GFX_H_beta_Canvas8(void);
  void drawPixel(int32_t x, int32_t y, uint32_t color);
  void fillScreen(uint32_t color);
  void writeFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
  uint8_t getPixel(int32_t x, int32_t y) const;
  /**********************************************************************/
  /*!
   @brief    Get a pointer to the internal buffer memory
   @returns  A pointer to the allocated buffer
  */
  /**********************************************************************/
  uint8_t *getBuffer(void) const { return buffer; }
 
protected:
  uint8_t getRawPixel(int32_t x, int32_t y) const;
 
private:
  uint8_t *buffer;
};
 
///  A GFX 16-bit canvas context for graphics
class yama_2_GFX_H_beta_Canvas16 : public yama_2_GFX_H_beta {
public:
  yama_2_GFX_H_beta_Canvas16(uint32_t w, uint32_t h);
  ~yama_2_GFX_H_beta_Canvas16(void);
  void drawPixel(int32_t x, int32_t y, uint32_t color);
  void fillScreen(uint32_t color);
  void byteSwap(void);
  uint16_t getPixel(int32_t x, int32_t y) const;

  /**********************************************************************/
  /*!
    @brief    Get a pointer to the internal buffer memory
    @returns  A pointer to the allocated buffer
  */
  /**********************************************************************/
  uint8_t *getBuffer(void) const { return (uint8_t *)buffer; }
 
protected:
  uint16_t getRawPixel(int32_t x, int32_t y) const;
 
private:
  uint16_t *buffer;
};

///  A GFX 24-bit canvas context for graphics
class yama_2_GFX_H_beta_Canvas24 : public yama_2_GFX_H_beta {
public:
  yama_2_GFX_H_beta_Canvas24(uint32_t w, uint32_t h);
  ~yama_2_GFX_H_beta_Canvas24(void);
  void drawPixel(int32_t x, int32_t y, uint32_t color);
  void fillScreen(uint32_t color);
  void byteSwap(void);
  uint32_t getPixel(int32_t x, int32_t y) const;

  /**********************************************************************/
  /*!
    @brief    Get a pointer to the internal buffer memory
    @returns  A pointer to the allocated buffer
  */
  /**********************************************************************/
  uint8_t *getBuffer(void) const { return (uint8_t *)buffer; }
 
protected:
  uint32_t getRawPixel(int32_t x, int32_t y) const;
 
private:
  struct yama_2_GFX_H_beta_RGB *buffer;
};

#endif // _YAMA_2_GFX_H_BETA_GFXCANVAS_H
