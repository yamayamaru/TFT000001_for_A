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

unsigned long long testFillScreen();
unsigned long long testLines(uint16_t color);
unsigned long long testFastLines(uint16_t color1, uint16_t color2);
unsigned long long testRects(uint16_t color);
unsigned long long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long long testCircles(uint8_t radius, uint16_t color);
unsigned long long testTriangles();
unsigned long long testFilledTriangles();
unsigned long long testRoundRects();
unsigned long long testFilledRoundRects();
#include <stdarg.h>
char str_buff01[1024];
void printf1(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

//    vsprintf(str_buff01, format, ap);
    vsnprintf(str_buff01, 1024, format, ap);
    Serial.print(str_buff01);
    va_end(ap);
}

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

  printf1("Benchmark                Time (milliseconds)\r\n");
  delay(10);
  printf1("Screen fill              ");
  printf1("%lld\r\n", testFillScreen());
  delay(500);

  printf1("Lines                    ");
  printf1("%lld\r\n", testLines(TFT000001_ILI9341_CYAN));
  delay(500);

  printf1("Horiz/Vert Lines         ");
  printf1("%lld\r\n", testFastLines(TFT000001_ILI9341_RED, TFT000001_ILI9341_BLUE));
  delay(500);

  printf1("Rectangles (outline)     ");
  printf1("%lld\r\n", testRects(TFT000001_ILI9341_GREEN));
  delay(500);

  printf1("Rectangles (filled)      ");
  printf1("%lld\r\n", testFilledRects(TFT000001_ILI9341_YELLOW, TFT000001_ILI9341_MAGENTA));
  delay(500);

  printf1("Circles (filled)         ");
  printf1("%lld\r\n", testFilledCircles(10, TFT000001_ILI9341_MAGENTA));

  printf1("Circles (outline)        ");
  printf1("%lld\r\n", testCircles(10, TFT000001_ILI9341_WHITE));
  delay(500);

  printf1("Triangles (outline)      ");
  printf1("%lld\r\n", testTriangles());
  delay(500);

  printf1("Triangles (filled)       ");
  printf1("%lld\r\n", testFilledTriangles());
  delay(500);

  printf1("Rounded rects (outline)  ");
  printf1("%lld\r\n", testRoundRects());
  delay(500);

  printf1("Rounded rects (filled)   ");
  printf1("%lld\r\n", testFilledRoundRects());
  delay(500);

  printf1("Done!\r\n");

  delay(5000);
}

unsigned long long testFillScreen() {
  unsigned long long start = millis();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_RED));
  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_GREEN));
  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLUE));
  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  yield();
  return millis() - start;
}

unsigned long long testLines(uint16_t color) {
  unsigned long long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = millis();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  t     = millis() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = millis();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  t    += millis() - start;

  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = millis();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  t    += millis() - start;

  yield();
  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = millis();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, tft.color16(color));

  yield();
  return millis() - start;
}

unsigned long long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(TFT000001_ILI9341_BLACK);
  start = millis();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, tft.color16(color1));
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, tft.color16(color2));

  return millis() - start;
}

unsigned long long testRects(uint16_t color) {
  unsigned long long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  n     = yama_2_GFX_H_beta_min(tft.width(), tft.height());
  start = millis();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, tft.color16(color));
  }

  return millis() - start;
}

unsigned long long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(TFT000001_ILI9341_BLACK);
  n = yama_2_GFX_H_beta_min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = millis();
    tft.fillRect(cx-i2, cy-i2, i, i, tft.color16(color1));
    t    += millis() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, tft.color16(color2));
    yield();
  }

  return t;
}

unsigned long long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  start = millis();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, tft.color16(color));
    }
  }

  return millis() - start;
}

unsigned long long testCircles(uint8_t radius, uint16_t color) {
  unsigned long long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = millis();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, tft.color16(color));
    }
  }

  return millis() - start;
}

unsigned long long testTriangles() {
  unsigned long long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  n     = yama_2_GFX_H_beta_min(cx, cy);
  start = millis();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color16(tft.color565(i, i, i)));
  }

  return millis() - start;
}

unsigned long long testFilledTriangles() {
  unsigned long long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  start = millis();
  for(i=yama_2_GFX_H_beta_min(cx,cy); i>10; i-=5) {
    start = millis();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color16(tft.color565(0, i*10, i*10)));
    t += millis() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color16(tft.color565(i*10, i*10, 0)));
    yield();
  }

  return t;
}

unsigned long long testRoundRects() {
  unsigned long long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  w     = yama_2_GFX_H_beta_min(tft.width(), tft.height());
  start = millis();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color16(tft.color565(i, 0, 0)));
  }

  return millis() - start;
}

unsigned long long testFilledRoundRects() {
  unsigned long long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(tft.color16(TFT000001_ILI9341_BLACK));
  start = millis();
  for(i=yama_2_GFX_H_beta_min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color16(tft.color565(0, i, 0)));
    yield();
  }

  return millis() - start;
}
