#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main(){
  key_t sem_key = ftok(".",'S');
  key_t mem_key = ftok(".",'R');

  int semID = semget(sem_key,1,0);

  struct sembuf opp;
  opp.sem_op = -1;
  opp.sem_flg = SEM_UNDO;
  semop(semID,&opp,1); //DOWNS the semaphore
  int memID = shmget(mem_key,127,0); //accesses shared mem
  char * data = shmat(memID,0,0); //attaches shared mem to data

  char bytes = data[0]; //gets size of message in shared mem

  if (!bytes){
    printf("Please input a phrase\n");
  }
  else{
    char * last_line = &data[1];
    printf("Please input a phrase\n");
    printf("Previous Line: %s\n",last_line);
  }

  char answer[127];
  fgets(answer,127,stdin); //gets user input

  bytes = sizeof(answer);

  data[0] = bytes; //writes bytes to first slot in shared mem
  strcpy(&data[1],answer); //writes string to shared mem

  shmdt(data); //detatches shared mem from data

  int fd = open("story", O_WRONLY | O_APPEND);
  write(fd, answer, bytes);
  close(fd);

  opp.sem_op = 1;
  semop(semID,&opp,1); //UPS the semaphore

  return 0;
}
