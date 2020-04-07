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
    clock_t startMakingMedium = clock();
    blockArray* medium = making(10, "mediumTests/file0.txt", "mediumTests/file1.txt",
    "mediumTests/file2.txt", "mediumTests/file3.txt", "mediumTests/file4.txt", "mediumTests/file5.txt",
    "mediumTests/file6.txt", "mediumTests/file7.txt", "mediumTests/file8.txt", "mediumTests/file9.txt");
    clock_t stopMakingMedium = clock();
    printf("%s%ld%s", "making medium array required: ", stopMakingMedium - startMakingMedium, "time\n");
    clock_t startUpdates = clock();
    for(int i = 0; i<5; i++){
        deleteOne(medium, i);
        addOne(medium, "mediumTests/file5.txt", "mediumTests/file6.txt");
    }
    clock_t stopUpdates = clock();
    printf("%s%ld%s", "making updates on medium array required: ", stopUpdates - startUpdates, "time\n");
    clock_t startDeletingMedium = clock();
    deleteArray(medium);
    clock_t stopDeletingMedium = clock();
    printf("%s%ld%s", "deleting medium array required: ", stopDeletingMedium - startDeletingMedium, "time\n");
    dlclose(lib);
    return 0;
}
