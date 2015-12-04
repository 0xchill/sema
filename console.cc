# include "helper.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int shmid;

void clean_up(int sig) 
{
  int res;

  printf("Received signal: %i\n",sig);
  res = shmctl(shmid, IPC_RMID, NULL);
  if(res != 0)
  {
    printf("Failed to free shared memory! Got %i from shmctl\n",res);
  }
  
  exit(0);
}

int main (int argc, char ** argv)
{
  signal(SIGINT, clean_up);
  
  //Create the shared memory segment
  int shmid = shmget(SHM_KEY,SHM_SIZE, 0666 | IPC_CREAT);
  
  //Creates a queue
  queue* q = (queue*)shmat(shmid,(void*)0,0);

  if(q == (queue*)(-1)){
    perror("shmat error ");
  }

  while(true){

    printf("Size\tFront\tEnd\n");
    printf("%d\t%d\t%d\n",q->size,q->front,q->end);

    printf("QUEUE : [");
    for(int i = 0; i < q->size;i++){
      if(q->job[i].id!=0){
	printf("(%d)",q->job[i].duration);
      }else{
	printf("(_)");
      }
    }
    printf("]\n");
    for(int i = 0; i < q->size;i++){
      if(q->job[i].id!=0){
	printf("Job id %d duration %d\n",q->job[i].id,q->job[i].duration);
      }
    }
    sleep(1);
    system("clear");
  }
  
  return 0;
}
