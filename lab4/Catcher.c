#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

#define TRUE 1

int caught;

void sigUsr1Handle(int sig, siginfo_t* info, void* ucontext){
    caught++;
    kill(info->si_pid, SIGUSR1);
}

void sigUsr2Handle(int sig, siginfo_t* info, void* ucontext){
    printf("%s %d %s\n","catcher got", caught, "signals");
    kill(info->si_pid, SIGUSR2);
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
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action1, NULL);
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigdelset(&set, SIGUSR2);
    sigprocmask(SIG_SETMASK, &set, NULL);
}


int main(){
    caught = 0;
    printf("%s %d\n","my pid", getpid());
    setUpSignalHandling();
    while(TRUE){}
    return 0;
}