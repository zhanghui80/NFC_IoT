#ifndef __NFC_H
#define __NFC_H

#define buttonPin_1 D0        //将D0口作为按键1
#define buttonPin_2 D5        //将D5口作为按键2
#define LED D6
#define Buzzer D7             //蜂鸣器
#define Motor1 D4               //电机驱动模块控制
#define Motor2 D8

class NFC {
  private:
    const char Wake_PN532[24] = {0X55 , 0X55 , 0X00 , 0X00 , 0X00 , 0X00 , 0X00 , //唤醒模块指令
                                         0X00 , 0X00 , 0X00 , 0X00 , 0X00 , 0X00 , 0X00 , 0X00 , 0X00 , 0XFF , 0X03 ,
                                         0XFD , 0XD4 , 0X14 , 0X01 , 0X17 , 0X00
                                        };

    const char Search_card[11] = {0X00 , 0X00 , 0XFF , 0X04 , 0XFC ,        //寻卡指令
                                          0XD4 , 0X4A , 0X01 , 0X00 , 0XE1 , 0X00
                                         };

    const char PN532_wakeuped[15] = {0X00 , 0X00 , 0XFF , 0X00 , 0XFF , 0X00 , 0X00 , //PN532唤醒后回复的指令
                                             0X00 , 0XFF , 0X02 , 0XFE , 0XD5 , 0X15 , 0X16 , 0X00
                                            };


    
    char PN532_card_UID[25];                                    //寻卡命令PN532返回的信息存入
    char card_UID[4];                                           //存UID
    static char Verify_card[22];
    static char read_card[12];                                  //读卡指令
    static char write_card[28];                                 //写卡指令
    int WakePN532(void);    //唤醒函数
    int Searchcard(void);     //寻卡操作
    int Authenticationkey(void);    //验证密钥
  public:

    
    void NFCInit(void);   //初始化
    void NFC_operate(void);         //NFC读卡完整操作
    int Write_card(void);           //写卡
    int Read_card(void);            //读卡
    int buttonState_1;               //一号按钮状态
    int buttonState_2;                //二号按键状态
    int temp_1;              //用于储存上一次的按键信息
    int temp_2;
    static char card_information[16];      //储存写卡的信息   

    /**上面两个static变量
     * 如果仅在头文件里声明，没在源文件中写实现
     * 会造成 error: ld returned 1 exit status
     * 
     */
    NFC(void);
    ~NFC();
};

#endif
