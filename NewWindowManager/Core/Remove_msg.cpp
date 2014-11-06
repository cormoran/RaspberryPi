#include<sys/msg.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<algorithm>
using namespace std;

int main()
{
  int msgid;
  key_t msgkey;

  const char *ftok_path ="./Core_msg";
  const int ftok_id = 1;
  msgkey=ftok(ftok_path,ftok_id);
  if( (msgid=msgget(msgkey,0666))==-1)
    {
      perror("");
      exit(1);
    }
  struct msqid_ds *buf;
  if(msgctl(msgid,IPC_RMID,buf)==-1)
    {
      perror("semctl error");
      exit(1);
    }
  puts("Success to remove");
  return 0;
}
