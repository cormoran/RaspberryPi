#include<cstdlib>
#include<cstdio>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string>


#define LOCK true
#define UNLOCK false

//ここあとで適切に設定すべし
const char *ftok_path ="./myFB_sem";
const int ftok_id = 3;
const int ftok_id_2 = 4;

const int fb_w 128;
const int fb_h 128;
const int shm_size 128*128*3;

MyFB::MyFB()
{
  fbkey=0;
  //共有メモリ初期化
  {
    shmkey=ftok(ftok_path,ftok_id_2);
    if( (shmid = shmget(key,shm_size, IPC_CREAT|IPC_EXCL|0666)) == -1)
      {
	if( (shmid = shmget(key,shm_size,|0666)) == -1)
	  {
	    perror("Can't make share memory : ");
	    exit(1);
	  }
	Is_Created_shm=false;
      }
    else
      Is_Created_shm=true;

    if( (shmadr = (unsigned char *)shmat(shmid, NULL, 0))==(*void)-1 )
      {
	perror("shmat error : ");
	if(shmctl(id, IPC_RMID, 0)==-1)
	  perror("shmctl error");
	exit(1);
      }
    //自分で共有メモリ作ったなら初期化
    if(Is_Created_sem)
      memset(shmadr,200,shm_size);
  }

  //セマフォ初期化
  {
    semkey=ftok(ftok_path,ftok_id);
    //全ユーザーがアクセス可能な要素１のセマフォ集合を作る(or取得)
    if( (semid=semget(semkey,1,IPC_CREAT|IPC_EXCL|0666)) ==-1)
      {
	//すでに存在していた場合
	Is_Created_sem=false;
	if( (semid=semget(semkey,1,IPC_CREAT|0666)) ==-1)
	  {
	    perror("In MyFB class : semget error :");
	    exit(1);
	  }
      }
    else
      {
	//新規作成できた場合
	Is_Created_sem=true;
	//セマフォ初期化(1で) 
	unsigned short vals[1]={1};
	ctl_arg.array=vals;
	if(semctl(semid,0,SETALL,ctl_arg)==-1)
	  {
	    perror("In MyFB class : semctl error :");
	    exit(1);
	  }
      }
  }
}

MyFB::~MyFB()
 {
   //共有メモリ解放
   {
     if(shmdt(shmadr)==-1){
       perror("shmdt error");
     }
     if(Is_Created_shm)
       {
	 if(shmctl(id, IPC_RMID, 0)==-1){
	   perror("shmctl error");
	   exit(1);
	 }
       }
   }

   //セマフォ解放
   {
     if(!Can_use_fb)//セマフォロックしてない時
       {
	 MySemop(LOCK);
	 //Base::Draw_rectangle(0,0,get_lcd_H(),get_lcd_W(),rgb);
	 MySemop(UNLOCK);
       }
     else //すでにロックしてる時
       //Base::Draw_rectangle(0,0,get_lcd_H(),get_lcd_W(),rgb);
       
       //このインスタンスがセマフォ集合を作ったのなら解放する
       if(Is_Created_sem)
	 {
	   if(semctl(semid,0,IPC_RMID,ctl_arg)==-1)
	     {
	       perror("In MyFB class : semctl rm error");
	       exit(1);
	     }
	 }
   }
  
}

//lock=trueでロック,falseでアンロック
void MyFB::MySemop(bool lock)
{
  struct sembuf    sops[1];
  sops[0].sem_num = 0;          /* セマフォ番号 */
  sops[0].sem_op = lock ? -1 : 1;               /* セマフォ操作 */
  sops[0].sem_flg = 0;                 /* 操作フラグ */
  
  Can_use_fb=lock;
  if (semop(semid,sops,1) == -1) {
    perror("In MyFB class MySemop error");
    exit(1);
  }
   
  return;
}

void MyFB::set_fbkey(long key)
{
  fbkey=key;
}

//エラーなら0返す
//成功なら (x,y)のRedのポインタを返す
unsigned char *get_fb_addr(int x,int y)
{
  if(x<fb_w && y<fb_h && x>=0 && y>=0)
    {
      return shmadr+y*fb_w*3+x*3;
    }
  return 0;
}

  bool write_dot(int x,int y,unsigned char *rgb)
  {
    unsigned char *addr=get_fb_addr(x,y);
    if(addr)
      {
	memset(addr,rgb,3);
	return false;
      }
    return true;
  }

  bool send_rect(int x,int y,int w,int h,unsigned char *rectaddr)
  {
    if(x>=0 && y>=0 && x+w<fb_w && y+h<fb_h)
      { 
	int i,j;
	unsigned char *addr;
	for(i=0;i<h;i++)
	  {
	    if( (addr=get_fb_addr(x,y))!=0 )
	      {
		memset(addr,recaddr,w*3);
		recaddr+=w*3;
	      }
	    else
	      return true;
	  }
	return false;
      }
    return true;
  }
