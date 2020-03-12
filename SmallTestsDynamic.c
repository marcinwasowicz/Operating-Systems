#include <stdio.h>
#include<dlfcn.h>
#include<time.h>
#include "DiffOperationsLibrary.h"

int main(){
    void* lib = dlopen("./libDiff.so", RTLD_LAZY);
    blockArray* (*making)(int, ...);
    void (*deleteOne)(blockArray*, int);
    void (*addOne)(blockArray*, char*, char*);
    void (*deleteArray)(blockArray*);
    making = dlsym(lib, "makeBlockArrayFromScratch");
    deleteOne = dlsym(lib, "deleteOneBlockFromArray");
    addOne = dlsym(lib, "addOneBlock");
    deleteArray = dlsym(lib, "deleteBlockArray");
    clock_t startMakingSmall = clock();
    blockArray* small = making(4, "smallTests/file1.txt","smallTests/file2.txt","smallTests/file3.txt","smallTests/file4.txt");
    clock_t stopMakingSmall= clock();
    printf("%s%ld%s", "making small array required: ", stopMakingSmall - startMakingSmall, "time\n");
    clock_t startUpdates = clock();
    for(int i = 0; i<2; i++){
        (*deleteOne)(small, i);
        (*addOne)(small, "smallTests/file2.txt", "smallTests/file3.txt");
    }
    clock_t stopUpdates = clock();
    printf("%s%ld%s", "making updates on small array required: ", stopUpdates - startUpdates, "time\n");
    clock_t startDeletingSmall = clock();
    (*deleteArray)(small);
    clock_t stopDeletingSmall = clock();
    printf("%s%ld%s", "deleting small array required: ", stopDeletingSmall - startDeletingSmall, "time\n");
    dlclose(lib);
    return 0;
}