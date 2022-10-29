#include "TFT000001.h"

#ifdef TFT000001_RP2040_PIO_8BIT_PARALELL
  #define TFT_DC   14
  #define TFT_RST  15
  #define TFT_WR   22

  #define TFT_D0    6
  #define TFT_D1    7
  #define TFT_D2    8
  #define TFT_D3    9
  #define TFT_D4   10
  #define TFT_D5   11
  #define TFT_D6   12
  #define TFT_D7   13
#endif

#ifdef TFT000001_RP2040_PIO_8BIT_PARALELL
  TFT000001 tft = TFT000001(TFT_D0, TFT_WR, TFT_DC, TFT_RST, -1);
#endif


#if defined(TFT000001_TEENSY4x_ARDUINO_SPI) || defined(TFT000001_TEENSY4x_8BIT_PARALELL)
  #define RANDOM_RASPBERRY_WIDTH   TFT000001_TFT_WIDTH
  #define RANDOM_RASPBERRY_HEIGHT  TFT000001_TFT_HEIGHT
#else
  #define RANDOM_RASPBERRY_WIDTH   240
  #define RANDOM_RASPBERRY_HEIGHT  320
#endif

yama_2_GFX_H_beta_Canvas16 canvas01 = yama_2_GFX_H_beta_Canvas16(RANDOM_RASPBERRY_WIDTH, RANDOM_RASPBERRY_HEIGHT);


void random_raspberry_setup01(void);
void random_raspberry();

void setup() {
  Serial.begin(115200);

  tft.INIT_TFT_DATA_BUS();

  delay(100); 
  Serial.println("\r\nReset:");

  tft.init(1);  // ILI9486の場合1なら18bitカラー、0なら16bitカラー
                // ILI9341は16bitカラーのみ

  tft.set_rotation(0);
  tft.fillRect(0, 0, tft.width(), tft.height(), tft.color(TFT000001_24BIT_COLOR_BLACK));
}

void loop() {
    static int r = 0;
    canvas01.setRotation(r);
    random_raspberry_setup01();
    for (int i = 0; i < 200; i++) {
        random_raspberry();
    }

    r++;
    if (r >= 4) r = 0;
}


extern const PROGMEM uint8_t raspberry_bitmap_mask01[];
extern const PROGMEM uint8_t raspberry_bitmap01[];
  
#define RASPBERRY_N   40
#define RDX        10
#define RDY        10

int rnd(int max);

int random_raspberry_width;
int random_raspberry_height;

struct {
    int    x;
    int    y;
    int    dx;
    int    dy;
    int    signx;
    int    signy;
    int    c;
} raspberry_data01[RASPBERRY_N];

void random_raspberry_setup01(void) {

    random_raspberry_width = canvas01.width();
    random_raspberry_height = canvas01.height();

    for (int i = 0; i < RASPBERRY_N; i++) {
        raspberry_data01[i].x = rnd(random_raspberry_width);
        raspberry_data01[i].y = rnd(random_raspberry_height);
        raspberry_data01[i].dx = rnd(RDX) + 1;
        raspberry_data01[i].dy = rnd(RDY) + 1;
        raspberry_data01[i].signx = rnd(2) ? 1 : -1;
        raspberry_data01[i].signy = rnd(2) ? 1 : -1;
    }
}

