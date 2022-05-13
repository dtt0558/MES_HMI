/*
 Name:    MES_HMI 上位机数据转发
 Created: 2022/5/13 10:09:08
 Author:  dtt0558@126.com
 Desp:使用Arduino Mega 2560主板
*/
#include<string.h>
#include<stdio.h>
//定义串口名称
#define EM Serial1
#define Scaner Serial2
#define TJC Serial3
//定义三色灯引脚
int redLED = 3; //红色LED 接数字引脚3
int greenLED = 4; //红色LED 接数字引脚3
int blueLED = 5; //红色LED 接数字引脚3

void setup() {

  //根据自己的实际改为对应的串口
  Serial.begin(115200);//自带串口
  EM.begin(115200);//电测机
  Scaner.begin(9600);//条码枪
  TJC.begin(115200);//大屏
  //定义三色灯引脚输出
  pinMode(redLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(blueLED,OUTPUT);  
}
void loop() {

  //扫码枪数据转发本机串口0
  char sn[100] = ""; //定义字符数组，接受来自上位机的数据
  int i = 0;
  //while (!Scaner.available()); //等待数据传送过来，若没有数据，一直等待，即执行到本行不向下执行
  while (Scaner.available()) //当发现缓存中有数据时，将数据送至字符数组a中
  {
    sn[i] = Scaner.read();
    i++;
    delay(3);
  }
  if(strlen(sn)>0)
  {
	//发送条码给PC
	Serial.println(sn);
	//同时将条码发给串口屏
    char reslut[100];
    sprintf(reslut,"t0.txt=\"%s\"",sn); 
    int len = strlen(reslut);
    TJC.write(reslut);
    SendEnd();
  }
  
  //电测机数据转发本机串口0
  char EMdata[1000] = ""; //定义字符数组，接受来自上位机的数据
  int e = 0;
  while (EM.available()) //当发现缓存中有数据时，将数据送至字符数组a中
  {
    EMdata[e] = EM.read();
    e++;
    delay(3);
  }
  if(strlen(EMdata)>0)
  {
     Serial.println(EMdata);
  }

  //PC数据转发本机串口0
  char PCdata[1000] = ""; //定义字符数组，接受来自上位机的数据
  int p = 0;
  while (Serial.available()) //当发现缓存中有数据时，将数据送至字符数组a中
  {
    PCdata[p] = Serial.read();
    p++;
    delay(3);
  }
  if(strlen(PCdata)>0)
  {
    //这里区分PC机发过来的命令，分别控制串口屏和单片机
    TJC.write(PCdata);
  }

  
}



//这里写串口事件
void serialEvent()
{
//   Serial.write("serialEvent.....");
//   delay(1000);   
}

//淘晶驰命令结尾
void SendEnd()
{
  TJC.write(0xff);
  TJC.write(0xff);
  TJC.write(0xff);
}