#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

#define TRUE 1

int caught = 0;
char mode[256];

void sigUsr1Handle(int sig){
    caught++;
}

void sigUsr2Handle(int sig, siginfo_t* info, void* ucontext){
    for(int i = 0; i<caught; i++){
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
    action.sa_flags = 0;
    action1.sa_flags = SA_SIGINFO;
    action.sa_handler = sigUsr1Handle;
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
