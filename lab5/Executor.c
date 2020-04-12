#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>

int main(int argc, char* argv[]){
    int numProducers = atoi(argv[1]);
    char* fifoName = "potok";
    mkfifo(fifoName, 0666);
    pid_t consPid = fork();
    if(consPid == 0){
        execlp("./cons", "./cons", fifoName, argv[2],argv[3], NULL);
    }
    for(int i = 4; i+1<argc; i+=2){
        pid_t pid = fork();
        if(pid == 0){
            execlp("./prod", "./prod", fifoName, argv[i], argv[i+1], NULL);
        }
    }
    for(int i = 0; i<numProducers; i++){
        wait(NULL);
    }
    kill(consPid, SIGINT);
    return 0;
}