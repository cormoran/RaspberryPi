#include "M014C9163SPI/M014C9163SPI.h"

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
  
  if( (semid=semget(semkey,1,IPC_CREAT|0666)) ==-1)//全ユーザーがアクセス可能な要素１のセマフォ集合を作る(or取得)
    {
      perror("In Glcd class : semget error :");
      exit(1);
    }
  //セマフォ初期化(1で)
  union semun{int val;struct semid_ds *buf;unsigned short *array;}ctl_arg;
  unsigned short vals[1]={1};
  ctl_arg.array=vals;
  if(semctl(semid,0,SETALL,ctl_arg)==-1)
    {
      perror("In Glcd class : semctl error :");
      exit(1);
    }
}

Glcd::~Glcd()
{
  if(semctl(semid,0,IPC_RMID,ctl_arg)==-1)
    {
      perror("In Glcd class : semctl error");
      exit(1);
    }
}

void Glcd::Sendbyte(char cmd,uchar data)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Can_use_lcd=true;
      Base::Sendbyte(cmd,data);
      Can_use_lcd=false;
      Mysemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Sendbyte(cmd,data);
}

void Glcd::Sendbytes(char cmd,char *data,unsigned int len)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Can_use_lcd=true;
      Base::Sendbytes(cmd,data,len);
      Can_use_lcd=false;
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Sendbyte(cmd,data,len);
}

void Glcd::Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Can_use_lcd=true;
      Base::Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb);
      Can_use_lcd=false;
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb);
  
}

void Glcd::SendData_RGB565(char *data,int num)
{
  if(!Can_use_lcd)//LCDセマフォロックしてない時
    {
      MySemop(LOCK);
      Can_use_lcd=true;
      Base::SendData_RGB565(data,num);
      Can_use_lcd=false;
      MySemop(UNLOCK);
    }
  else //すでにロックしてる時
    Base::Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb);
}

//lock=trueでロック,falseでアンロック
void Glcd::MySemop(bool lock)
{
  struct sembuf    sops[1];
  sops[0].sem_num = 1;          /* セマフォ番号 */
  sops[0].sem_op = lock ? -1 : 1;               /* セマフォ操作 */
  sops[0].sem_flg = 0;                 /* 操作フラグ */
  
  if (semop(semid,sops,1) == -1) {
    perror("In Glcd class MySemop error");
    exit(1);
  }
   
  return;
}
