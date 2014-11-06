#include "M014C9163SPI/M014C9163SPI.h"
#include"Glcd.h"
#include<cstdlib>
#include<cstdio>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string>


typedef M014C9163SPI Base;
#define LOCK true
#define UNLOCK false

//ここあとで適切に設定すべし
const char *ftok_path ="./Glcd_sem";
const int ftok_id = 1;


Glcd::Glcd()
{
  semkey=ftok(ftok_path,ftok_id);
  
  //全ユーザーがアクセス可能な要素１のセマフォ集合を作る(or取得)
  if( (semid=semget(semkey,1,IPC_CREAT|IPC_EXCL|0666)) ==-1)
    {
      //すでに存在していた場合
      puts("connect");
      Is_Created_sem=false;
      if( (semid=semget(semkey,1,IPC_CREAT|0666)) ==-1)
	{
	  perror("In Glcd class : semget error :");
	  exit(1);
	}
    }
  else
    {
      //新規作成できた場合
      init();//M0...の奴
      Is_Created_sem=true;
      //セマフォ初期化(1で) 
      unsigned short vals[1]={1};
      ctl_arg.array=vals;
      if(semctl(semid,0,SETALL,ctl_arg)==-1)
	{
	  perror("In Glcd class : semctl error :");
	  exit(1);
	}
    }
}

Glcd::~Glcd()
{
  unsigned char rgb[3]={200,200,200};
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Base::Draw_rectangle(0,0,get_lcd_H(),get_lcd_W(),rgb);
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Draw_rectangle(0,0,get_lcd_H(),get_lcd_W(),rgb);
  
  //このインスタンスがセマフォ集合を作ったのなら解放する
  if(Is_Created_sem)
    {
      if(semctl(semid,0,IPC_RMID,ctl_arg)==-1)
	{
	  perror("In Glcd class : semctl rm error");
	  exit(1);
	}
    }
}

void Glcd::Sendbyte(char cmd,uchar data)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Base::Sendbyte(cmd,data);
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Sendbyte(cmd,data);
}

void Glcd::Sendbytes(char cmd,char *data,unsigned int len)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Base::Sendbytes(cmd,data,len);
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Sendbytes(cmd,data,len);
}

void Glcd::Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Base::Draw_rectangle(x,y,w,h,rgb);
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Draw_rectangle(x,y,w,h,rgb);
  
}

void Glcd::SendData_RGB565(char *data,int num)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Base::SendData_RGB565(data,num);
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
      Base::SendData_RGB565(data,num);
}

//lock=trueでロック,falseでアンロック
void Glcd::MySemop(bool lock)
{
  struct sembuf    sops[1];
  sops[0].sem_num = 0;          /* セマフォ番号 */
  sops[0].sem_op = lock ? -1 : 1;               /* セマフォ操作 */
  sops[0].sem_flg = 0;                 /* 操作フラグ */
  
  Can_use_lcd=lock;
  if (semop(semid,sops,1) == -1) {
    perror("In Glcd class MySemop error");
    exit(1);
  }
   
  return;
}
