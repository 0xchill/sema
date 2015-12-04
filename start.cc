/******************************************************************
 * Program for setting up semaphores/shared memory before running
 * the Producer and Consumer 
 ******************************************************************/

# include "helper.h"

int main (int argc, char **argv)
{
  //Create and init the two semaphores array
  int semid = sem_create(SEM_KEY, 2);
  sem_init(semid,1,0);
  sem_init(semid,2,1);
  
  //Create the shared memory segment
  int shmid = shmget(SHM_KEY,SHM_SIZE, 0666 | IPC_CREAT);
  
  //Creates a queue
  queue* q = (queue*)shmat(shmid,(void*)0,0);
  q->size = 0;
  q->front = 0;
  q->end =0;

  return 0;
}
