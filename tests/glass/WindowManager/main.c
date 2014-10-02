#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <math.h>
#include "gl_lcd.h"
#include "flamebuffer.h"
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#define rep(i,j) for(i=0;i<j;i++)

int  id;
int init(){
  //GPIO Init
  if(!bcm2835_init()){
    printf("cannot open bcm");
    exit(1);
  }

  init_glcd();

  int w=128,h=128;
  SET_PIXEL_FORMAT_glcd(0b01010101);//RGB565 16bitモードで送る（フレームバッファ）
  fb_init(&w,&h);
  
}

void SendWindow(){
    uchar *buf;
    SET_PAGE_ADDRESS_glcd(0,128-1);
    SET_COLUMN_ADDRESS_glcd(0,128-1);
    WRITE_MEMORY_START_glcd;
    buf=getFrame();
    Sendbytes_glcd(1,buf,128*128*2);
    return;
}

char* setShareMemory(){
  key_t key = ftok("/home/pi/Programs/RaspberryPi/tests/glass/ShareMemoryKeyFile",1);

  char *adr;
  id = shmget(key, 8, IPC_CREAT|/*IPC_EXCL|*/0666);//8byte確保
  if(id == -1){perror("shmget error");exit(-1);}
  adr = (char *)shmat(id, NULL, 0);
  if(adr == (void *)-1){
    perror("shmat error");
    if(shmctl(id, IPC_RMID, 0)==-1){perror("shmctl error");exit(EXIT_FAILURE);}
  }
  memset(adr,0,8);//メモリ初期化
  return adr;
}

int main(int argc,int **argv)
{
  char *adr;
  char flg=0;
  init();
    
  adr=setShareMemory();
  
  while(1){
    if(!(*adr))SendWindow();//共有メモリの1byte目が0ならフレームバッファ送る
    else if (strcmp(adr, "end") == 0) {
      break;
    }
    delay(10);
  }
  
  if(shmdt(adr)==-1){
    perror("shmdt error");
  }
  if(shmctl(id, IPC_RMID, 0)==-1){
    perror("shmctl error");
    exit(EXIT_FAILURE);
  }
  return 0;
}
