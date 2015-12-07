/******************************************************************
 * Program for setting up semaphores/shared memory before running
 * the Producer and Consumer 
 ******************************************************************/

# include "helper.h"

int main (int argc, char **argv)
{
	
  if(argc != 2){
    printf("help: ./start queue-size \n");
    return 1;
  }
  
  //get the size of the queue
  int queue_size = check_arg(argv[1]);
  if(queue_size == -1){
    printf("Wrong input for queue-size, only numbers are accepted\n");
    return 1;
  }else if(queue_size > QUEUE_MAX_SIZE){
    printf("Queue too long \n");
    return 1;
  }
  
  //Create the semaphores array
  int semid = sem_create(SEM_KEY, 3);
  if(semid == -1){
    printf("Error creating the semaphores \n");
    return 1;
  }
  
  //Init the semaphores
  sem_init(semid,SEM_ITEM,0);
  sem_init(semid,SEM_SPACE,queue_size);
  sem_init(semid,SEM_MUTEX,1);

  //Create the shared memory segment
  int shmid = shmget(SHM_KEY,SHM_SIZE, 0666 | IPC_CREAT);
  if(shmid == -1){
    printf("Error creating the shared memory segment\n");
    return 1;
  }
  
  //Creates a queue
  queue* q = (queue*)shmat(shmid,(void*)0,0);
  if(q == (queue*)(-1)){
    perror("shmat in ./start");
    return 1;
  }
  
  //Init the queue
  q->size = queue_size;
  q->front = 0;
  q->end =0;
  for(int i = 0;i<queue_size;i++){
    q->job[i].id = 0;
  }

  return 0;
}
