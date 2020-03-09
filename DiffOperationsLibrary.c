#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdarg.h>
#include<stdio.h>
// basic operations on strings and files
char* susbtr(char* string, int start, int length){ // działa
    char* result = (char*) malloc(sizeof(char)*length);
    for(int i = start, j = 0; j<length; i++, j++){
        result[j] = string[i];
    }
    return result;
}

char* stringConcat(int num, ...){// działa
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

char* readTextFileToString(char* fileName){// działa
    FILE* file = fopen(fileName, "r");
    int count = 0;
    for(char curr = fgetc(file); curr!=EOF; curr = fgetc(file)){
        count++;
    }
    char* result = (char*)malloc(sizeof(char)*count);
    rewind(file);
    count = 0;
    for(char curr = fgetc(file); curr!=EOF; curr = fgetc(file)){
       result[count] = curr;
       count++;
    }
    fclose(file);
    remove(fileName);
    return result;
}

char* makeSysCall(char* name1, char* name2, char* outputFile){// działa
    char* call = stringConcat(6, "diff ", name1, " ", name2, " > ", outputFile);
    return call;
}
// structures and their constructors
typedef struct{
    char* name1;
    char* name2;
}filesPair;

filesPair* makeFilesPair(char* n1, char* n2){
    filesPair* res = (filesPair*) malloc(sizeof(filesPair));
    res->name1 = (char*)malloc(sizeof(char)*strlen(n1));
    res->name2 = (char*)malloc(sizeof(char)*strlen(n2));
    strcpy(res->name1, n1);
    strcpy(res->name2,n2);
    return res;
}

void deletePair(filesPair* pair){
    if(pair == NULL){
        return;
    }
    free(pair->name1);
    free(pair->name2);
    free(pair);
}

typedef struct{
    char** operations;
    int numberOfOperations;
    int rawSize;
}operationsBlock;

operationsBlock* makeBlock(char** operations, int numberOfOperations){
    operationsBlock* result = (operationsBlock*) malloc(sizeof(operationsBlock));
    result->operations = operations;
    result->numberOfOperations = numberOfOperations;
    result->rawSize = numberOfOperations;
    return result;
}

void printOneBlockOperation(operationsBlock* block, int index){
    if(index <0 || index >= block->numberOfOperations){
        printf("%s", "error: invalid index");
        return;
    }
    printf("%s", block->operations[index]);
}

void printBlock(operationsBlock* block){
    for(int i = 0; i<block->numberOfOperations; i++){
        printOneBlockOperation(block, i);
    }
}

void deleteBlock(operationsBlock* block){
    if(block == NULL){
        return;
    }
    for(int i = 0; i<block->numberOfOperations; i++){
        free(block->operations[i]);
    }
    free(block->operations);
    free(block);
}

typedef struct{
    operationsBlock** arr;
    filesPair** sequence;
    int rawSize;
    int head;
}blockArray;


// operations on "diff" output
bool isDigit(char c){// działa
    return (c-'0' >= 0 && '9' - c >=0);
}

bool isThisBeginingOfOperations(char* string, int index){// działa
    if(index == 0){
        return true;
    }
    return isDigit(string[index]) && (string[index-1] == '\n');
}

operationsBlock* parseOperationsStream(char* stream){// działa
    int len = strlen(stream);
    int numOfOperations = 0;
    char** operations;
    for(int i = 0; i<len; i++){
        if(isThisBeginingOfOperations(stream, i)){
            numOfOperations++;
        }
    }
    operations = (char**)malloc(sizeof(char*)*numOfOperations);
    int iterator = 0;
    for(int i = 0; i<len;){
        if(isThisBeginingOfOperations(stream, i)){
            int j = i+1;
            for(; j<len && !isThisBeginingOfOperations(stream, j); j++){}
            operations[iterator] = susbtr(stream, i, j-i);
            iterator++;
            i = j;
        }
        else{
            i++;
        }
    }
    return makeBlock(operations, numOfOperations);
}

operationsBlock* diffPair(filesPair* pair){// działa 
    char tempFile[] = "temp.txt";
    char* call = makeSysCall(pair->name1, pair->name2, tempFile);
    system(call);
    char* tempResult = readTextFileToString(tempFile);
    operationsBlock* result = parseOperationsStream(tempResult);
    free(tempResult);
    free(call);
    return result;
}

blockArray* makeBlockArrayFromScratch(int number, ...){
    operationsBlock** arr = (operationsBlock**) malloc(sizeof(operationsBlock*)*(number/2));
    filesPair** sequence = (filesPair**) malloc(sizeof(filesPair*)*(number/2));
    va_list list;
    va_start(list, number);
    for(int i = 0; i<number/2; i++){
        char* file1 = va_arg(list, char*);
        char* file2 = va_arg(list, char*);
        sequence[i] = makeFilesPair(file1, file2);
        arr[i] = diffPair(sequence[i]);
    }
    va_end(list);
    blockArray* result = (blockArray*) malloc(sizeof(blockArray));
    result->arr = arr;
    result->sequence = sequence;
    result->head = number/2;
    result->rawSize = number/2;
    return result;
}

void deleteBlockArray(blockArray* arr){
    for(int i = 0; i<arr->rawSize; i++){
        deletePair(arr->sequence[i]);
        deleteBlock(arr->arr[i]);
    }
    free(arr->arr);
    free(arr->sequence);
    free(arr);
}

void deleteOneBlockFromArray(blockArray* arr, int index){
    if(index == arr->head-1){
        arr->head--;
        deleteBlock(arr->arr[arr->head]);
    }
    else{
        arr->head--;
        deleteBlock(arr->arr[index]);
        arr->arr[index] = arr->arr[arr->head];
        arr->arr[arr->head] = NULL;
    }
    if(arr->head <= arr->rawSize/4){
        operationsBlock** temp = (operationsBlock**) malloc(sizeof(operationsBlock*) * (arr->rawSize/2));
        for(int i = 0; i<arr->head; i++){
            temp[i] = arr->arr[i];
            arr->arr[i] = NULL;
        }
        arr->rawSize/=2;
        operationsBlock** temp2 = arr->arr;
        arr->arr = temp;
        free(temp2);
    }
}

void deleteSingleOperation(operationsBlock* block, int index){
    if(index == block->numberOfOperations-1){
        block->numberOfOperations--;
        free(block->operations[block->numberOfOperations]);
        block->operations[block->numberOfOperations] = NULL;
    }
    else{
        block->numberOfOperations--;
        free(block->operations[index]);
        block->operations[index] = block->operations[block->numberOfOperations];
        block->operations[block->numberOfOperations] = NULL;
    }
    if(block->numberOfOperations <= block->rawSize/4){
        char** temp = (char**) malloc(sizeof(char*)*(block->rawSize/2));
        for(int i = 0; i<block->numberOfOperations; i++){
            temp[i] = block->operations[i];
            block->operations[i] = NULL;
        }
        block->rawSize/=2;
        char** temp2 = block->operations;
        block->operations = temp;
        free(temp2);
    }
}

void addOneBlock(blockArray* bl, char* fileName1, char* fileName2){
    filesPair* tempPair = makeFilesPair(fileName1, fileName2);
    operationsBlock* block = diffPair(tempPair);
    if(bl->head == bl->rawSize){
        operationsBlock** temp = (operationsBlock**) malloc(sizeof(operationsBlock*)*bl->rawSize*2);
        for(int i = 0; i<bl->head; i++){
            temp[i] = bl->arr[i];
            bl->arr[i] = NULL;
        }
        operationsBlock** temp2 = bl->arr;
        bl->arr = temp;
        bl->rawSize*=2;
        free(temp2);
    }
    bl->arr[bl->head] = block;
    bl->head++;
    deletePair(tempPair);
}


