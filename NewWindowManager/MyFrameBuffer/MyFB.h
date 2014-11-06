#ifndef MYFB_H
#define MYFB_H

#include"M014C9163SPI/M014C9163SPI.h"
#include <sys/types.h>
//#include <sys/ipc.h>

//同時に使用しようとした時は待機になる
class MyFB
{
private:
  union semun{int val;struct semid_ds *buf;unsigned short *array;}ctl_arg;
  bool Can_use_fb;//LCD使用権を持っているか（セマフォの話）
  bool Is_Created_sem;//このインスタンスがセマフォ集合を作ったか
  int semid;
  key_t semkey;

  bool Is_Created_shm;
  int shmid;
  key_t shmkey;
  unsigned char *shmadr;

  long fbkey;//初期値0 これを合わせないと書き込めない
  void MySemop(bool lock);

  unsigned char *get_fb_addr(int x,int y);
public:
  MyFB();
  ~MyFB();
  void set_fbkey(long key);

  inline bool write_dot(int x,int y,unsigned char *rgb);
  bool send_rect(int x,int y,int w,int h,unsigned char *rectaddr);

};

#endif

/*
  ここまでの覚書
  セマフォと共有メモリはできた。あとは共有メモリに書き込むライブラリを増やす＋動作確認する
  これをクライアントメッセージキュークラスに継承させる。
 */

