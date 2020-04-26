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

#define FLAG 0666

int semaphores;
int sharedMemoryID;
int* sharedMemory;
int MAX_PRODUCTS;
int NUM_WORKERS_1;
int NUM_WORKERS_2;
int NUM_WORKERS_3;
int NUM_PROCESSES;

int processIter = 0;
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
}arg;

void setUpSemaphores(){
    semaphores = semget(ftok(getenv("HOME"), '1'), 6, IPC_CREAT | FLAG);
    arg.val = MAX_PRODUCTS;
    semctl(semaphores, 0, SETVAL, arg);
    arg.val = 1;
    semctl(semaphores, 1, SETVAL, arg);
    arg.val = 0;
    semctl(semaphores, 2, SETVAL, arg);
    arg.val = 1;
    semctl(semaphores, 3, SETVAL, arg);
    arg.val = 0;
    semctl(semaphores, 4, SETVAL, arg);
    arg.val = 1;
    semctl(semaphores, 5, SETVAL, arg);
}

void setUpSharedMemory(){
    sharedMemoryID = shmget(ftok(getenv("HOME"), '1'), (MAX_PRODUCTS+NUM_PROCESSES+3)*sizeof(int), IPC_CREAT | FLAG);
    sharedMemory = (int*) shmat(sharedMemoryID, NULL, FLAG);
}

void handleSignal(int sig){

    for(int i = 0; i<NUM_PROCESSES; i++){
        kill(sharedMemory[i], SIGINT);
    }

    for(int i = 0; i<NUM_PROCESSES; i++){
        wait(NULL);
    }

    shmdt(sharedMemory);
    shmctl(sharedMemoryID, IPC_RMID, NULL);
    semctl(semaphores, 0, IPC_RMID);
    exit(0);
}
void spawnWorkers(int num, char* path, char* arg1, char* arg2){
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

    setUpSemaphores();
    setUpSharedMemory();

    signal(SIGINT, handleSignal);

    sharedMemory[NUM_PROCESSES] = NUM_PROCESSES+3;
    sharedMemory[NUM_PROCESSES+1] = NUM_PROCESSES+3;
    sharedMemory[NUM_PROCESSES+2] = NUM_PROCESSES+3;

    char arg1[10];
    char arg2[10];
    sprintf(arg1, "%d", MAX_PRODUCTS);
    sprintf(arg2, "%d", NUM_PROCESSES);

    spawnWorkers(NUM_WORKERS_1, "./Worker1", arg1, arg2);
    spawnWorkers(NUM_WORKERS_2, "./Worker2", arg1, arg2);
    spawnWorkers(NUM_WORKERS_3, "./Worker3", arg1, arg2);

    while(1){}
}

