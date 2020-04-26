#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<errno.h>
#include<time.h>
#include<string.h>
#include<sys/mman.h>

#define FLAG 0666

sem_t* semas[6];
int sharedMemoryID;
int* sharedMemory;
int MAX_PRODUCTS;
int NUM_PROCESSES;

void setUpSemaphores(){
    semas[0] = sem_open("sema0", O_RDWR, FLAG);
    semas[1] = sem_open("sema1", O_RDWR, FLAG);
    semas[2] = sem_open("sema2", O_RDWR, FLAG);
    semas[3] = sem_open("sema3", O_RDWR, FLAG);
    semas[4] = sem_open("sema4", O_RDWR, FLAG);
    semas[5] = sem_open("sema5", O_RDWR, FLAG);
}

void setUpSharedMemory(){
    sharedMemoryID = shm_open("shared", O_RDWR, FLAG);
    sharedMemory = (int*) mmap(NULL, (NUM_PROCESSES+3+MAX_PRODUCTS)*sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, sharedMemoryID, 0);
}

void handleSignal(int sig){
    for(int i = 0; i<6; i++){
        sem_close(semas[i]);
    }
    munmap(sharedMemory, (NUM_PROCESSES+3+MAX_PRODUCTS)*sizeof(int));
    exit(0);
}

void setUpWorker(){
    setUpSemaphores();
    setUpSharedMemory();
    signal(SIGINT, handleSignal);
}

void doMyJob(){
    sem_wait(semas[2]);
    sem_wait(semas[3]);

    int n = sharedMemory[sharedMemory[NUM_PROCESSES+1]];
    int idx = sharedMemory[NUM_PROCESSES+1];

    sharedMemory[sharedMemory[NUM_PROCESSES+1]] *= 2;
    sharedMemory[NUM_PROCESSES+1]++;
    if(sharedMemory[NUM_PROCESSES+1] >= (MAX_PRODUCTS + 3 + NUM_PROCESSES)){
        sharedMemory[NUM_PROCESSES+1] = NUM_PROCESSES + 3;
    }

    printf("%s%d%s%d%s%d%s%ld\n", "prepared ", 2*n, " at index ", idx, " my pid is ", getpid(), " the time is ", time(NULL));


    sem_post(semas[3]);
    sem_post(semas[4]);
}

int main(int argc, char* argv[]){

    MAX_PRODUCTS = atoi(argv[1]);
    NUM_PROCESSES = atoi(argv[2]);

    setUpWorker();

    while(1){
        doMyJob();
    }
}