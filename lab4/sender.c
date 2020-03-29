#define _X_OPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

#define TRUE 1

int toSend;
int caught = 0;

void sigUsr1Handle(int sig){
    caught++;
}

void sigUsr2Handle(int sig){
    printf("%s %d %s %d\n","should have caught",toSend,"actually caught", caught);
    exit(0);
}

void setUpSignalHandling(){
    struct sigaction action;
    struct sigaction action1;
    action.sa_flags = 0;
    action1.sa_flags = 0;
    action.sa_handle = sigUsr1Handle;
    action1.sa_handle = sigUsr2Handle;
    sigemptyset(action.sa_mask);
    sigemptyset(action1.sa_mask);
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action1, NULL);
}


int main(int argc, char* argv[]){
    int pid = atoi(argv[1]);
    toSend = atoi(argv[2]);
    setUpSignalHandling();
    for(int i = 0; i<toSend; i++){
        kill(pid, SIGUSR1);
    }
    kill(pid, SIGUSR2);
    while(TRUE){}
    return 0;
}

