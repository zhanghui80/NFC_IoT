#include <Arduino.h>
#include <U8g2lib.h>
#include "OLED.h"
#include <math.h>
#include <cstdlib>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


void OLED::Init_OLED(void) {

  u8g2.begin();
  u8g2.enableUTF8Print(); 
}

void OLED::Init_display(int a) {
  if (a == 1) {
  u8g2.setFont(u8g2_font_unifont_t_chinese2);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();

  u8g2.setCursor(25, 15);
  u8g2.print(title);
  u8g2.sendBuffer();
  delay(1000);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 10, wifi);
    u8g2.sendBuffer();
    delay(100);
  }
  if(a == 2) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 10, success);
    u8g2.sendBuffer();
    delay(100);
  }
  if(a == 3) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 10, success);
    u8g2.drawStr(20, 25, "Service connceted!");
    u8g2.sendBuffer();
    delay(100);  
  }
}

void OLED::Prompt_dispay(int a) {

  if(a == 1) {
    u8g2.clearDisplay();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 25, readcard);
    u8g2.sendBuffer();
    delay(100);
  } 

  if(a == 2) {

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 25, "HunanXiangtan");
    u8g2.sendBuffer();
    delay(100);
  }

    if(a == 3) {

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 10, "Maker-IoT");
    u8g2.drawStr(24, 25, "Ready");
    u8g2.sendBuffer();
    delay(100);
  }
    if(a == 4) {

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 10, "Read Sccess");
    u8g2.sendBuffer();
    delay(100);
    }
    if(a == 5) {

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(20, 10, "Upload failed");
    u8g2.sendBuffer();
    delay(100);
  }
}
