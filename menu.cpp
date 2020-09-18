#include <Arduino.h>

#include <TFT_eSPI.h>
#include <SPI.h>
#include "menu.h"

extern TFT_eSPI tft;

int Menu(int nbClick) {
  nbClick++;
  switch (nbClick) {
    case 1:
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.fillScreen(TFT_BLACK);
      tft.setTextDatum(MC_DATUM);
      tft.setTextSize(3);
      tft.drawString("Menu", 2, 10);
      tft.setTextSize(2);
      tft.drawString(">IP Address", 2, 50);
      tft.drawString(" item 2", 2, 70);
      tft.drawString(" Sleep", 2, 90);
      tft.drawString(" Config", 2, 110);
      tft.drawString(" C&S block", 2, 130);
      break;
    case 2:
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.fillScreen(TFT_BLACK);
      tft.setTextDatum(MC_DATUM);
      tft.setTextSize(3);
      tft.drawString("Menu", 2, 10);
      tft.setTextSize(2);
      tft.drawString(" IP Address", 2, 50);
      tft.drawString(">item 2", 2, 70);
      tft.drawString(" Sleep", 2, 90);
      tft.drawString(" Config", 2, 110);
      tft.drawString(" C&S block", 2, 130);
      break;
    case 3:
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.fillScreen(TFT_BLACK);
      tft.setTextDatum(MC_DATUM);
      tft.setTextSize(3);
      tft.drawString("Menu", 2, 10);
      tft.setTextSize(2);
      tft.drawString(" IP Address", 2, 50);
      tft.drawString(" item 2", 2, 70);
      tft.drawString(">Sleep", 2, 90);
      tft.drawString(" Config", 2, 110);
      tft.drawString(" C&S block", 2, 130);
      break;
    case 4:
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.fillScreen(TFT_BLACK);
      tft.setTextDatum(MC_DATUM);
      tft.setTextSize(3);
      tft.drawString("Menu", 2, 10);
      tft.setTextSize(2);
      tft.drawString(" IP Address", 2, 50);
      tft.drawString(" item 2", 2, 70);
      tft.drawString(" Sleep", 2, 90);
      tft.drawString(">Config", 2, 110);
      tft.drawString(" C&S block", 2, 130);
      break;
    case 5:
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.fillScreen(TFT_BLACK);
      tft.setTextDatum(MC_DATUM);
      tft.setTextSize(3);
      tft.drawString("Menu", 2, 10);
      tft.setTextSize(2);
      tft.drawString(" IP Address", 2, 50);
      tft.drawString(" item 2", 2, 70);
      tft.drawString(" Sleep", 2, 90);
      tft.drawString(" Config", 2, 110);
      tft.drawString(">C&S block", 2, 130);
      break;
  }
  return nbClick;
}
