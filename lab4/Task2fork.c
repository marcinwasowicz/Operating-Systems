#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>


void handler(int sig){
    printf("%s %d\n", "recieved SIGUSR1 in process of id", getpid());
}

int main(int argc, char* argv[]){
    char* mode = argv[1];
    int masked = 0;
    if(strcmp(mode, "ignore")==0){
        signal(SIGUSR1, SIG_IGN);
    }
    else if(strcmp(mode, "handler")==0){
        signal(SIGUSR1, handler);
    }
    else{
        sigset_t newMask;
        sigemptyset(&newMask);
        sigaddset(&newMask, SIGUSR1);
        sigprocmask(SIG_SETMASK, &newMask, NULL);
        masked = 1;
    }
    raise(SIGUSR1);
    if(masked){
        sigset_t test;
        sigpending(&test);
        printf("%s %d\n", "ture/false val is:", sigismember(&test, SIGUSR1));
    }
    pid_t pid = fork();
    if(pid == 0){
        if(strcmp(mode, "pending")!=0){
            raise(SIGUSR1);
        }
        if(masked){
            sigset_t test;
            sigpending(&test);
            printf("%s %d\n", "ture/false val is:", sigismember(&test, SIGUSR1));
        }
    }
    else{
        wait(NULL);
    }
    return 0;
}