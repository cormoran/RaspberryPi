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
#include<string>
//#include<sys/ipc.h>
//#include<sys/types.h>


/*
  TFT液晶画面用の独自フレームバッファ
  共有メモリを使用。
  セマフォ使用
 */
#define FBSIZE 128*128*3
class LCDFlamebuffer
{
private :
  key_t key = ftok("LCDManager",1);
  int  id;
  char *adr;
  
public:
  LCDFlamebuffer()
  {
    //全ユーザーにアクセス許可
    if( (id = shmget(key,FBSIZE, IPC_CREAT|0666) )  == -1)
      {
	perror("MyFlamebuffer shmget error");
	exit(1);
      }
    if((adr = (char *)shmat(id, NULL, 0)) == (void *)-1)
      {
	perror("shmat error");
	exit(1);
      }
    memset(adr,128,FBSIZE);//128で初期化
  }

  ~LCDFlamebuffer()
  {
    if(shmdt(adr)==-1){
      perror("shmdt error");
    }
    
    if(shmctl(id, IPC_RMID, 0)==-1){
      perror("shmctl error");
      exit(EXIT_FAILURE);
    }
  }
  
  char *get_FBaddress()
  {
    return adr;
  }
  
  int get_FBsize()
  {
    return FBSIZE;
  }

}

class Semaphore
{
private :
  key_t key = ftok("LCDManager",2);
  int  id;

  Semaphore()
  {
    //セマフォ作成
    if( (id=semget(key,1,IPC_CREAT|0666)) ==-1)
      {
	perror("semget error :");
	exit(1);
      }
  
    //セマフォ初期化(0で)
    union semun{int val;struct semid_ds *buf;unsigned short *array;}ctl_arg;
    unsigned short vals[1]={1};
    ctl_arg.array=vals;
    if(semctl(semid,0,SETALL,ctl_arg)==-1)
      {
	perror("semctl error :");
	exit(1);
      }

  }
  ~Semaphore()
  {

  }


}

int main()
{

  




}
