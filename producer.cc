// Producer

# include "helper.h"
# include <time.h>

int produce_job(queue* q,int job_duration){
	//Produce a job
	int job_index;

	//if the queue is empty
	if(q->end == q->front && q->job[q->end].id == 0){
		job_index = q->end;
	}else{
		job_index = (q->end + 1)%(q->size);
		q->end = job_index;
	}
	q->job[job_index].id = job_index + 1;
	q->job[job_index].duration = job_duration;

	return job_index;
}


int main (int argc, char *argv[])
{
  int job_number, producer_id;
  if(argc != 3){
    printf("help: ./producer producer-id job-number\n");
    return 1;
  }
  //Read in the two command line arguments
  producer_id = check_arg(argv[1]);
  if(producer_id == -1){
    printf("Wrong input for producer-id, only numbers are accepted\n");
    return 1;
  }
  job_number = check_arg(argv[2]);
  if(job_number == -1){
    printf("Wrong input for job-number, only numbers are accepted\n");
    return 1;
  }
  
  //associate with the semaphores
  int semid = sem_attach(SEM_KEY);
  if(semid == -1){
    printf("Error, can't attach producer (%d) to the semaphores\n",producer_id);
    return 1;
  }
  
  //associate with the shared memory
  int shmid = shmget(SHM_KEY,SHM_SIZE,SHM_W);
  if(semid == -1){
    printf("Error, can't associate producer (%d) to the shared memory\n",producer_id);
    return 1;
  }
  
  //link to the queue
  queue* q = (queue*)shmat(shmid,(void*)0,0);
  if(q==(queue*)(-1)){
    perror("shmat producer ");
  }
  
  time_t start = time(0);
  time_t now = start;
  
  for(int i=0;i<job_number;i++){
	  
    int time_wait = (rand()%3)+2;
    int job_duration = (rand()%6)+2;

    //-------------Down space-----------
    sem_wait(semid,SEM_SPACE);
    
	//-------------Down mutex-----------
    sem_wait(semid,SEM_MUTEX);

	int job_index = produce_job(q,job_duration);

	now = time(0);
	printf("Producer (%d) time %d: Job id ",producer_id,(int)(now-start));
	printf("%d duration %d\n",q->job[job_index].id,job_duration);

    //-------------Up mutex-----------
    sem_signal(semid,SEM_MUTEX);

    //-------------Up item-----------
    sem_signal(semid,SEM_ITEM);
    
    
    if(i!=job_number-1){
      sleep(time_wait);
    }
    
  }
  
  now = time(0);
  
  printf("Producer (%d) time %d: ",producer_id,(int)(now-start));
  printf("No more jobs to generate.\n");
  
  sleep(50);
  //detach from the memory
  shmdt(q);
  
  return 0;
}
