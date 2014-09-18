#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <math.h>
#include "gl_lcd.h"
#include "flamebuffer.h"
#define rep(i,j) for(i=0;i<j;i++)



int init(){
  //GPIO Init
  if(!bcm2835_init()){
    printf("cannot open bcm");
    return 1;
  }
  init_glcd();
}

int main(int argc,int **argv)
{
    init();

    SET_PIXEL_FORMAT_glcd(0b01010101);//RGB565 16bitモードで送る（フレームバッファ）

    int w=128,h=128;
    uchar *buf,bu[2],rgb[3];
    fb_init(&w,&h);
   
    while(1){
      SET_PAGE_ADDRESS_glcd(0,128-1);
      SET_COLUMN_ADDRESS_glcd(0,128-1);
      WRITE_MEMORY_START_glcd;
      buf=getFrame();
      Sendbytes_glcd(1,buf,128*128*2);
      delay(10);
    }
}
