#ifndef _TFT000001_GFX_CONFIG_H
#define _TFT000001_GFX_CONFIG_H

// TFT000001の設定ファイルです
// 8bitパラレル、SPIの選択やILI9468、ILI9341の選択ができます。



// Raspberry Pi Pico 8bit paralellの場合に宣言
//#define TFT000001_RP2040_PIO_8BIT_PARALELL




// Raspberry Pi Pico SPIの場合に宣言
//#define TFT000001_RP2040_ARDUINO_SPI

    //TFT000001_RP2040_FAST_SPI_MODEをdefineすると直接レジスタアクセスでSPIを操作するモードをOnにします。
    //#define TFT000001_RP2040_FAST_SPI_MODE




// Teensy 4.x SPIの場合に宣言
#define TFT000001_TEENSY4x_ARDUINO_SPI




// Teensy 4.x 8bit paralellの場合に宣言
//#define TFT000001_TEENSY4x_8BIT_PARALELL




// 一般的なArduino互換機のSPIの場合に宣言(AVRのArduinoでは動作しません。)
//#define TFT000001_ARDUINO_SPI



// ILI9486の場合に宣言、ILI9341の場合はコメントアウト
#define TFT000001_ILI9486_DISPLAY




#endif // _TFT000001_GFX_CONFIG_H
