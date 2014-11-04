#include<sys/sem.h>
#include<cstdio>
#include<cstdlib>
const char *ftok_path ="./Glcd_sem";
const int ftok_id = 1;


int main()
{
  int semid;
  key_t semkey;
  semkey=ftok(ftok_path,ftok_id);
  if( (semid=semget(semkey,1,0666)) ==-1)
    {
      perror("error : ");
      exit(1);
    }
  if(semctl(semid,0,IPC_RMID)==-1)
    {
      perror("error :");
      exit(1);
    }
  puts("Success to remove sem");
  return 0;
}