void random_raspberry() {

    int temp1, temp2;
    canvas01.fillScreen(TFT000001_16BIT_COLOR_BLACK);
    for (int i = 0; i < RASPBERRY_N; i++ ) {
        temp1 = raspberry_data01[i].dx * raspberry_data01[i].signx;
        temp2 = raspberry_data01[i].x + temp1;
        if (temp2 > random_raspberry_width) {
            raspberry_data01[i].signx = -1;
            raspberry_data01[i].dx = rnd(RDX) + 1;
            raspberry_data01[i].x = random_raspberry_width + raspberry_data01[i].signx * raspberry_data01[i].dx;
        } else if (temp2 < 0 ) {
            raspberry_data01[i].signx = 1;
            raspberry_data01[i].dx = rnd(RDX) + 1;
            raspberry_data01[i].x = 0 + raspberry_data01[i].signx * raspberry_data01[i].dx;
        } else {
            raspberry_data01[i].x = raspberry_data01[i].x + temp1;
        } 
        temp1 = raspberry_data01[i].dy * raspberry_data01[i].signy;
        temp2 = raspberry_data01[i].y + temp1;
        if (temp2 > random_raspberry_height) {
            raspberry_data01[i].signy = -1;
            raspberry_data01[i].dy = rnd(RDY) + 1;
            raspberry_data01[i].y = random_raspberry_height + raspberry_data01[i].signy * raspberry_data01[i].dy;
        } else if (temp2 < 0 ) {
            raspberry_data01[i].signy = 1;
            raspberry_data01[i].dy = rnd(RDY) + 1;
            raspberry_data01[i].y = 0 + raspberry_data01[i].signy * raspberry_data01[i].dy;
        } else {
            raspberry_data01[i].y = raspberry_data01[i].y + temp1;
        }

        // ラズベリーのアイコンの描画
        canvas01.drawRGBBitmap16(raspberry_data01[i].x, raspberry_data01[i].y, raspberry_bitmap01, raspberry_bitmap_mask01, 32, 32);
    }
    tft.drawRGBBitmap16((tft.width() - RANDOM_RASPBERRY_WIDTH) / 2, (tft.height() - RANDOM_RASPBERRY_HEIGHT) / 2, (uint8_t *)canvas01.getBuffer(), RANDOM_RASPBERRY_WIDTH, RANDOM_RASPBERRY_HEIGHT);
    delay(10);
}

int rnd(int a) {
    return rand() % a;
}

