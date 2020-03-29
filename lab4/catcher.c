#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

#define TRUE 1

int caught = 0;

void sigUsr1Handle(int sig){
    caught++;
}

void sigUsr2Handle(int sig, siginfo_t* info, void* ucontext){
    for(int i = 0; i<caught; i++){
        kill(info->si_pid, SIGUSR1);
    }
    printf("%s %d %s\n","catcher got", caught, "signals");
    kill(info->si_pid, SIGUSR2);
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
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action1, NULL);
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigdelset(&set, SIGUSR2);
    sigprocmask(SIG_SETMASK, &set, NULL);
}

int main(){
    printf("%s %d\n","my pid", getpid());
    setUpSignalHandling();
    while(TRUE){};
    return 0;
}