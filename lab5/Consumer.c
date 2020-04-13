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

char* buffer;

void freeBuffer(int sig){
    loop = false;
}

int main(int argc, char* argv[]){
    signal(SIGINT, freeBuffer);

    int outPipe = open(argv[1], O_RDONLY);
    int storage = open(argv[2], O_WRONLY);
    int numChars = atoi(argv[3]);

    buffer = (char*)malloc(sizeof(char)*numChars);
    int isWriter = 0;

    while(loop){
        isWriter = read(outPipe, buffer, numChars*sizeof(char));
        if(isWriter!=0){
            write(storage, buffer, sizeof(char)*isWriter);
        }
    }

    while((isWriter = read(outPipe, buffer, numChars*sizeof(char)))!=0){
        write(storage, buffer, sizeof(char)*isWriter);
    }

    close(outPipe);
    close(storage);
    free(buffer);
    return 0;
}