// 32x32 ラズベリーマスクデータ(1ピクセル1ビット MSBが先頭)
const uint8_t PROGMEM raspberry_bitmap_mask01[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x7c,0x3e,0x00,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,0x03,0xff,0xff,0xc0,
0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,
0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,
0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,
0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,
0x01,0xff,0xff,0x80,0x00,0xff,0xff,0x00,0x00,0x7f,0xfe,0x00,0x00,0x3f,0xfc,0x00,
0x00,0x0f,0xf0,0x00,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


// 32x32 ラズベリー16bitピクセルRGB565画像ーデータ(リトルエンディアン)
// (drawRGBBitmap16で使う画像データは2バイトアラインメント境界に合わせないと正常に動作しないときがあります。)
const uint8_t PROGMEM __attribute__ ((aligned(2))) raspberry_bitmap01[] = {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xbe,0xf7,0x5d,0xef,0x5d,0xef,0x9e,0xf7,0xdf,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xdf,0xff,0x9e,0xf7,0x5d,0xef,0x5d,0xef,0xbf,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,0xde,
0x71,0x8c,0xca,0x52,0x67,0x42,0xc7,0x42,0xc8,0x4a,0x0b,0x5b,0x14,0xa5,0xbe,0xf7,
0xbe,0xf7,0x14,0xa5,0x0b,0x5b,0xc8,0x4a,0xc7,0x42,0x67,0x42,0xea,0x5a,0x92,0x94,
0xfb,0xde,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,
0x63,0x2a,0x86,0x4c,0x27,0x5d,0x88,0x65,0x07,0x5d,0x26,0x4c,0x23,0x2a,0xb2,0x94,
0x92,0x94,0x43,0x2a,0x26,0x4c,0x07,0x5d,0x88,0x65,0x27,0x5d,0x86,0x54,0x63,0x2a,
0x8d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdf,0xff,0xaa,0x52,
0x46,0x4c,0x29,0x6e,0x88,0x65,0x48,0x5d,0xe9,0x6d,0x29,0x6e,0x47,0x5d,0xe5,0x29,
0xe5,0x29,0x48,0x5d,0x29,0x6e,0xe9,0x6d,0x48,0x5d,0x88,0x65,0x29,0x6e,0x26,0x4c,
0xaa,0x52,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8e,0x73,
0xc3,0x2a,0xe9,0x6d,0xe9,0x6d,0x07,0x5d,0x46,0x4c,0x27,0x5d,0x09,0x6e,0x23,0x22,
0x23,0x22,0x09,0x6e,0x28,0x5d,0x46,0x4c,0x07,0x5d,0xe9,0x6d,0xe9,0x6d,0xa3,0x2a,
0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x18,0xc6,
0x64,0x21,0x86,0x54,0x09,0x6e,0x09,0x6e,0x88,0x65,0xe6,0x43,0x45,0x3b,0xa1,0x08,
0xa1,0x08,0x25,0x3b,0x06,0x4c,0x88,0x65,0x09,0x6e,0x09,0x6e,0x86,0x54,0x64,0x21,
0x18,0xc6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x51,0x8c,0xc3,0x21,0xe7,0x5c,0x09,0x6e,0x29,0x6e,0x07,0x5d,0xc1,0x08,0x00,0x00,
0x00,0x00,0xc1,0x08,0x07,0x5d,0x29,0x6e,0x09,0x6e,0xe7,0x5c,0xc3,0x21,0x72,0x8c,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xdf,0xff,0x92,0x94,0x83,0x21,0xe4,0x32,0xc4,0x32,0x42,0x11,0x00,0x08,0x22,0x28,
0x22,0x28,0x00,0x08,0x42,0x11,0xc4,0x32,0xe4,0x32,0x83,0x21,0x92,0x94,0xdf,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x9e,0xf7,0xae,0x7b,0x84,0x40,0x65,0x70,0x44,0x60,0x22,0x30,0x86,0x88,0xc8,0xb8,
0xc8,0xb8,0x86,0x90,0x22,0x30,0x44,0x60,0x65,0x70,0x83,0x40,0xae,0x7b,0x9e,0xf7,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x92,0x94,0x43,0x48,0xc8,0xb8,0xc9,0xc0,0x43,0x50,0x43,0x48,0xc9,0xc0,0xc9,0xc8,
0xc9,0xc8,0xc9,0xc8,0x64,0x58,0x43,0x50,0xc9,0xc0,0xc8,0xb8,0x43,0x40,0xb2,0x94,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdf,0xff,
0x69,0x52,0x66,0x80,0xa8,0xa8,0x43,0x40,0x00,0x10,0x21,0x18,0x43,0x48,0x65,0x70,
0x85,0x70,0x64,0x58,0x21,0x20,0x00,0x10,0x43,0x48,0xa8,0xb0,0x66,0x80,0x69,0x52,
0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5d,0xef,
0x86,0x31,0x43,0x48,0x22,0x28,0x44,0x50,0xa7,0xa0,0xa7,0xa0,0x44,0x50,0x00,0x00,
0x00,0x00,0x64,0x50,0xa7,0xa0,0xa7,0xa0,0x43,0x50,0x22,0x30,0x43,0x48,0x86,0x31,
0x5d,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0xf7,0x8e,0x73,
0x63,0x38,0x01,0x10,0x43,0x48,0xc9,0xc0,0xc9,0xc8,0xc9,0xc8,0xc8,0xb0,0x21,0x20,
0x21,0x20,0xc8,0xb0,0xc9,0xc8,0xc9,0xc8,0xc9,0xb8,0x43,0x40,0x01,0x10,0x63,0x38,
0x8e,0x73,0xbe,0xf7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0x64,0x48,
0xc8,0xb8,0x43,0x50,0x86,0x80,0xc9,0xc8,0xc9,0xc0,0xc9,0xc0,0xc9,0xb8,0x22,0x28,
0x22,0x28,0xc9,0xb8,0xc9,0xc0,0xc9,0xc0,0xc9,0xc8,0x86,0x80,0x43,0x50,0xc8,0xb8,
0x63,0x48,0x14,0xa5,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcb,0x5a,0x65,0x80,
0xc9,0xc8,0x43,0x48,0x86,0x88,0xc9,0xc8,0xc9,0xc0,0xc9,0xc0,0xa8,0xb0,0x21,0x18,
0x21,0x18,0xa8,0xb0,0xc9,0xc0,0xc9,0xc0,0xc9,0xc8,0x86,0x88,0x43,0x48,0xc9,0xc8,
0x65,0x80,0xcb,0x5a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0x5a,0x65,0x78,
0xc8,0xb8,0x22,0x28,0x43,0x50,0xc9,0xb8,0xc9,0xc0,0xc8,0xb8,0x64,0x58,0x00,0x08,
0x00,0x10,0x64,0x60,0xc8,0xb8,0xc9,0xc0,0xc8,0xb8,0x43,0x50,0x22,0x28,0xc8,0xb8,
0x65,0x78,0xcb,0x5a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x71,0x8c,0x43,0x48,
0x86,0x90,0x00,0x08,0x00,0x00,0x42,0x38,0x43,0x50,0x43,0x40,0x64,0x68,0xa7,0xa0,
0xa7,0xa0,0x85,0x78,0x43,0x48,0x43,0x50,0x22,0x38,0x00,0x00,0x00,0x08,0x86,0x88,
0x43,0x48,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3c,0xe7,0xc7,0x39,
0x01,0x28,0x65,0x68,0x64,0x58,0x00,0x08,0x00,0x00,0x64,0x58,0xc9,0xc0,0xc9,0xc8,
0xc9,0xc0,0xc9,0xc8,0x65,0x68,0x00,0x00,0x00,0x10,0x64,0x58,0x65,0x68,0x01,0x28,
0xc7,0x39,0x3c,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8d,0x6b,
0x01,0x28,0xc9,0xc0,0xc9,0xc0,0x85,0x70,0x00,0x08,0x86,0x88,0xc9,0xc8,0xc9,0xc0,
0xc9,0xc0,0xc9,0xc8,0xa7,0x90,0x00,0x08,0x85,0x78,0xc9,0xc0,0xc9,0xc0,0x01,0x28,
0xce,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x75,0xad,
0x42,0x28,0xa8,0xb0,0xc9,0xc0,0xc9,0xc0,0x43,0x38,0x65,0x68,0xc9,0xc8,0xc9,0xc0,
0xc9,0xc0,0xc9,0xc8,0x85,0x70,0x43,0x40,0xc9,0xc0,0xc9,0xc0,0xa8,0xb0,0x42,0x28,
0x96,0xad,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7d,0xef,
0xc7,0x39,0x65,0x70,0xc9,0xc8,0xc9,0xc8,0x65,0x68,0x21,0x18,0x86,0x88,0xc8,0xb8,
0xc8,0xb8,0x86,0x88,0x21,0x18,0x65,0x70,0xc9,0xc8,0xc9,0xc8,0x65,0x70,0xe7,0x41,
0x7d,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xb6,0xb5,0xe4,0x30,0x65,0x78,0xa8,0xa8,0x64,0x58,0x00,0x00,0x00,0x10,0x42,0x38,
0x43,0x40,0x01,0x10,0x00,0x00,0x64,0x60,0xa8,0xa8,0x65,0x70,0xe4,0x28,0xd7,0xb5,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x18,0xc6,0x8a,0x52,0x21,0x18,0x00,0x00,0x42,0x38,0x86,0x88,0xa7,0xa0,
0xa8,0xa8,0xa7,0x98,0x43,0x50,0x00,0x00,0x21,0x18,0x8a,0x52,0x18,0xc6,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0x7d,0xef,0x34,0xa5,0x49,0x4a,0x63,0x48,0xa8,0xb0,0xc9,0xc8,
0xc9,0xc0,0xa7,0xa8,0x63,0x48,0x69,0x4a,0x34,0xa5,0x7d,0xef,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x9e,0xf7,0x51,0x8c,0x46,0x49,0xa4,0x58,
0x84,0x50,0x46,0x41,0x51,0x8c,0x9e,0xf7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x1b,0xdf,0x18,0xbe,
0x18,0xbe,0x1b,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
