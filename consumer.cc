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
  
  queue* q;
  
  bool keep_going = true;
  
  while(keep_going){
    
    //wait for memory access
    sem_wait(semid,2);
    
    //get the queue
    void* data = shmat(shmid,(void*)0,0);
    if(data==(void*)(-1))
      perror("shmat consumer ");
    q = (queue*)data;
    
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
      
      sem_signal(semid,2);
      
    }else{
      sem_signal(semid,2);
      if(sem_timewait(semid,1,10) == -1){
        printf("Consumer (%d) time %d : No jobs left.\n",consumer_id,time(0)-start);
        keep_going = false;
        //close semaphores
        sem_close(semid);
        //detach from shared memory
        shmdt(data);
        //delete shared memory
        shmctl(shmid,IPC_RMID,NULL);
      }
    }
    
  }
  return 0;
}
