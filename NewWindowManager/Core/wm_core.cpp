/*
  ウィンドウマネージャのコア部分
  ここからLCDライブラリにアクセスする。
  クライアントアプリはこれとやりとりする
 */

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
//#include<sys/ipc.h>
//#include<sys/types.h>
