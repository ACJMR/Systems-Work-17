#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

//add struct sembuf
//add union semun

int main(int argc, char* argv[]){
  if (argc == 1){
    printf("Not enough arguments.\n");
    return 0;
  }
  if (argc > 2){
    printf("Too many arguments.\n");
    return 0;
  }

  key_t sem_key = ftok(".",'S');
  key_t mem_key = ftok(".",'R');

  if (strcmp(argv[1],"-c") == 0){
    shmget(mem_key,127,IPC_CREAT | 0644); //creates shared mem
    int semID = semget(sem_key,1,IPC_CREAT | 0644); //creates semaphore
    open("story", IPC_CREAT | O_TRUNC, 0644); //creates file
    union semun data;
    data.val = 1;
    semctl(semID,0,SETVAL,data);
  }
  if (strcmp(argv[1],"-r") == 0){
    int semID = semget(sem_key,1,0); //accesses semaphore
    struct sembuf down;
    down.sem_op = -1;
    down.sem_flg = SEM_UNDO;
    semop(semID,&down,1); //DOWNS the semaphore
    int memID = shmget(mem_key,127,0); //accesses shared mem
    shmctl(memID,IPC_RMID,0); //removes shared mem
    semctl(semID,0,IPC_RMID,0); //removes semaphore

    int fd = open("story", O_RDONLY);
    char story[10000];
    read(fd,story,10000);
    printf("%s\n",story);
    close(fd);

    remove("story"); //removes file
  }

  if (strcmp(argv[1],"-v") == 0){
    int fd = open("story", O_RDONLY);
    char story[10000];
    read(fd,story,10000);
    printf("%s\n",story);
    close(fd);
  }
  return 0;
}
