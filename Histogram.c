#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<fcntl.h>

int width;
int height;
int numOfThreads;
int histogram[256];
int** image;
void* (*method) (void*);

typedef struct{
    pthread_t tid;
    int idx;
    long int retval;
}myThread;

myThread** threads;

int min(int x, int y){
    if(x>y){
        return y;
    }
    return x;
}

int max(int x, int y){
    if(x>y){
        return x;
    }
    return y;
}

void* signMethod(void* args){
    struct timespec start;
    struct timespec end;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &start);
    myThread* parameters = (myThread*) args;

    int work = 256/numOfThreads;
    int res = 256%numOfThreads;
    int from = (parameters->idx)*work + min(res, parameters->idx);
    int to = from + work + min(1, max(0, res - (parameters->idx))) - 1;

    for(int i = 0; i<height; i++){
        for(int j = 0; j<width; j++){
            if(image[i][j] >= from && image[i][j]<= to){
                histogram[image[i][j]]++;
            }
        }
    }
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &end);
    end.tv_sec -= start.tv_sec;
    end.tv_nsec -= start.tv_nsec;

    parameters->retval = end.tv_nsec;

    pthread_exit(&parameters->retval);
}

void* blockMethod(void* args){
    struct timespec start;
    struct timespec end;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &start);
    myThread* parameters = (myThread*) args;

    int work = width/numOfThreads;
    int res = width%numOfThreads;
    int from = (parameters->idx)*work + min(res, parameters->idx);
    int to = from + work + min(1, max(0, res - (parameters->idx))) - 1;

    for(int i = 0; i<height; i++){
        for(int j = from; j<= to; j++){
            histogram[image[i][j]]++;
        }
    }

    clock_gettime(_POSIX_MONOTONIC_CLOCK, &end);
    end.tv_sec -= start.tv_sec;
    end.tv_nsec -= start.tv_nsec;

    parameters->retval = end.tv_nsec;

    pthread_exit(&parameters->retval);
}

void* interleavedMethod(void* args){
    struct timespec start;
    struct timespec end;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &start);
    myThread* parameters = (myThread*) args;

    for(int i = 0; i<height; i++){
        for(int j = parameters->idx; j<width; j += numOfThreads){
            histogram[image[i][j]]++;
        }
    }

    clock_gettime(_POSIX_MONOTONIC_CLOCK, &end);
    end.tv_sec -= start.tv_sec;
    end.tv_nsec -= start.tv_nsec;

    parameters->retval = end.tv_nsec;

    pthread_exit(&parameters->retval);
}

void setUpGlobalVariables(char* argv[]){
    numOfThreads = atoi(argv[1]);

    if(strcmp(argv[2], "sign") == 0){
        method = &signMethod;
    }
    else if(strcmp(argv[2], "block") == 0){
        method = &blockMethod;
    }
    else{
        method = &interleavedMethod;
    }

    FILE* input = fopen(argv[3], "r");
    char c[2];
    fscanf(input,"%c%c\n%d %d", &c[0], &c[1], &width, &height);
    image = (int**)malloc(sizeof(int*)*height);
    for(int i = 0; i<height; i++){
        image[i] = (int*)malloc(sizeof(int)*width);
    }
    for(int i = 0; i<height; i++){
        for(int j = 0; j<width; j++){
            fscanf(input, "%d", &image[i][j]);
        }
    }
    threads = (myThread**) malloc(numOfThreads*sizeof(myThread));
    for(int i = 0; i<numOfThreads; i++){
        threads[i] = (myThread*) malloc(sizeof(myThread));
    }
    for(int i = 0; i<256; i++){
        histogram[i] = 0;
    }
}

void spawnThreads(void* (*method)(void*)){
    for(int i = 0; i<numOfThreads; i++){
        threads[i]->idx = i;
        pthread_create(&threads[i]->tid, NULL, method, (void*) threads[i]);
    }
}

void waitForThreads(){
    for(int i = 0; i<numOfThreads; i++){
        long int* result;
        pthread_join(threads[i]->tid, NULL);
    }
}

void raportThreads(){
    for(int i = 0; i<numOfThreads; i++){
        printf("%s %ld %s %ld\n","thread of id", threads[i]->tid, "returned with value", threads[i]->retval/1000);
    }
}

void clean(){
    for(int i = 0; i<numOfThreads; i++){
        free(threads[i]);
    }
    free(threads);
    for(int i = 0; i<height; i++){
        free(image[i]);
    }
    free(image);
}

void printHistogram(char* outputFile){
    FILE* output = fopen(outputFile, "w+");
    for(int i = 0; i<256; i++){
        fprintf(output, "%d %d\n", i, histogram[i]);
    }
    fclose(output);
}

int main(int argc, char* argv[]){
    setUpGlobalVariables(argv);
    struct timespec start;
    struct timespec end;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &start);
    spawnThreads(method);
    waitForThreads();
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &end);
    raportThreads();
    printf("%s %ld\n", "whole operation took", (end.tv_nsec-start.tv_nsec)/1000);
    clean();
    printHistogram(argv[4]);
    return 0;
}