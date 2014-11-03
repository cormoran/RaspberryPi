#include<sys/msg.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<algorithm>

using namespace std;

#define MSG_APP_START 1
#define MSG_APP_END 2
#define MSG_SVR_RE 3
#define MSG_APP_SLEEP 4
#define MSG_APP_MSG 5

class MessageServer
{
private:
  int msgid;
  key_t msgkey;
  bool Is_this_created_msg;
  const char *ftok_path ="./Core_msg";
  const int ftok_id = 1;
public:
  MessageServer()
  {
    //メッセージキュー作成
    msgkey=ftok(ftok_path,ftok_id);
    if( (msgid=msgget(msgkey,IPC_CREAT|IPC_EXCL|0666))==-1)
      {
	Is_this_created_msg=false;
	if( (msgid=msgget(msgkey,IPC_CREAT|0666))==-1)
	  {
	    perror("In MessageServer Class error : ");
	    exit(1);
	  }
      }
    else
      {
	Is_this_created_msg=true;
      }
  }
  ~MessageServer()
  {
    //自分が作ったのなら解放
    if(Is_this_created_msg)
      {
	struct msqid_ds *buf;
	if(msgctl(msgid,IPC_RMID,buf)==-1)
	  {
	    perror("semctl error");
	    exit(1);
	  }
      }
  }

  /* Mymsgsnd
    mtype : メッセージタイプ番号
    msg : メッセージ
    wait : キューがいっぱいの時に待機するかどうか
    戻り値 : 送信成功 0 失敗 -1
   */
  char Mymsgsnd(long mtype,string msg,bool wait)
  {
    struct msgbuf{
      long mtype;
      char mdata[51];
    };
    struct msgbuf msgdata;
    msgdata.mtype=mtype;
    int i;
    for(i=0;i<msg.size();i++)
      msgdata.mdata[i]=msg[i];
    msgdata.mdata[50]='\0';
    if (msgsnd(msgid,&msgdata,msg.size(), wait ? 0 : IPC_NOWAIT) == -1) {
      perror("In MessageServer Class Mymsgsnd error : ");
      return -1;
    }
    return 0;
  }
  
  /*
    msgtype
    0->一番古いメッセージ
    n-> flgにMSG_EXCEPT 指定でタイプn以外の最初
                        なしでタイプnの最初
    MSGNOERROR:受信箱より長いメッセージは一部切り捨てる
    MSG_NOWAIT:指定の型のメッセージがなかったらエラー出す（指定しない場合は来るまで待機）
   */

  //現在の仕様：受信成功したらメッセージ、失敗（メッセージなし）したら""を返す
  string Mymsgrcv(long mtype)
  {
    struct msgbuf{
      long mtype;
      char mdata[51];
    } rcvdata;
    if(msgrcv(msgid,&rcvdata,50,mtype,MSG_NOERROR|IPC_NOWAIT)==-1)
      return "";
    rcvdata.mdata[50]='\0';
    return rcvdata.mdata;
  }

  void ShowInfo()
  {
    struct msqid_ds info;
    msgctl(msgid,IPC_STAT,&info);
    cout<<"max q_bytes : "<<info.msg_qbytes<<endl;
    cout<<"now q_num : "<<info.msg_qnum<<endl;
  }

};

int main()
{
  MessageServer MsgSvr;
  int i=1;
  string snd=to_string(getpid())+" "+"child"+to_string(i);
  //  MsgSvr.shows();
  cout<<snd<<endl;
  MsgSvr.Mymsgsnd(1,snd,false);
  sleep(5);
  snd=to_string(getpid());
  MsgSvr.Mymsgsnd(2,snd,false);
  return 0;
}
