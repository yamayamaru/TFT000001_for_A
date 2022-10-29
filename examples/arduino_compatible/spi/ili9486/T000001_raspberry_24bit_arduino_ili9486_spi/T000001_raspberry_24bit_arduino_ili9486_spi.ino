#include "TFT000001.h"

#ifdef TFT000001_ARDUINO_SPI
  #define TFT_MOSI PIN_SPI_MOSI
  #define TFT_SCK  PIN_SPI_SCK
  #define TFT_CS   PIN_SPI_SS
  #define TFT_MISO PIN_SPI_MISO

  #define TFT_RST  19           // digitalWrite()が使える任意のピンを設定してください。
  #define TFT_DC   18           // digitalWrite()が使える任意のピンを設定してください。
#endif

#ifdef TFT000001_ARDUINO_SPI
    #include "SPI.h"
    #define SPI01 SPI
    const int tft_frequency = 20000000;
    SPISettings spi_settings01 = SPISettings(tft_frequency, MSBFIRST, SPI_MODE0);
#endif

#ifdef TFT000001_ARDUINO_SPI
  TFT000001 tft = TFT000001(spi_settings01, &SPI01, TFT_DC, TFT_RST, TFT_CS);
#endif


#if defined(TFT000001_TEENSY4x_ARDUINO_SPI) || defined(TFT000001_TEENSY4x_8BIT_PARALELL)
  #define RANDOM_RASPBERRY_WIDTH   TFT000001_TFT_WIDTH
  #define RANDOM_RASPBERRY_HEIGHT  TFT000001_TFT_HEIGHT
#else
  #define RANDOM_RASPBERRY_WIDTH   225
  #define RANDOM_RASPBERRY_HEIGHT  300
#endif

yama_2_GFX_H_beta_Canvas24 canvas01 = yama_2_GFX_H_beta_Canvas24(RANDOM_RASPBERRY_WIDTH, RANDOM_RASPBERRY_HEIGHT);


void random_raspberry_setup01(void);
void random_raspberry();

void setup() {
  Serial.begin(115200);

  tft.INIT_TFT_DATA_BUS();

  SPI01.begin();

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
    canvas01.fillScreen(TFT000001_24BIT_COLOR_BLACK);
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
        canvas01.drawRGBBitmap24(raspberry_data01[i].x, raspberry_data01[i].y, raspberry_bitmap01, raspberry_bitmap_mask01, 32, 32);
    }
    tft.drawRGBBitmap24((tft.width() - RANDOM_RASPBERRY_WIDTH) / 2, (tft.height() - RANDOM_RASPBERRY_HEIGHT) / 2, (uint8_t *)canvas01.getBuffer(), RANDOM_RASPBERRY_WIDTH, RANDOM_RASPBERRY_HEIGHT);
    delay(10);
}

int rnd(int a) {
    return rand() % a;
}

