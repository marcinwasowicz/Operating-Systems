#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/times.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include<stdbool.h>


#define TRUE 1

int caught;
char mode[256];

void sigUsr1Handle(int sig, siginfo_t* info, void* ucontext){
    caught++;
    if(strcmp(mode, "kill") == 0){
        kill(info->si_pid, SIGUSR1);
    }
    else if(strcmp(mode, "sigqueue") == 0){
        union sigval val;
        val.sival_int = caught;
        sigqueue(info->si_pid, SIGUSR1, val);
    }
    else{
        kill(info->si_pid, SIGRTMIN+1);
    }
}

void sigUsr2Handle(int sig, siginfo_t* info, void* ucontext){
    printf("%s %d %s\n","catcher got", caught, "signals");
    if(strcmp(mode, "kill") == 0){
        kill(info->si_pid, SIGUSR2);
    }
    else if(strcmp(mode, "sigqueue") == 0){
        union sigval val;
        sigqueue(info->si_pid, SIGUSR2, val);
    }
    else{
        kill(info->si_pid, SIGRTMIN+2);
    }
    exit(0);
}

void setUpSignalHandling(){
    struct sigaction action;
    struct sigaction action1;
    action.sa_flags = SA_SIGINFO;
    action1.sa_flags = SA_SIGINFO;
    action.sa_sigaction = sigUsr1Handle;
    action1.sa_sigaction = sigUsr2Handle;
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
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigdelset(&set, SIGUSR2);
    sigdelset(&set, SIGINT);
    if(strcmp(mode, "SIGRT") == 0){
        sigdelset(&set, SIGRTMIN+1);
        sigdelset(&set, SIGRTMIN+2);
    }
    sigprocmask(SIG_SETMASK, &set, NULL);
}


int main(int argc, char* argv[]){
    strcat(mode, argv[1]);
    caught = 0;
    printf("%s %d\n","my pid", getpid());
    setUpSignalHandling();
    while(TRUE){}
    return 0;
}