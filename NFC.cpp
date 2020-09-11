#include "ESP8266WiFi.h"
#include "NFC.h"
#include <string.h>
#include "OLED.h"

OLED oled2;

char NFC::Verify_card[22] = {0X00 , 0X00 , 0XFF , 0X0F , 0XF1 , 0XD4 , 0X40 , 0X01 ,
                             0X60 , 0X0B , 0XFF, 0XFF , 0XFF , 0XFF , 0XFF , 0XFF
                            };     //验证身份指令
char NFC::read_card[12] = {0X00 , 0X00 , 0XFF , 0X05 , 0XFB , 0XD4 ,
                           0X40 , 0X01 , 0X30 , 0X0A , 0XB1 , 0X00
                          };        //预设读第二扇区第二块，块号为10
char NFC::write_card[28] = {0X00 , 0X00 , 0XFF , 0X15 , 0XEB , 0XD4 ,
                            0X40 , 0X01 , 0XA0 , 0X0A , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF ,
                            0XFF , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF , 0XFF ,
                            0X48 , 0X00
                           };
char NFC::card_information[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               };        //要写入卡内的信息存于此
char op[15];


NFC::NFC(void) {        //构造器实现
  buttonState_1 = LOW;
  buttonState_2 = LOW;
  temp_1 = LOW;
  temp_2 = LOW;
}

NFC::~NFC() {

}


/*********初始化函数**********/
void NFC::NFCInit(void) {

  Serial.begin(115200);
  pinMode(buttonPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Motor1,OUTPUT);
  pinMode(Motor2,OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(Motor1,LOW);
  digitalWrite(Motor2,LOW);
}
/***************************/


int NFC::WakePN532(void) {

  int i = 0;
  int state = 1;                                  //用于标记PN532是否唤醒，1为唤醒，0未唤醒
  int readover = 0;                             //用于判断是否读到数据

  buttonState_1 = digitalRead(buttonPin_1);       //按键状态码  =  读GPIO的电平

  delay(5);                                   //延时，防止误按
  if (buttonState_1 == HIGH && temp_1 == LOW) {                //当一号按钮被按下，执行唤醒操作，同时保证按一下只会发一次数据！！
    for (i = 0; i < 24; i++) {
      Serial.print(Wake_PN532[i]);              //发送唤醒指令
      delay(2);
    }
    oled2.Prompt_dispay(3);
  }
  delay(10);                                    //延时，防止接收不到
  temp_1 = buttonState_1;

  i = 0;
  while (Serial.available() > 0) {      //接受反馈
    op[i++] = char(Serial.read());
    delay(2);
    readover = 1;
  }

  if (readover) {
    //Str_to_Characterarray(cache,op);           // 调用String转字符数组函数
    for (i = 0; i < 15; i++) {        //判断是否唤醒
      if (op[i] != PN532_wakeuped[i]) {
        state = 0;
      }
    }
    if (state == 0) {                            //如果唤醒不成功
      digitalWrite(LED, LOW);
      return 0;
    }
    else {                                      //当唤醒后
      digitalWrite(LED, HIGH);
      return 1;
    }
    readover = 0;
  }


}

int NFC::Searchcard(void) {            //寻卡操作函数
  int i;                                        //通用记数变量
  int readover = 0;                             //用于判断是否读到数据
  int read_UID = 0;
  delay(10);

  for (i = 0; i < 11; i++) {          //发送寻卡指令
    Serial.print(Search_card[i]);
    delay(2);
  }



  i = 0;                              //将i置零,后面要用
  delay(10);

  while (Serial.available() > 0) {
    PN532_card_UID[i++] = char(Serial.read());
    delay(2);
    readover ++;

    /*
      要更改判断判断机制，不仅是收到数据，而且是收到完整的数据 ！@！！！！！qaq
    */
  }
  if (!(readover >= 24)) {
    readover = 0;
  }

  if (readover >= 24) {
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    // digitalWrite(LED, HIGH);
    for (i = 0; i < 4; i++) {
      card_UID[i] = PN532_card_UID[i + 19];               //将卡返回数据中的UID部分存入card_UID中
    }
    readover = 0;
    read_UID = 1;
  }
  if (read_UID)
    return 1;
  else
    return 0;

}

int NFC::Authenticationkey(void) {

  int i, sum = 0;
  int readover = 0;

  char cache[16] = {0x00};                    //临时
  for (i = 0; i < 4; i++) {                   //将卡的UID放入验证身份的指令中
    Verify_card[i + 16] = card_UID[i];
  }
  /**这几步用于计算校验位并存入验证身份的指令中***/
  for (i = 5; i < 20 ; i++) {
    sum += Verify_card[i];
  }
  Verify_card[20] = 0x100 - (sum & 0xFF);
  /****************************************/
  /**************(+_+)*********************/

  delay(5);
  for (i = 0; i < 22; i++) {                  //发送验证身份的指令
    Serial.print(Verify_card[i]);
    delay(2);
  }

  delay(20);
  i = 0;
  while (Serial.available() > 0) {
    cache[i++] = char(Serial.read());
    delay(2);
    readover = 1;
  }

  if (readover) {

    if (cache[12] == 0X41 && cache[13] == 0X00) {
      digitalWrite(Buzzer, HIGH);
      delay(500);
      digitalWrite(Buzzer, LOW);
      return 1;
    }
    else {
      return 0;
    }
    readover = 0;
  }


}

int NFC::Write_card(void) {           //写卡

  int i = 0;
  int readover = 0;
  int sum = 0;
  char cache[16] = {0x00};
  for (i = 0; i < 16; i++) {
    write_card[i + 10] = card_information[i + 1];  //将要写的信息拷入写卡命令中
  }
  delay(2);

  for (i = 0; i < 21; i++) {                          //这两步用于计算校验位
    sum += write_card[i + 5];
  }
  write_card[26] = 0x100 - (sum & 0xFF);


  for (i = 0; i < 28; i++) {
    Serial.print(write_card[i]);
    delay(2);
  }

  i = 0;
  while (Serial.available() > 0) {
    cache[i++] = char(Serial.read());
    delay(2);
    readover++;
  }

  if (readover > 15) {
    if (cache[12] == 0x41 && cache[13] == 0x00) {     //验证是否读卡成功
      digitalWrite(Buzzer, HIGH);
      delay(500);                                  //成功则响一秒
      digitalWrite(Buzzer, LOW);
      return 1;
    }
    else {
      return 0;
    }
  }
}

int NFC::Read_card(void) {          //读卡

  char cache[32];
  int i, readover = 0;
  delay(2);
  oled2.Prompt_dispay(1);
  for (i = 0; i < 12; i++) {
    Serial.print(read_card[i]);     //发送读卡指令
    delay(2);
  }

  delay(20);
  i = 0;
  while (Serial.available() > 0) {
    cache[i++] = char(Serial.read());  //接受PN532传回的信息
    delay(2);
    readover ++;
  }
  if (!(readover > 14)) {
    readover = 0;
  }

  if (readover > 14) {
    for (i = 0; i < 16; i++) {                  //将信息存入NFC::card_information中，从第一位存是因为首位有单引号
      NFC::card_information[i] = cache[i + 14];
    }
    readover = 0;
    return 1;
  }
  else
    return 0;

}

void NFC::NFC_operate(void)
{
    
}
