#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include<fcntl.h>

void formatBuffer(char* buffer){
    char pid[10];
    sprintf(pid, "%d", getpid());
    strcat(pid, "#");
    strcat(buffer, "#");
    strcat(buffer, pid); 
}

//assume thah source file has enough characters (length(source)%numChars =0).

int main(int argc, char* argv[]){
    int inPipe = open(argv[1], O_WRONLY);
    int source = open(argv[2], O_RDONLY);
    int numChars = atoi(argv[3]);

    char buffer[PIPE_BUF];
    formatBuffer(buffer);
    int prefix = strlen(buffer);
    while(read(source, buffer + prefix, numChars*sizeof(char))!=0){
        write(inPipe, buffer, (numChars+prefix)*sizeof(char));
    }
    close(source);
    close(inPipe);
    return 0;
}