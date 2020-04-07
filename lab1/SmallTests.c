#include "DiffOperationsLibrary.h"
#include<time.h>
int main(){
    clock_t startMakingSmall = clock();
    blockArray* small = makeBlockArrayFromScratch(4, "smallTests/file1.txt","smallTests/file2.txt","smallTests/file3.txt","smallTests/file4.txt");
    clock_t stopMakingSmall= clock();
    printf("%s%ld%s", "making small array required: ", stopMakingSmall - startMakingSmall, "time\n");
    clock_t startUpdates = clock();
    for(int i = 0; i<2; i++){
        deleteOneBlockFromArray(small, i);
        addOneBlock(small, "smallTests/file2.txt", "smallTests/file3.txt");
    }
    clock_t stopUpdates = clock();
    printf("%s%ld%s", "making updates on small array required: ", stopUpdates - startUpdates, "time\n");
    clock_t startDeletingSmall = clock();
    deleteBlockArray(small);
    clock_t stopDeletingSmall = clock();
    printf("%s%ld%s", "deleting small array required: ", stopDeletingSmall - startDeletingSmall, "time\n");
    return 0;
}
