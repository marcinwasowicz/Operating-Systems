#include "DiffOperationsLibrary.h"
#include<time.h>
int main(){
    clock_t startMakingLarge = clock();
    blockArray* large = makeBlockArrayFromScratch(20, "largeTests/file0.txt", "largeTests/file1.txt", "largeTests/file2.txt", "largeTests/file3.txt"
    , "largeTests/file4.txt", "largeTests/file5.txt", "largeTests/file6.txt", "largeTests/file7.txt"
    , "largeTests/file8.txt", "largeTests/file9.txt", "largeTests/file10.txt", "largeTests/file11.txt"
    , "largeTests/file12.txt", "largeTests/file13.txt", "largeTests/file14.txt", "largeTests/file15.txt"
    , "largeTests/file16.txt", "largeTests/file17.txt", "largeTests/file18.txt", "largeTests/file19.txt");
    clock_t stopMakingLarge = clock();
    printf("%s%ld%s", "making large array required: ", stopMakingLarge - startMakingLarge, "time\n");
    clock_t startUpdates = clock();
    for(int i = 0; i<10; i++){
        deleteOneBlockFromArray(large, i);
        addOneBlock(large, "largeTests/file17.txt", "largeTests/file18.txt");
    }
    clock_t stopUpdates = clock();
    printf("%s%ld%s", "making updates on large array required: ", stopUpdates - startUpdates, "time\n");
    clock_t startDeletingLarge = clock();
    deleteBlockArray(large);
    clock_t stopDeletingLarge = clock();
    printf("%s%ld%s", "deleting large array required: ", stopDeletingLarge - startDeletingLarge, "time\n");
    return 0;
}
