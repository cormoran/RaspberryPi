#ifndef GLCD_H
#define GLCD_H

#include"M014C9163SPI/M014C9163SPI.h"
#include <sys/types.h>
//#include <sys/ipc.h>

//セマフォ実装済みのLCDコントロールクラス
//同時に使用しようとした時は待機になる
class Glcd : public M014C9163SPI
{
private:
  union semun{int val;struct semid_ds *buf;unsigned short *array;}ctl_arg;
  bool Can_use_lcd;//LCD使用権を持っているか（セマフォの話）
  bool Is_Created_sem;//このインスタンスがセマフォ集合を作ったか
  int semid;
  key_t semkey;

public:
  Glcd();
  ~Glcd();
  void MySemop(bool lock);  
  void Sendbyte(char cmd,uchar data);
  void Sendbytes(char cmd,char *data,unsigned int len);
  void SendData_RGB666(char *data,int num);
  void SendData_RGB565(char *data,int num);
  void Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb);
};

#endif
