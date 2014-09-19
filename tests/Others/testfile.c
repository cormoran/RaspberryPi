#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <math.h>
#include "gl_lcd.h"

#define rep(i,j) for(i=0;i<j;i++)

int init(){
  //GPIO Init
  if(!bcm2835_init())
    return 1;
  init_glcd();
}

int main(int argc,int **argv)
{
  init();
  
  //charatest();

  // @ Show_rawimage("uc?id=0B00BTPHYTcDgR3pCelpOSGJWV0k",128,128);
  Show_rawimage("./three.raw",128,128);
  return 0;
}
