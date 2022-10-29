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


#define RANDOM_RASPBERRY_WIDTH   TFT000001_TFT_WIDTH
#define RANDOM_RASPBERRY_HEIGHT  TFT000001_TFT_HEIGHT

yama_2_GFX_H_beta_Canvas8 canvas01 = yama_2_GFX_H_beta_Canvas8(RANDOM_RASPBERRY_WIDTH, RANDOM_RASPBERRY_HEIGHT);


void random_raspberry_setup01(void);
void random_raspberry();

void setup() {
  Serial.begin(115200);

  tft.INIT_TFT_DATA_BUS();

  delay(100); 
  Serial.println("\r\nReset:");

  tft.init(0);  // ILI9486の場合1なら18bitカラー、0なら16bitカラー
                // ILI9341は16bitカラーのみ

  tft.set_rotation(0);
  tft.fillRect(0, 0, tft.width(), tft.height(), tft.color(TFT000001_24BIT_COLOR_BLACK));
  canvas01.setWeb216Palette256();
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
    canvas01.fillScreen(TFT000001_WEB216_PALETTE_BLACK);
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
        canvas01.drawRGBBitmap8(raspberry_data01[i].x, raspberry_data01[i].y, raspberry_bitmap01, raspberry_bitmap_mask01, 32, 32);
    }
    tft.drawPalette256Bitmap((tft.width() - RANDOM_RASPBERRY_WIDTH) / 2, (tft.height() - RANDOM_RASPBERRY_HEIGHT) / 2, (uint8_t *)canvas01.getBuffer(), RANDOM_RASPBERRY_WIDTH, RANDOM_RASPBERRY_HEIGHT, canvas01.getPaletteTable256(tft.getColorMode()));
    delay(10);
}

int rnd(int a) {
    return rand() % a;
}

// 32x32 ラズベリーマスクデータ(1ピクセル1ビット MSBが先頭)
const uint8_t PROGMEM raspberry_bitmap_mask01[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0xfe,0x7f,0x80,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,
    0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,0x00,0xff,0xff,0x00,0x00,0x7f,0xfe,0x00,
    0x00,0x7f,0xfe,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,
    0x01,0xff,0xff,0x80,0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,
    0x03,0xff,0xff,0xc0,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,0x01,0xff,0xff,0x80,
    0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x7f,0xfe,0x00,0x00,0x1f,0xf8,0x00,
    0x00,0x07,0xe0,0x00,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// 32x32 ラズベリー8bitピクセル Web216カラー画像ーデータ
const uint8_t PROGMEM raspberry_bitmap01[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x56,0x81,0xac,0x82,0x81,0x81,0x56,0x00,
    0x00,0x56,0x81,0x81,0x82,0xac,0x81,0x56,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xa6,0x7c,0x7c,0x75,0x7c,0x7c,0xd1,0x56,
    0x56,0xd1,0x7c,0x7c,0x75,0x7c,0x7c,0xa6,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0x7c,0x76,0x7c,0x7c,0x75,0x76,0x7c,0xac,
    0xac,0x7c,0x76,0x75,0x7c,0x7c,0x76,0x7c,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xa6,0x75,0x75,0x7c,0x7c,0xa0,0x75,0xd1,
    0xd1,0x75,0x7c,0x7c,0x7c,0x75,0x75,0xa6,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0xac,0x7c,0x75,0x75,0x7c,0xa7,0xa6,0xd7,
    0xd7,0xa6,0xa7,0x7c,0x75,0x75,0x7c,0xac,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xd1,0x7c,0x75,0x75,0x7c,0xd7,0xd7,
    0xd7,0xd7,0x7c,0x75,0x75,0x7c,0xd1,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xac,0xa6,0xa6,0xd1,0xd7,0xb3,
    0xb3,0xd7,0xd1,0xa6,0xa6,0xac,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xb3,0x8e,0x8e,0xb3,0x6a,0x6a,
    0x6a,0x6a,0xb3,0x8e,0x8e,0xb3,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xb3,0x6a,0x6a,0x8e,0xb3,0x3f,0x3f,
    0x3f,0x3f,0x8e,0x8e,0x6a,0x6a,0xb3,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0x6a,0x6a,0xb3,0xd7,0xd7,0xb3,0x8e,
    0x8e,0x8e,0xb3,0xd7,0xb3,0x6a,0x6a,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xac,0xb3,0xb3,0x8e,0x6a,0x6a,0x8e,0xd7,
    0xd7,0x8e,0x6a,0x6a,0x8e,0xb3,0xb3,0xac,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xb3,0xd7,0xb3,0x6a,0x3f,0x3f,0x6a,0xd7,
    0xd7,0x6a,0x3f,0x3f,0x6a,0xb3,0xd7,0xb3,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xb3,0x6a,0xb3,0x6a,0x3f,0x3f,0x3f,0x6a,0xb3,
    0xb3,0x6a,0x3f,0x3f,0x3f,0x6a,0xb3,0x6a,0xb3,0x56,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x81,0x6a,0x3f,0xb3,0x6a,0x3f,0x3f,0x3f,0x6a,0xd7,
    0xd7,0x6a,0x3f,0x3f,0x3f,0x6a,0xb3,0x3f,0x6a,0x81,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x81,0x6a,0x6a,0xb3,0xb3,0x6a,0x3f,0x6a,0x8e,0xd7,
    0xd7,0x8e,0x6a,0x3f,0x6a,0xb3,0xb3,0x6a,0x6a,0x81,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xb3,0x6a,0xd7,0xd7,0xb3,0x8e,0xb3,0x8e,0x6a,
    0x6a,0x8e,0xb3,0xb3,0xb3,0xd7,0xd7,0x6a,0xb3,0x56,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xac,0xb3,0x8e,0x8e,0xd7,0xd7,0x8e,0x3f,0x3f,
    0x3f,0x3f,0x8e,0xd7,0xd7,0x8e,0x8e,0xb3,0xac,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xb3,0x6a,0x3f,0x8e,0xd7,0x6a,0x3f,0x3f,
    0x3f,0x3f,0x6a,0xd7,0x6a,0x3f,0x6a,0xb3,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xb3,0x6a,0x3f,0x6a,0xb3,0x8e,0x3f,0x3f,
    0x3f,0x3f,0x8e,0xb3,0x6a,0x3f,0x6a,0xb3,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xac,0x8e,0x3f,0x3f,0x8e,0xd7,0x6a,0x6a,
    0x6a,0x6a,0xd7,0x8e,0x3f,0x3f,0x8e,0xac,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0xac,0x6a,0x6a,0x8e,0xd7,0xd7,0xb3,
    0xb3,0xd7,0xd7,0x8e,0x6a,0x6a,0xac,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x81,0xd7,0xd7,0xb3,0x6a,0x6a,
    0x6a,0x6a,0x8e,0xd7,0xd7,0x81,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xac,0xb3,0x6a,0x3f,
    0x3f,0x6a,0xb3,0x81,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x88,0x8e,
    0x88,0x88,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x2b,
    0x2b,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
