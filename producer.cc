// Producer

# include "helper.h"
# include <time.h>

int main (int argc, char *argv[])
{
  int job_number, producer_id;
  if(argc != 3){
    printf("help: ./producer producer-id jobs-number\n");
    return 1;
  }
  //Read in the two command line arguments
  producer_id = check_arg(argv[1]);
  job_number = check_arg(argv[2]);
  
  //associate with the semaphores
  int semid = sem_attach(SEM_KEY);
  
  //associate with the shared memory
  int shmid = shmget(SHM_KEY,SHM_SIZE,SHM_R);
  
  queue* q;
  
  time_t start = time(0);
  time_t now = start;
  
  for(int i=0;i<job_number;i++){
    int time_wait = (rand()%3)+2;
    int job_duration = (rand()%6)+2;
    
    //wait for access
    sem_wait(semid,2);
    
    //penetration
    void* data = shmat(shmid,(void*)0,0);
    if(data==(void*)(-1))
      perror("shmat producer ");
    q = (queue*)data;
    if(q->size != 0){
      q->end ++;
    }
    q->job[q->end].id = q->end + 1;
    q->job[q->end].duration = job_duration;
    q->size ++;
    
    //signals that the queue is not empty
    sem_signal(semid,1);
    
    now = time(0);
    printf("Producer (%d) time %d: Job id ",producer_id,now-start);
    printf("%d duration %d\n",q->job[q->end].id,job_duration);
    
    //I'm OUT
    sem_signal(semid,2);
    
    if(i==job_number -1){
      shmdt(data);
    }else{
      sleep(time_wait);
    }
    
  }
  
  now = time(0);
  printf("Producer (%d) time %d: ",producer_id,now-start);
  printf("No more jobs to generate.\n");
  
  return 0;
}
