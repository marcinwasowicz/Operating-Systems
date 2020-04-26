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
#include<sys/wait.h>

#define FLAG 0666

sem_t* semas[6];
int sharedMemoryID;
int* sharedMemory;
int MAX_PRODUCTS;
int NUM_WORKERS_1;
int NUM_WORKERS_2;
int NUM_WORKERS_3;
int NUM_PROCESSES;

int processIter = 0;

void setUpSemaphores(){
    semas[0] = sem_open("sema0", O_RDWR|O_CREAT, FLAG, MAX_PRODUCTS);
    semas[1] = sem_open("sema1", O_RDWR|O_CREAT, FLAG, 1);
    semas[2] = sem_open("sema2", O_RDWR|O_CREAT, FLAG, 0);
    semas[3] = sem_open("sema3", O_RDWR|O_CREAT, FLAG, 1);
    semas[4] = sem_open("sema4", O_RDWR|O_CREAT, FLAG, 0);
    semas[5] = sem_open("sema5", O_RDWR|O_CREAT, FLAG, 1);
}

void setUpSharedMemory(){
    sharedMemoryID = shm_open("shared", O_RDWR|O_CREAT, FLAG);
    ftruncate(sharedMemoryID, (MAX_PRODUCTS + 3 + NUM_PROCESSES)*sizeof(int));
    sharedMemory = (int*) mmap(NULL, (NUM_PROCESSES+3+MAX_PRODUCTS)*sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, sharedMemoryID, 0);
}

void handleSignal(){

    for(int i = 0; i<NUM_PROCESSES; i++){
        kill(sharedMemory[i], SIGINT);
    }

    for(int i = 0; i<NUM_PROCESSES; i++){
        wait(NULL);
    }

    munmap(sharedMemory, (NUM_PROCESSES+3+MAX_PRODUCTS)*sizeof(int));
    shm_unlink("shared");
    for(int i = 0; i<6; i++){
        sem_close(semas[i]);
    }
    sem_unlink("sema0");
    sem_unlink("sema1");
    sem_unlink("sema2");
    sem_unlink("sema3");
    sem_unlink("sema4");
    sem_unlink("sema5");
    exit(0);
}

void spawnWorkers(int num,char* path, char* arg1, char* arg2){
    for(int i = 0; i<num; i++){
        pid_t pid = fork();
        if(pid == 0){
            execlp(path, path, arg1, arg2, NULL);
        }
        else{
            sharedMemory[processIter] = pid;
            processIter++;
        }
    }
}

int main(int argc, char* argv[]){

    MAX_PRODUCTS = atoi(argv[1]);
    NUM_WORKERS_1 = atoi(argv[2]);
    NUM_WORKERS_2 = atoi(argv[3]);
    NUM_WORKERS_3 = atoi(argv[4]);
    NUM_PROCESSES = NUM_WORKERS_1+NUM_WORKERS_2+NUM_WORKERS_3;

    signal(SIGINT, handleSignal);

    setUpSemaphores();
    setUpSharedMemory();

    sharedMemory[NUM_PROCESSES] = NUM_PROCESSES+3;
    sharedMemory[NUM_PROCESSES+1] = NUM_PROCESSES+3;
    sharedMemory[NUM_PROCESSES+2] = NUM_PROCESSES+3;

    char arg1[10];
    char arg2[10];
    sprintf(arg1, "%d", MAX_PRODUCTS);
    sprintf(arg2, "%d", NUM_PROCESSES);

    spawnWorkers(NUM_WORKERS_1, "./wposix1", arg1, arg2);
    spawnWorkers(NUM_WORKERS_2, "./wposix2", arg1, arg2);
    spawnWorkers(NUM_WORKERS_3, "./wposix3", arg1, arg2);

    while(1){}
}