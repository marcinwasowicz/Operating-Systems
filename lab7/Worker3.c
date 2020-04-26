#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<errno.h>
#include<time.h>

#define FLAG 0666

int semaphores;
int sharedMemoryID;
int* sharedMemory;
int MAX_PRODUCTS;
int NUM_PROCESSES;

struct sembuf semaphoreBuffer1;
struct sembuf semaphoreBuffer2;
struct sembuf semaphoreBuffer3;
struct sembuf semaphoreBuffer4;

void handleSignal(int sig){
    shmdt(sharedMemory);
    exit(0);
}

void setUpSemaphoreBuffer(struct sembuf* buff, int semNum, int semOp, int semFlag){
    buff->sem_flg = semFlag;
    buff->sem_num = semNum;
    buff->sem_op = semOp;
}

void setUpWorker(){
    srand(time(NULL));

    setUpSemaphoreBuffer(&semaphoreBuffer1, 4, -1, FLAG);
    setUpSemaphoreBuffer(&semaphoreBuffer2, 5, -1, FLAG);
    setUpSemaphoreBuffer(&semaphoreBuffer3, 5, 1, FLAG);
    setUpSemaphoreBuffer(&semaphoreBuffer4, 0, 1, FLAG);

    signal(SIGINT, handleSignal);
    semaphores = semget(ftok(getenv("HOME"), '1'), 0, FLAG);
    sharedMemoryID = shmget(ftok(getenv("HOME"), '1'), 0, FLAG);
    sharedMemory = (int*) shmat(sharedMemoryID, NULL, FLAG);
}

void doMyJob(){
    semop(semaphores, &semaphoreBuffer1, 1);
    semop(semaphores, &semaphoreBuffer2, 1);

    int n = sharedMemory[sharedMemory[NUM_PROCESSES+2]];
    int idx = sharedMemory[NUM_PROCESSES+2];

    sharedMemory[sharedMemory[NUM_PROCESSES+2]] *= 3;
    sharedMemory[NUM_PROCESSES+2]++;
    if(sharedMemory[NUM_PROCESSES+2] >= (MAX_PRODUCTS + 3 + NUM_PROCESSES)){
        sharedMemory[NUM_PROCESSES+2] = NUM_PROCESSES + 3;
    }

    sleep(1);

    printf("%s%d%s%d%s%d%s%ld\n", "send ", 3*n, " at index ", idx, " my pid is ", getpid(), " the time is ", time(NULL));

    semop(semaphores, &semaphoreBuffer3, 1);
    semop(semaphores, &semaphoreBuffer4, 1);
}

int main(int argc, char* argv[]){
    MAX_PRODUCTS = atoi(argv[1]);
    NUM_PROCESSES = atoi(argv[2]);

    setUpWorker();

    while(1){
        doMyJob();
    }
}