// Consumer

# include "helper.h"
#include <time.h>

int main (int argc, char *argv[])
{
  int consumer_id;
  if(argc != 2){
    printf("help: ./consumer consumer-id \n");
    return 1;
  }
  time_t start = time(0);
  
  //Read in the one command line argument
  consumer_id = check_arg(argv[1]);
  
  //associate with the semaphore array
  int semid = sem_attach(SEM_KEY);
  
  //associate with the shared memory
  int shmid = shmget(SHM_KEY,SHM_SIZE,SHM_R);
  
  //get the queue
  queue* q = (queue*)shmat(shmid,(void*)0,0);
  if(q==(queue*)(-1))
    perror("shmat consumer ");
    
  while(true){
	  
	//-------------Down item-----------
	if(sem_timewait(semid,SEM_ITEM,10) == -1){
	  printf("Consumer (%d) time %d : No jobs left.\n",consumer_id,time(0)-start);
	  break;
	}

	//-------------Down mutex-----------
	sem_wait(semid,SEM_MUTEX);
	
	int duration = q->job[q->front].duration;
	int job_id = q->job[q->front].id;

	q->job[q->front].id = 0;
	q->front = (q->front + 1)%(q->size);

	//-------------Up mutex-----------
	sem_signal(semid,SEM_MUTEX);

	//-------------Up space-----------
    sem_signal(semid,SEM_SPACE);

	printf("Consumer (%d) time %d : Job id ",consumer_id,time(0)-start);
	printf("%d executing sleep duration %d\n",job_id,duration);
	sleep(duration);
	printf("Consumer (%d) time %d : Job id ",consumer_id,time(0)-start);
	printf("%d completed\n",job_id);
     
  }
  sleep(50);
  
  //close semaphores
  sem_close(semid);
        
  //detach from shared memory
  shmdt(q);

  //delete shared memory
  shmctl(shmid,IPC_RMID,NULL);
  
  return 0;
}
