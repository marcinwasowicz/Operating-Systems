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

void handleSigChild(int sig, siginfo_t* info, void* additionalInfo){
    printf("%s %d\n", "signal code for SIGCHILD", info->si_code);
    printf("%s %d\n", "real user id of process sending signal", info->si_uid);
    printf("%s %ld\n", "band event", info->si_band);
    exit(0);
}

void handleSigInt(int sig, siginfo_t* info, void* additionalInfo){
    printf("%s %d\n", "signal code for SIGINT", info->si_code);
    printf("%s %d\n", "real user id of process sending signal", info->si_uid);
    printf("%s %ld\n", "band event", info->si_band);   
    kill(info->si_pid, SIGINT);
}

void handleSigIntAsAChild(int sig){
    exit(0);
}

void setUpSignalHandling(){
    struct sigaction actionSIGCHILD;
    struct sigaction actionSIGINT;
    actionSIGCHILD.sa_sigaction = handleSigChild;
    actionSIGINT.sa_sigaction = handleSigInt;
    actionSIGCHILD.sa_flags = SA_SIGINFO;
    actionSIGINT.sa_flags = SA_SIGINFO;
    sigemptyset(&actionSIGCHILD.sa_mask);
    sigemptyset(&actionSIGINT.sa_mask);
    sigaction(SIGCHLD, &actionSIGCHILD, NULL);
    sigaction(SIGINT, &actionSIGINT, NULL);
}

int main(){
    setUpSignalHandling();
    pid_t pid = fork();
    if(pid == 0){
        signal(SIGINT, handleSigIntAsAChild);
        kill(getppid(), SIGINT);
        pause();
    }
    while(TRUE){}
    return 0;
}
