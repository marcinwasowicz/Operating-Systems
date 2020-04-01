#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/times.h>
#include <time.h>
#include <signal.h>
#include<stdbool.h>


#define TRUE 1

int toSend;
int caught;
bool wait = false;
char mode[256];

void sigUsr1Handle(int sig){
    caught++;
    wait = false;
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
    action.sa_handler = sigUsr1Handle;
    action1.sa_handler = sigUsr2Handle;
    sigemptyset(&action.sa_mask);
    sigemptyset(&action1.sa_mask);
    if(strcmp(mode, "SIGRT")!=0){
        sigaction(SIGUSR1, &action, NULL);
        sigaction(SIGUSR2, &action1, NULL);
    }
    else{
        sigaction(SIGRTMIN+1, &action, NULL);
        sigaction(SIGRTMIN+2, &action1, NULL);
    }
}

int main(int argc, char* argv[]){
    caught = 0;
    int pid = atoi(argv[1]);
    toSend = atoi(argv[2]);
    strcat(mode,argv[3]);
    setUpSignalHandling();
    for(int i = 0; i<toSend;){
        if(!wait){
            wait = true;
            i++;
            if(strcmp(mode, "kill") == 0){
                kill(pid, SIGUSR1);
            }
            else if(strcmp(mode, "sigqueue") == 0){
                union sigval val;
                sigqueue(pid, SIGUSR1, val);
            }
            else{
                kill(pid, SIGRTMIN+1);   
            }
        }
    }
    while(wait){}
    if(strcmp(mode, "kill") == 0){
        kill(pid, SIGUSR2);
    }
    else if(strcmp(mode, "sigqueue") == 0){
        union sigval val;
        sigqueue(pid, SIGUSR2, val);
    }
    else{
        kill(pid, SIGRTMIN+2);   
    }
    while(TRUE){}
    return 0;
}
