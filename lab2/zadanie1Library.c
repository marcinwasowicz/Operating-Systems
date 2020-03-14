#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdarg.h>

char* stringConcat(int num, ...){
    va_list list;
    char** tempArrays = (char**)malloc(sizeof(char*)*num);
    int* sizes = (int*) malloc(sizeof(int)*num);
    int totalLength = 0;
    va_start(list, num);
    for(int i = 0; i<num; i++){
        char* temp = va_arg(list, char*);
        tempArrays[i] = temp;
        sizes[i] = strlen(temp);
        totalLength+=sizes[i];
    }
    char* result = (char*)malloc(sizeof(char)*totalLength);
    int iterator = 0;
    for(int i = 0; i<num; i++){
        for(int j = 0; j<sizes[i]; j++){
            result[iterator] = tempArrays[i][j];
            iterator++;
        }
    }
    free(sizes);
    free(tempArrays);
    va_end(list);
    return result;
}


void GenerateFileWithSystem(char* name, int count, int rlen){
    int fd = open(name, O_RDWR | O_CREAT, 0666);
    //srand(time(NULL));
    char* temp = (char*) malloc(sizeof(char)*rlen);
    temp[rlen-1] = '\n';
    for(int i = 0; i<count; i++){
        for(int j = 0; j<rlen-1; j++){
            temp[j]= rand()%26+'a';
        }
        write(fd, temp, rlen);
    }
    free(temp);
    close(fd);
}

//swaps records indexed with pos1 and pos2
void swapFileRecordsSystem(char* buff1, char* buff2, int pos1, int pos2, int fd, int rlen){
    lseek(fd, pos1*rlen, 0);
    write(fd, buff2, rlen);
    lseek(fd, pos2*rlen, 0);
    write(fd, buff1, rlen);
}

int partitionSystem(int low, int high, char* filePath, int rlen){
   int fd = open(filePath, O_RDWR);
   char* pivot = (char*)malloc(sizeof(char)*rlen);
   char* buff = (char*)malloc(sizeof(char)* rlen);
   lseek(fd, high*rlen, 0);
   read(fd, pivot, rlen);
   int ppos = low-1;
   int j = low;
   lseek(fd, j*rlen, 0);
   for(;j<high; j++){
        read(fd, buff, rlen);
        if(strcmp(buff, pivot) < 0){
            ppos++;
            lseek(fd, ppos*rlen, 0);
            read(fd, pivot, rlen);
            swapFileRecordsSystem(pivot, buff, ppos, j, fd, rlen);
            lseek(fd, high*rlen, 0);
            read(fd, pivot, rlen);
            lseek(fd, (j+1)*rlen, 0);
        }
   }
    ppos++;
    lseek(fd, ppos*rlen, 0);
    read(fd, buff, rlen);
    swapFileRecordsSystem(pivot, buff, high, ppos, fd, rlen);
    close(fd);
    free(buff);
    free(pivot);
    return ppos;
}

void QuickSortSystem(char* filePath, int low, int high, int rlen){
    if(low < high){
        int q = partitionSystem(low, high, filePath, rlen);
        QuickSortSystem(filePath, low, q-1, rlen);
        QuickSortSystem(filePath, q+1, high, rlen);
    }
}

void GenerateFileWithLibrary(char* name, int count, int rlen){
    FILE* file = fopen(name, "w+");
    char* temp = (char*)malloc(sizeof(char)*rlen);
    temp[rlen-1] = '\n';
    //srand(time(NULL));
    for(int i = 0; i<count; i++){
        for(int j = 0 ; j<rlen-1; j++){
            temp[j] = rand()%26 + 'a';
        }
        fwrite(temp, sizeof(char), rlen, file);
    }
    free(temp);
    fclose(file);
}

void swapFileRecordsLibrary(char* buff1, char* buff2, int pos1, int pos2, FILE* f, int rlen){
    fseek(f, pos1*rlen, 0);
    fwrite(buff2,sizeof(char), rlen, f);
    fseek(f, pos2*rlen, 0);
    fwrite(buff1,sizeof(char), rlen, f);
}

int partitionLibrary(int low, int high, char* filePath, int rlen){
    FILE* f = fopen(filePath, "r+");
    char* pivot = (char*)malloc(sizeof(char)*rlen);
    char* buff = (char*)malloc(sizeof(char)* rlen);
    fseek(f, high*rlen, 0);
    fread(pivot,sizeof(char), rlen, f);
    int ppos = low-1;
    int j = low;
    fseek(f, j*rlen, 0);
    for(;j<high; j++){
        fread(buff,sizeof(char), rlen, f);
        if(strcmp(buff, pivot) < 0){
            ppos++;
            fseek(f, ppos*rlen, 0);
            fread(pivot,sizeof(char), rlen, f);
            swapFileRecordsLibrary(pivot, buff, ppos, j, f, rlen);
            fseek(f, high*rlen, 0);
            fread(pivot,sizeof(char), rlen, f);
            fseek(f, (j+1)*rlen, 0);
        }
    }
    ppos++;
    fseek(f, ppos*rlen, 0);
    fread(buff,sizeof(char), rlen,f);
    swapFileRecordsLibrary(pivot, buff, high, ppos, f, rlen);
    fclose(f);
    free(buff);
    free(pivot);
    return ppos;
}

void QuickSortLibrary(char* filePath, int low, int high, int rlen){
    if(low < high){
        int q = partitionLibrary(low, high, filePath, rlen);
        QuickSortLibrary(filePath, low, q-1, rlen);
        QuickSortLibrary(filePath, q+1, high, rlen);
    }
}


