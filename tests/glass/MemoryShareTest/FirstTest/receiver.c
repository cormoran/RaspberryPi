#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcpy */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<unistd.h>

int main(void)
{
  key_t key = ftok("LCDManager",1);


  int  id;
  char *adr;

  id = shmget(key, 128, IPC_CREAT|0666);
  if(id == -1){
    perror("shmget error");
    exit(-1);
  }

  printf("ID = %d\n",id);

  adr = (char *)shmat(id, NULL, 0);
  if(adr == (void *)-1){
    perror("shmat error");
  } else {
    char strbuf[128]="Initial";
    strcpy(adr,strbuf);

    while(1){
      if(strcmp(adr,strbuf)!=0){
	strcpy(strbuf,adr);
	printf("%s\n",strbuf);

	if (strcmp(strbuf, "end") == 0) {
          break;
	}
      }
      usleep(1000);//1ms                                                                           
    }

    if(shmdt(adr)==-1){
      perror("shmdt error");
    }
  }


  if(shmctl(id, IPC_RMID, 0)==-1){
    perror("shmctl error");
    exit(EXIT_FAILURE);
  }

  return 0;
}
