#include "U8glib.h"
#include <stdio.h>

#define I2C_WIDTH 128
#define I2C_HEIGHT 32

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

void drawTagFrame(void) {
  u8g.drawLine(0, 0, I2C_WIDTH - 1, 0);
  u8g.drawLine(0, 31, I2C_WIDTH - 1, 31);
  u8g.drawLine(0, 0, 0, I2C_HEIGHT - 1);
  u8g.drawLine(I2C_WIDTH - 1, 0, I2C_WIDTH - 1, I2C_HEIGHT - 1);
}

void drawTagName(char *name) { u8g.drawStr(5, I2C_HEIGHT - 10, name); }

void drawTagCircleFrame(void) {
  u8g.drawEllipse(I2C_WIDTH - 28, I2C_HEIGHT - 17, 20, 10, U8G_DRAW_ALL);
}

void drawTagPrice(int price) {
  int xCoordinate = 0;

  if (price < 10) {
    xCoordinate = 97;
  } else if (price < 100) {
    xCoordinate = 93;
  } else if (price < 1000) {
    xCoordinate = 89;
  } else {
    xCoordinate = 85;
  }

  char priceStringify[10] = {0};
  snprintf(priceStringify, 10, "%d", price);

  u8g.drawStr(xCoordinate, I2C_HEIGHT - 12, priceStringify);
}

void drawTag(char *tagName, int tagPrice) {
  drawTagFrame();
  drawTagName(tagName);
  drawTagCircleFrame();
  drawTagPrice(tagPrice);
}

void tagSettings(void) {
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  pinMode(8, OUTPUT);

  u8g.setFont(u8g_font_unifont);
}

void testTag(void) {
  static int index = 0;
  int p[] = {8, 88, 888, 8888};

  u8g.firstPage();
  do {
    draw("Eggs", p[index % 4]);
  } while (u8g.nextPage());

  delay(500);
  index += 1;
}

void setup(void) { tagSettings(); }

void loop(void) { testTag(); }
