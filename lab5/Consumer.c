#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include<fcntl.h>
#include<signal.h>
#include<stdbool.h>

bool loop = true;
bool writing = false;

char* buffer;

void freeBuffer(int sig){
    if(writing){
        loop = false;
    }
    else{
        free(buffer);
        exit(0);
    }
}

int main(int argc, char* argv[]){
    signal(SIGINT, freeBuffer);

    int outPipe = open(argv[1], O_RDONLY);
    int storage = open(argv[2], O_WRONLY);
    int numChars = atoi(argv[3]);

    buffer = (char*)malloc(sizeof(char)*numChars);

    while(loop){
        writing = false;
        int isWriter = read(outPipe, buffer, numChars*sizeof(char));
        writing = true;
        if(isWriter!=0){
            write(storage, buffer, sizeof(char)*isWriter);
        }
    }

    free(buffer);
    return 0;
}