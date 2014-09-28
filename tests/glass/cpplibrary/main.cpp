#include <cstdio>
#include <cstdlib>
#include <bcm2835.h>
#include <cmath>
#include "gl_lcd.h"
#include "framebuffer.h"
#define rep(i,j) for(i=0;i<j;i++)



int init(){
  //GPIO Init
  if(!bcm2835_init()){
    printf("cannot open bcm");
    return 1;
  }
  return 0;
}

int main(int argc,char **argv)
{
  init();

  gl_lcd::M014C9163SPI glcd;
  gl_lcd::framebuffer fb;
  char *buf;
  glcd.SET_PIXEL_FORMAT((uchar)0b01010101);
  while(1){
      glcd.SET_PAGE_ADDRESS(0,128-1);
      glcd.SET_COLUMN_ADDRESS(0,128-1);
      glcd.WRITE_MEMORY_START();
      buf=fb.GetFrameAddress();
      glcd.Sendbytes(1,buf,128*128*2);

      delay(10);

  }
}
