#include "DiffOperationsLibrary.h"
#include<time.h>
#include<dlfcn.h>

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
    clock_t startMakingLarge = clock();
    blockArray* large = making(20, "largeTests/file0.txt", "largeTests/file1.txt", "largeTests/file2.txt", "largeTests/file3.txt"
    , "largeTests/file4.txt", "largeTests/file5.txt", "largeTests/file6.txt", "largeTests/file7.txt"
    , "largeTests/file8.txt", "largeTests/file9.txt", "largeTests/file10.txt", "largeTests/file11.txt"
    , "largeTests/file12.txt", "largeTests/file13.txt", "largeTests/file14.txt", "largeTests/file15.txt"
    , "largeTests/file16.txt", "largeTests/file17.txt", "largeTests/file18.txt", "largeTests/file19.txt");
    clock_t stopMakingLarge = clock();
    printf("%s%ld%s", "making large array required: ", stopMakingLarge - startMakingLarge, "time\n");
    clock_t startUpdates = clock();
    for(int i = 0; i<10; i++){
        deleteOne(large, i);
        addOne(large, "largeTests/file17.txt", "largeTests/file18.txt");
    }
    clock_t stopUpdates = clock();
    printf("%s%ld%s", "making updates on large array required: ", stopUpdates - startUpdates, "time\n");
    clock_t startDeletingLarge = clock();
    deleteArray(large);
    clock_t stopDeletingLarge = clock();
    printf("%s%ld%s", "deleting large array required: ", stopDeletingLarge - startDeletingLarge, "time\n");
    dlclose(lib);
    return 0;
}