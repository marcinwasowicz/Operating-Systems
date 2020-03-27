#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<stdbool.h>
#include<dirent.h>

#define endl printf("%s", "\n")

bool stopped = false;

void sigIntHandle(int sig){
    printf("%s\n", "Odebrano sygnał SIGINT");
    exit(0);
}

void sigStpHandle(int sig){
    stopped = !stopped;
    printf("%s %d\n", "Ctlr+Z - continue, Ctrl+C - end", stopped);
}
// perhaps i am wrong, but using system("ls") might fail, since once system() is executed the main process is stopped and Ctrl + Z signal
// might be sent to the ls process, which is stopped, but has not a handler defined to come back.
void printCurrentDirectory(){
    DIR* dir = opendir("./");
    for(struct dirent* file = readdir(dir); file!=NULL; file = readdir(dir)){
        if(stopped){
            return;
        }
        printf("%s ", file->d_name);
    }
    endl;
    closedir(dir);
}

int main(){
    signal(SIGINT, sigIntHandle);
    struct sigaction act;
    act.sa_handler = sigStpHandle;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGTSTP, &act, NULL);
    while(1){
        printCurrentDirectory();
    }
}