#include <ESP8266WiFi.h>
#include "WIFI.h"
#include <string.h>
#include "NFC.h"
#include "OLED.h"

/*
 * @auther zhanghui
 * 2019/11/26
 */
OLED oled1;
WiFiClient espClient;
PubSubClient  client(espClient);

/****监听云端下发指令并处理****/
void callback(char *topic, byte *payload, unsigned int length)
{
  //  Serial.print("Message arrived [");
  //  Serial.print(topic);
  //  Serial.print("] ");
    payload[length] = '\0';
  //  Serial.println((char *)payload);
}

/************初始化部分********************/
void WIFI::WiFi_Connect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }  

  WiFi.setAutoConnect(true);          //设置自动重连
  client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接WiFi之后，连接MQTT服务器 */
  client.setCallback(callback);
}

void WIFI::MQTT_check_connect()
{
  while (!client.connected())
    {
      //Serial.println("Connecting to MQTT Server ...");
      if (client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))
      {
        //Serial.println("MQTT Connected!");
      }
      else
      {
       // Serial.print("MQTT Connect err:");
      //  Serial.println(client.state());
        delay(5000);
      }
    }
}

void WIFI::WifiInit(void)
{
  WiFi_Connect();
  MQTT_check_connect();
}
/********************************************************/

void WIFI::Connect_check(void)
{
  if(WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
    while(WiFi.status() != WL_CONNECTED)
    {
      delay(100);  
    }
  }
  WIFI::MQTT_check_connect();
}

boolean WIFI::MQTT_attribute_post(void)
{
  char param[128];
  char jsonBuf[512];
  sprintf(param,"{\"DeviceType\":\"%s\",\"Device_site\":\"%s\",}",Device_type,Device_site);
  sprintf(jsonBuf,ALINK_BODY_FORMAT,ALINK_METHOD_PROP_POST,param);
 // Serial.println(jsonBuf);
  boolean state = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf); //发布操作，向指定Topic上报
 // if(state)
 //   Serial.println("上报成功！");
 // else
 //   Serial.println("上报失败！");
    return state; 
}

#ifdef TRANSFER

void WIFI::MQTT_transfer_post(void)
{
  char param[256];
  char jsonBuf[512];
  sprintf(param,"{\"Express_info\":\"%s\",}",NFC::card_information);
  sprintf(jsonBuf,ALINK_BODY_FORMAT,ALINK_METHOD_EXPRESS_POST,param);
 // Serial.println(jsonBuf);
  boolean state = client.publish(ALINK_TOPIC_EXPRESS_POST,jsonBuf);
  
  if(state)
    oled1.Prompt_dispay(4);
  else
    oled1.Prompt_dispay(5);   
}

#else
void WIFI::MQTT_out_post(void)         //出库事件发布
{
  char param[128];
  char jsonBuf[512];
  sprintf(param,"{\"Express_info\":\"%s\",}",NFC::card_information);
  sprintf(jsonBuf,ALINK_BODY_FORMAT,ALINK_METHOD_OUT_POST,param);
  //Serial.println(jsonBuf);
  boolean state = client.publish(ALINK_TOPIC_OUT_POST,jsonBuf);
  //if(state)
  //  Serial.println("上报成功！");
  //else
  //  Serial.println("上报失败！");  
}
#endif

void WIFI::Loop(void)
{
  client.loop();
  delay(10);
}
