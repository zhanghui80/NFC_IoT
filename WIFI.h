#ifndef __WIFI_H
#define __WIFI_H

#include <PubSubClient.h>
#include <ArduinoJson.h>

#define TRANSFER

/* 设备的三元组信息*/
#define PRODUCT_KEY      "a1Kw2foRVQ8"
#define DEVICE_NAME       "NFC_transfer"
#define DEVICE_SECRET     "732f185d11e63912d527df964c3ec32a"
#define REGION_ID         "cn-shanghai"
/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER       PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME "&" PRODUCT_KEY

/***********/

/*客户端ID和密码*/
#define CLIENT_ID         "esp1|securemode=3,signmethod=hmacsha1,timestamp=789|"
/***（productKey,deviceName,timestamp,clientId）, 按照字母顺序排序, 
然后将参数值依次拼接。比如明文clientIdesp1deviceNameNFC_transferproductKeya1Kw2foRVQ8timestamp789，
秘钥就是DEVICE_SECRET***/
#define MQTT_PASSWD       "822b49ec26e1f8af8818a3720258158ca369fc73"


//MQTT订阅和发布所需的主题和方法
#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_TOPIC_EXPRESS_POST  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/Express_rec/post"
#define ALINK_TOPIC_OUT_POST  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/Out_info_rec/post"

#define ALINK_METHOD_PROP_POST      "thing.event.property.post"
#define ALINK_METHOD_PROP_POSTRSP   "thing.event.property.post_reply"
#define ALINK_METHOD_PROP_SET       "thing.service.property.set"
#define ALINK_METHOD_EXPRESS_POST   "thing.event.Express_rec.post"
#define ALINK_METHOD_OUT_POST       "thing.event.Out_info_rec.post"



class WIFI {
  
  private:  

    const char *WIFI_SSID = "10032";
    const char *WIFI_PASSWD = "1234567890aa";
    const char *Device_type = "NodeMCU";
    const char *Device_site = "湖南科技大学南校";
    void WiFi_Connect(void);    //连接WiFi
    void MQTT_check_connect(void);      //登录物联网平台 
    


  public:
    boolean MQTT_attribute_post(void);    //主题内容上报
    void MQTT_transfer_post(void);    //中转事件发布
    void MQTT_out_post(void);         //出库事件发布
    void WifiInit(void);
    void Connect_check(void);
    void Loop(void);
     
};


#endif
