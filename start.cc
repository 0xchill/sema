/******************************************************************
 * Program for setting up semaphores/shared memory before running
 * the Producer and Consumer 
 ******************************************************************/

# include "helper.h"

int main (int argc, char **argv)
{
	
  if(argc != 2){
    printf("Wrong number of arguments for %s\n",argv[0]);
    exit(1);
  }
  
  //get the size of the queue
  int queue_size = check_arg(argv[1]);
  
  //Create and init the two semaphores array
  int semid = sem_create(SEM_KEY, 3);
  sem_init(semid,SEM_ITEM,0);
  sem_init(semid,SEM_SPACE,queue_size);
  sem_init(semid,SEM_MUTEX,1);
  
  //Create the shared memory segment
  int shmid = shmget(SHM_KEY,SHM_SIZE, 0666 | IPC_CREAT);
  
  //Creates a queue
  queue* q = (queue*)shmat(shmid,(void*)0,0);
  q->size = queue_size;
  q->front = 0;
  q->end =0;

  for(int i = 0;i<queue_size;i++){
    q->job[i].id = 0;
  }

  return 0;
}