// 32x32 ラズベリーマスクデータ(1ピクセル1ビット MSBが先頭)
const uint8_t PROGMEM raspberry_bitmap_mask01[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x7c,0x3e,0x00,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,0x03,0xff,0xff,0xc0,
    0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x00,0x00,0xff,0xff,0x00,
    0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,
    0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x07,0xff,0xff,0xe0,0x07,0xff,0xff,0xe0,
    0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,
    0x01,0xff,0xff,0x80,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x3f,0xfc,0x00,
    0x00,0x0f,0xf0,0x00,0x00,0x07,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// 32x32 ラズベリー24bitピクセルRGB画像ーデータ(R, G, B)
const uint8_t PROGMEM raspberry_bitmap01[] = {
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xf6,0xf7,0xeb,0xea,0xeb,0xe9,0xe8,0xe9,
    0xf2,0xf2,0xf3,0xfa,0xfa,0xfa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xfa,0xfa,0xfa,0xf3,0xf2,0xf3,0xe9,0xe8,0xe9,0xeb,0xea,0xeb,
    0xf8,0xf7,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xdd,0xdd,0xdd,0x8b,0x8c,0x8b,0x53,0x59,0x51,
    0x41,0x4f,0x3b,0x45,0x58,0x3d,0x4b,0x58,0x46,0x5d,0x61,0x5b,0xa1,0xa1,0xa1,
    0xf5,0xf5,0xf5,0xf5,0xf5,0xf5,0xa1,0xa0,0xa1,0x5d,0x61,0x5b,0x4b,0x58,0x46,
    0x45,0x58,0x3d,0x41,0x4f,0x3b,0x59,0x5f,0x56,0x90,0x91,0x90,0xdf,0xdf,0xdf,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6f,0x6c,
    0x29,0x4c,0x1b,0x4f,0x90,0x35,0x5d,0xa7,0x3f,0x63,0xb2,0x43,0x5a,0xa2,0x3c,
    0x4a,0x87,0x31,0x2a,0x46,0x1e,0x94,0x94,0x94,0x92,0x92,0x92,0x2a,0x48,0x1f,
    0x4a,0x87,0x31,0x5a,0xa3,0x3c,0x63,0xb2,0x43,0x5d,0xa7,0x3f,0x50,0x91,0x35,
    0x2a,0x4d,0x1c,0x6e,0x70,0x6d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xf9,0xf9,0xf9,0x52,0x56,0x51,0x4b,0x88,0x32,0x6f,0xc5,0x4c,0x64,0xb1,0x44,
    0x5f,0xaa,0x41,0x6b,0xbe,0x49,0x6f,0xc5,0x4c,0x5d,0xa8,0x3f,0x2e,0x3c,0x28,
    0x2e,0x3c,0x28,0x5e,0xa8,0x40,0x6f,0xc5,0x4c,0x6b,0xbe,0x49,0x5e,0xa8,0x41,
    0x64,0xb2,0x44,0x6f,0xc5,0x4c,0x4a,0x87,0x31,0x52,0x56,0x51,0xf9,0xf9,0xf9,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x71,0x71,0x71,0x2f,0x58,0x1f,
    0x6b,0xbe,0x49,0x6a,0xbd,0x49,0x5c,0xa3,0x3f,0x4d,0x88,0x34,0x5c,0xa4,0x3f,
    0x6d,0xc2,0x4b,0x26,0x44,0x1a,0x26,0x44,0x1a,0x6d,0xc2,0x4b,0x5d,0xa5,0x40,
    0x4d,0x89,0x35,0x5b,0xa2,0x3e,0x6a,0xbd,0x49,0x6b,0xbe,0x49,0x2f,0x57,0x1f,
    0x71,0x71,0x71,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xc0,0xc0,0xc1,0x24,0x2f,0x20,0x50,0x90,0x36,0x6e,0xc3,0x4b,0x6d,0xc2,0x4b,
    0x64,0xb1,0x44,0x47,0x7d,0x30,0x3b,0x68,0x28,0x0c,0x16,0x08,0x0c,0x16,0x08,
    0x3a,0x67,0x28,0x49,0x81,0x32,0x64,0xb1,0x44,0x6d,0xc2,0x4b,0x6e,0xc3,0x4b,
    0x50,0x91,0x36,0x25,0x2f,0x20,0xc1,0xc0,0xc1,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xff,0x89,0x89,0x8a,0x24,0x3b,0x1b,
    0x58,0x9e,0x3c,0x6d,0xc3,0x4b,0x6e,0xc4,0x4c,0x5c,0xa3,0x3f,0x0f,0x1b,0x0a,
    0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x1b,0x0a,0x5c,0xa3,0x3f,0x6e,0xc4,0x4c,
    0x6d,0xc2,0x4b,0x58,0x9d,0x3c,0x25,0x3b,0x1c,0x8f,0x8f,0x90,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xf9,0xf9,0xf9,0x93,0x93,0x93,0x22,0x33,0x1d,0x32,0x5d,0x23,0x32,0x59,0x22,
    0x16,0x2a,0x10,0x0b,0x02,0x04,0x29,0x05,0x10,0x2a,0x05,0x10,0x0c,0x02,0x05,
    0x17,0x2a,0x10,0x32,0x5a,0x23,0x32,0x5c,0x22,0x21,0x32,0x1c,0x93,0x92,0x93,
    0xf9,0xf9,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0xf3,0xf3,0x78,0x75,0x76,0x46,0x11,0x20,
    0x71,0x0c,0x29,0x62,0x0a,0x24,0x30,0x05,0x12,0x8e,0x13,0x35,0xba,0x19,0x46,
    0xbb,0x19,0x46,0x91,0x13,0x36,0x31,0x05,0x12,0x62,0x0b,0x24,0x70,0x0c,0x29,
    0x43,0x10,0x1f,0x78,0x75,0x76,0xf3,0xf3,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x91,0x93,0x92,
    0x49,0x0b,0x1d,0xbc,0x18,0x46,0xc0,0x19,0x48,0x54,0x0b,0x1f,0x4c,0x0a,0x1c,
    0xc4,0x1a,0x4a,0xc9,0x1b,0x4c,0xc9,0x1b,0x4c,0xc9,0x1b,0x4b,0x5a,0x0c,0x22,
    0x54,0x0b,0x1f,0xc0,0x19,0x48,0xbc,0x18,0x46,0x46,0x0b,0x1b,0x95,0x97,0x97,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xf9,0xf9,0xf9,0x50,0x4c,0x4d,0x87,0x0e,0x30,0xaf,0x17,0x42,0x46,0x09,0x1a,
    0x11,0x02,0x06,0x1c,0x04,0x0b,0x4c,0x0a,0x1d,0x74,0x0f,0x2c,0x76,0x10,0x2c,
    0x59,0x0c,0x22,0x26,0x05,0x0e,0x10,0x02,0x06,0x48,0x09,0x1b,0xb1,0x17,0x43,
    0x87,0x0e,0x30,0x50,0x4d,0x4e,0xf9,0xf9,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xea,0xea,0xea,0x37,0x33,0x34,0x4e,0x08,0x1b,
    0x2d,0x06,0x11,0x57,0x0b,0x21,0xa2,0x15,0x3d,0xa7,0x16,0x3f,0x55,0x0b,0x20,
    0x03,0x00,0x01,0x03,0x00,0x01,0x57,0x0c,0x21,0xa6,0x16,0x3e,0xa0,0x15,0x3c,
    0x53,0x0b,0x1f,0x30,0x06,0x12,0x4e,0x08,0x1b,0x37,0x33,0x34,0xea,0xea,0xea,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf4,0xf4,0xf4,0x73,0x70,0x71,
    0x3e,0x0c,0x1a,0x16,0x02,0x08,0x48,0x09,0x1b,0xc0,0x19,0x48,0xc8,0x1a,0x4b,
    0xc8,0x1a,0x4b,0xb5,0x18,0x44,0x22,0x04,0x0d,0x22,0x04,0x0d,0xb6,0x18,0x44,
    0xc8,0x1a,0x4b,0xc8,0x1a,0x4b,0xbf,0x19,0x48,0x44,0x09,0x19,0x15,0x02,0x08,
    0x3c,0x0c,0x19,0x73,0x70,0x71,0xf4,0xf4,0xf4,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0x9b,0x9d,0x9d,0x4e,0x0e,0x20,0xbc,0x18,0x46,0x51,0x0b,0x1e,0x86,0x12,0x32,
    0xc9,0x1a,0x4b,0xc5,0x1a,0x4a,0xc5,0x1a,0x4a,0xbf,0x19,0x48,0x2f,0x06,0x12,
    0x2f,0x06,0x12,0xbf,0x19,0x48,0xc5,0x1a,0x4a,0xc5,0x1a,0x4a,0xc9,0x1a,0x4b,
    0x85,0x11,0x32,0x51,0x0b,0x1e,0xbb,0x18,0x46,0x4a,0x0e,0x1f,0xa0,0xa2,0xa2,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xfc,0xfc,0xfc,0x5b,0x58,0x59,0x84,0x0e,0x2f,0xc8,0x1a,0x4b,
    0x4f,0x0a,0x1e,0x8e,0x13,0x35,0xc9,0x1a,0x4b,0xc5,0x1a,0x4a,0xc7,0x1a,0x4b,
    0xb0,0x17,0x42,0x1d,0x04,0x0b,0x1d,0x04,0x0b,0xb0,0x17,0x42,0xc7,0x1a,0x4b,
    0xc5,0x1a,0x4a,0xc9,0x1a,0x4b,0x8e,0x13,0x35,0x4f,0x0a,0x1e,0xc8,0x1a,0x4b,
    0x83,0x0e,0x2f,0x5d,0x5a,0x5b,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0x58,0x57,0x57,
    0x7c,0x0d,0x2c,0xbe,0x19,0x47,0x2a,0x06,0x10,0x51,0x0b,0x1e,0xbe,0x19,0x48,
    0xc7,0x1a,0x4b,0xbd,0x19,0x47,0x5c,0x0c,0x22,0x0f,0x02,0x06,0x12,0x02,0x07,
    0x62,0x0d,0x25,0xbe,0x19,0x47,0xc7,0x1a,0x4b,0xbe,0x19,0x47,0x50,0x0b,0x1e,
    0x2a,0x06,0x10,0xbe,0x19,0x47,0x7a,0x0c,0x2b,0x5a,0x58,0x59,0xfc,0xfc,0xfc,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0x8d,0x8f,0x8e,0x4c,0x0b,0x1d,0x90,0x13,0x36,0x0c,0x02,0x04,
    0x04,0x01,0x02,0x3a,0x08,0x16,0x52,0x0b,0x1f,0x45,0x09,0x1a,0x68,0x0e,0x27,
    0xa0,0x15,0x3c,0xa4,0x16,0x3e,0x79,0x10,0x2e,0x4f,0x0a,0x1e,0x51,0x0b,0x1f,
    0x38,0x07,0x15,0x04,0x01,0x02,0x0c,0x02,0x04,0x8f,0x13,0x36,0x48,0x0a,0x1c,
    0x93,0x95,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe6,0xe7,0xe7,0x3f,0x38,0x3a,
    0x2b,0x03,0x0e,0x6a,0x0e,0x28,0x5a,0x0c,0x22,0x0f,0x02,0x06,0x00,0x00,0x00,
    0x5b,0x0c,0x22,0xc5,0x1a,0x4a,0xc8,0x1a,0x4b,0xc7,0x1a,0x4b,0xc8,0x1a,0x4b,
    0x6d,0x0e,0x29,0x00,0x00,0x00,0x10,0x02,0x06,0x5d,0x0c,0x23,0x6a,0x0e,0x28,
    0x2b,0x03,0x0e,0x3f,0x38,0x3a,0xe6,0xe7,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0x6d,0x70,0x6f,0x2d,0x02,0x0e,0xc1,0x1a,0x49,0xc7,0x1a,0x4b,
    0x76,0x10,0x2c,0x0b,0x01,0x04,0x88,0x12,0x33,0xc9,0x1a,0x4b,0xc5,0x1a,0x4a,
    0xc5,0x1a,0x4a,0xc8,0x1a,0x4b,0x95,0x14,0x38,0x0e,0x02,0x05,0x7a,0x10,0x2e,
    0xc7,0x1a,0x4b,0xc1,0x1a,0x49,0x2d,0x03,0x0f,0x75,0x78,0x77,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xac,0xad,0xad,0x28,0x0b,0x13,
    0xb3,0x17,0x43,0xc7,0x1a,0x4b,0xc0,0x19,0x48,0x3f,0x08,0x18,0x6d,0x0e,0x29,
    0xca,0x1b,0x4c,0xc6,0x1a,0x4a,0xc6,0x1a,0x4a,0xca,0x1b,0x4c,0x75,0x10,0x2c,
    0x44,0x09,0x1a,0xc0,0x19,0x48,0xc7,0x1a,0x4b,0xb3,0x17,0x43,0x28,0x0b,0x13,
    0xaf,0xb1,0xb0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xec,0xec,0xec,0x3f,0x3b,0x3c,0x74,0x0c,0x29,0xc8,0x1a,0x4b,0xca,0x1b,0x4c,
    0x6e,0x0e,0x29,0x1d,0x04,0x0b,0x8b,0x12,0x34,0xba,0x19,0x46,0xba,0x19,0x46,
    0x8d,0x13,0x35,0x1f,0x04,0x0c,0x71,0x0f,0x2b,0xcb,0x1b,0x4c,0xc8,0x1a,0x4b,
    0x70,0x0c,0x28,0x40,0x3d,0x3e,0xec,0xec,0xec,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xb6,0xb5,0x30,0x1d,0x22,
    0x7a,0x0d,0x2b,0xae,0x17,0x41,0x5c,0x0c,0x22,0x00,0x00,0x00,0x11,0x02,0x06,
    0x3e,0x08,0x17,0x42,0x09,0x19,0x16,0x03,0x08,0x00,0x00,0x00,0x60,0x0d,0x24,
    0xae,0x17,0x41,0x77,0x0c,0x2a,0x2f,0x1d,0x22,0xb7,0xb9,0xb8,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xfe,0xfe,0xfe,0xc2,0xc3,0xc3,0x55,0x50,0x52,0x1a,0x07,0x0c,0x05,0x00,0x01,
    0x3e,0x08,0x17,0x8c,0x13,0x35,0xa5,0x16,0x3e,0xaa,0x16,0x40,0x9b,0x14,0x3a,
    0x52,0x0b,0x1f,0x07,0x00,0x01,0x1a,0x07,0x0c,0x56,0x52,0x53,0xc2,0xc3,0xc3,
    0xfe,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0xef,0xef,
    0xa5,0xa6,0xa6,0x49,0x4b,0x4a,0x4a,0x0d,0x1e,0xb3,0x15,0x42,0xc8,0x1a,0x4b,
    0xc7,0x1a,0x4a,0xad,0x14,0x3f,0x48,0x0d,0x1e,0x4c,0x4d,0x4d,0xa5,0xa6,0xa6,
    0xef,0xef,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf2,0xf2,0xf2,0x8a,0x89,0x89,
    0x4e,0x2a,0x34,0x5a,0x14,0x27,0x56,0x13,0x26,0x45,0x28,0x30,0x8a,0x8a,0x8a,
    0xf2,0xf2,0xf2,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xfd,0xfd,0xfd,0xde,0xe0,0xdf,0xbe,0xc1,0xc0,0xbd,0xc0,0xc0,
    0xde,0xe0,0xdf,0xfd,0xfd,0xfd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
