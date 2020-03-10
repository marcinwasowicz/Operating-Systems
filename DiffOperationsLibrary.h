#ifndef HEADER_FILE
#define HEADER_FILE

#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdarg.h>
#include<stdio.h>

char* susbtr(char* string, int start, int length);

char* stringConcat(int num, ...);

char* readTextFileToString(char* fileName);

char* makeSysCall(char* name1, char* name2, char* outputFile);

typedef struct{
    char* name1;
    char* name2;
}filesPair;

filesPair* makeFilesPair(char* n1, char* n2);

void deletePair(filesPair* pair);

typedef struct{
    char** operations;
    int numberOfOperations;
    int rawSize;
}operationsBlock;

operationsBlock* makeBlock(char** operations, int numberOfOperations);

void printOneBlockOperation(operationsBlock* block, int index);

void printBlock(operationsBlock* block);

void deleteBlock(operationsBlock* block);

typedef struct{
    filesPair** arr;
    int size;
    int rawSize;
}filesSequence;

typedef struct{
    operationsBlock** arr;
    filesSequence* sequence;
    int rawSize;
    int head;
}blockArray;

bool isDigit(char c);

bool isThisBeginingOfOperations(char* string, int index);

operationsBlock* parseOperationsStream(char* stream);

operationsBlock* diffPair(filesPair* pair);

blockArray* makeBlockArrayFromScratch(int number, ...);

void printArray(blockArray* block);

void deleteBlockArray(blockArray* arr);

void deleteOneBlockFromArray(blockArray* arr, int index);

void deleteSingleOperation(operationsBlock* block, int index);

void addOneBlock(blockArray* bl, char* fileName1, char* filename2);

void updatePairSequenceAddition(blockArray* bl, char* fileName1, char* filename2);

void updatePairSequenceDeletion(blockArray* bl, int index);

#endif
