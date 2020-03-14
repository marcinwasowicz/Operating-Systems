#ifndef HEADER_FILE
#define HEADER_FILE

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdarg.h>


char* stringConcat(int num, ...);

void GenerateFileWithSystem(char* name, int count, int rlen);

void swapFileRecordsSystem(char* buff1, char* buff2, int pos1, int pos2, int fd, int rlen);

int partitionSystem(int low, int high, char* filePath, int rlen);

void QuickSortSystem(char* filePath, int low, int high, int rlen);

void GenerateFileWithLibrary(char* name, int count, int rlen);

void swapFileRecordsLibrary(char* buff1, char* buff2, int pos1, int pos2, FILE* f, int rlen);

int partitionLibrary(int low, int high, char* filePath, int rlen);

void QuickSortLibrary(char* filePath, int low, int high, int rlen);
// not implemented yet
void CopyWithSystem(char* fileFrom, char* fileTo, int count, int rlen);
// not implemented yet
void CopyWithLibrary(char* fileFrom, char* fileTo, int count, int rlen);

#endif