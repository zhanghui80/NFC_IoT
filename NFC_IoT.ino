#include "ESP8266WiFi.h"
#include <string.h>
#include "NFC.h"
#include "WIFI.h"
#include "OLED.h"

/*
 * 作者：张慧
 * 最后修改时间 2020.9.10
 * 18电子信息工程三班
 */
WIFI wifi;
NFC nfc;
OLED oled;
unsigned long lastMs = 0;   //用于周期性上报

void setup(void) {
  oled.Init_OLED();
  nfc.NFCInit();
  wifi.WifiInit();
}

void loop(void) {

  if(millis() - lastMs >= 5000)
  {
       wifi.Connect_check();
       wifi.MQTT_attribute_post();
       lastMs = millis();
  }
  nfc.WakePN532();     //唤醒
  nfc.buttonState_2 = digitalRead(buttonPin_2);
  if(nfc.buttonState_2 == HIGH && nfc.temp_2 == LOW)
  {
    if (nfc.Searchcard()) 
    {                       //如果寻到卡
      if (nfc.Authenticationkey()) 
      {             //验证身份的第二扇区第二块
        if (nfc.Read_card()) 
        {                 //读卡，第二扇区第二块
          wifi.MQTT_transfer_post();        //将信息上传到服务器
        }
      }
    }
    
  }
  nfc.temp_2 = nfc.buttonState_2;
  wifi.Loop();
}
