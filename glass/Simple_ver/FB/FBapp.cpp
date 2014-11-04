#include"../../../Library/easy_v1/Glcd.h"
#include"../../../Library/easy_v1/FrameBuffer/framebuffer.h"
#include<unistd.h>
#include<iostream>
#include<cstdio>

//セマフォ付きのフレームバッファ表示クラス
int main()
{
  Glcd lcd;
  gl_lcd::framebuffer fb;
  while(1)
    {
      lcd.SendData_RGB565(fb.GetFrameAddress(),128*128*2);
      usleep(50000);//50ms
    }
  return 0;


}
