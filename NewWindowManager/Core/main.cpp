//WindowManagerのサーバ側

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

#define MSG_APP_ID_START 100

/*
  順序など：全てプロセスIDで管理
 */
typedef struct ApplicatonData
{
  long pid;//process id
  long msgq_id;
  string name;
  string about;
  ApplicatonData(long pid_,long msgq_id_,string name_,string about_="")
  {
    pid=pid_;msgq_id=msgq_id_;name=name_;about=about_;
  }
  bool operator<(const ApplicatonData &right)const
  {
    return pid<right.pid;
  }
  bool operator==(const ApplicatonData &a)const
  {
    return pid==a.pid;
  }
}ApplicatonData;

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

MessageServer MsgSvr;
vector<ApplicatonData> AppList;
long MsgQue_next_id=MSG_APP_ID_START;
long now_using_pid=0;

bool get_Start_msg()
{
  string msg;
  //起動通知受付
  if( (msg=MsgSvr.Mymsgrcv(MSG_APP_START))!="")
    {
      long pid=atol(msg.c_str());//数値が先頭であればOK
      if(pid>0)
	{
	  ApplicatonData newapp(pid,MsgQue_next_id,msg.substr(msg.find(" ",0)+1));
	  AppList.push_back(newapp);
	  string ret=to_string(pid)+" "+to_string(MsgQue_next_id);
	  if(MsgSvr.Mymsgsnd(MSG_SVR_RE,ret,false)==-1)perror("can't send response : ");
	  MsgQue_next_id++;
	  return true;
	}
    }
  return false;
}

bool get_End_msg()
{
  string msg;
  //終了通知受付
  if( (msg=MsgSvr.Mymsgrcv(MSG_APP_END))!="")
    {
      puts("Remove App");
      long pid=atol(msg.c_str());
      if(pid)
	{
	  ApplicatonData temp(pid,0,"");
	  vector<ApplicatonData>::iterator itr=find(AppList.begin(),AppList.end(),temp);
	  if(itr!=AppList.end())
	    {
	      AppList.erase(itr);
	      return true;
	    }
	}
    }
  return false;
}

//現在使用中のをやめさせる
bool prohibit_use()
{
  if(now_using_pid!=0)
    {
      ApplicatonData temp(now_using_pid,0,"");
      vector<ApplicatonData>::iterator itr=find(AppList.begin(),AppList.end(),temp);
      if(itr!=AppList.end())
	{
	  if( MsgSvr.Mymsgsnd(itr->msgq_id,"enduse",false)==-1)return false;
	  now_using_pid=0;
	  return true;
	}
    }
  return false;
}

bool allow_use(int App_pid)
{
  ApplicatonData temp(App_pid,0,"");
  vector<ApplicatonData>::iterator itr=find(AppList.begin(),AppList.end(),temp);
  if(itr!=AppList.end())
    {
      if(MsgSvr.Mymsgsnd(itr->msgq_id,"uselcd",false)==-1)return false;
      now_using_pid=App_pid;
    }
}

int main()
{
  int i;
  for(i=0;i<25;i++)
    {
      get_Start_msg();
      get_End_msg();
      int j;
      for(j=0;j<AppList.size();j++)
	{
	  cout<<"Process ID : "<<AppList[j].pid<<" Queue type ID : "<<AppList[j].msgq_id<<endl;
	}
      puts("");
      sleep(2);
    }
  return 0;
}
