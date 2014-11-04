#include"../../../Library/easy_v1/Glcd.h"
#include"../../../Library/easy_v1/FrameBuffer/framebuffer.h"
#include<unistd.h>
#include<iostream>
#include<cstdio>
#include<string.h>
//単色表示を何回か
int main()
{
  Glcd lcd;
  char temp[128*128*2];
  memset(temp,200,128*128*2);
  lcd.MySemop(true);
  lcd.SendData_RGB565(temp,128*128*2);
  sleep(1);
  memset(temp,20,128*128*2);
  lcd.SendData_RGB565(temp,128*128*2);
  sleep(3);
    lcd.MySemop(false);
  return 0;
}
