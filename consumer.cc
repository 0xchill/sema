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
    
    //wait for memory access
    sem_wait(semid,2);
    
    //if the queue is not empty execute the job
    if(q->size!=0){
      int duration = q->job[q->front].duration;
      
      printf("Consumer (%d) time %d : Job id ",consumer_id,time(0)-start);
      printf("%d executing sleep duration %d\n",q->job[q->front].id,duration);
      sleep(duration);
      printf("Consumer (%d) time %d : Job id ",consumer_id,time(0)-start);
      printf("%d completed\n",q->job[q->front].id);
      
      q->front ++;
      q->size --;
      
      //up on the data
      sem_signal(semid,2);
    }else{
      //up on the data
      sem_signal(semid,2);
      
      if(sem_timewait(semid,1,10) == -1){
        printf("Consumer (%d) time %d : No jobs left.\n",consumer_id,time(0)-start);
        break;
      }
    }
      
    
    
  }
  sleep(100);
  //close semaphores
  sem_close(semid);
        
  //detach from shared memory
  shmdt(q);
        
  //delete shared memory
  shmctl(shmid,IPC_RMID,NULL);
  
  return 0;
}
