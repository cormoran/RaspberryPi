#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcpy */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int main(int argc, char *argv[])
{
  int   id;
  char  *adr;
  key_t key =0xffffffff;//ftok("WindowManager",1);

  id = shmget(key,8,0);
  if(id==-1){perror("shmget error ");exit(-1);}

  if(( adr = (char *)shmat(id,0,0)) == (void *)-1) {
    perror("shmat");
  }
  else {

    if(argc<2){
      printf("please type : writer hogehoge\n");
      exit(1);
    }
    if(!strcmp(argv[1],"end")){
	strcpy(adr, argv[1]);
    }
    else{
      *adr=atoi(argv[1]);
    }
    puts("written.");
    if( shmdt(adr) == -1) {
      perror("shmdt");
    }
  }
}
