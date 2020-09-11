#ifndef __OLED_H
#define __OLED_H

#define SCL D1

#define SDA D2


class OLED{

    private:

    const char *title = "物流系统";


  public:
     const char *wifi  = "WiFi connecting...";
     const char *success = "WiFi connected!";
     const char *readcard = "read Card....";
     const char *writecard =   "正在写卡";
    void Init_OLED(void);
    void Init_display(int);        //在屏幕上显示出各种初始化的信息,1为wifi连接,2为服务器连接
    void Prompt_dispay(int);        //在屏幕上显示出提示信息
    void Info_display(void);       //在屏幕上显示出快递信息

};
#endif
