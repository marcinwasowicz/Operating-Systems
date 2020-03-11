#include "DiffOperationsLibrary.h"
#include<time.h>
int main(){
    clock_t startMakingMedium = clock();
    blockArray* medium = makeBlockArrayFromScratch(10, "mediumTests/file0.txt", "mediumTests/file1.txt",
    "mediumTests/file2.txt", "mediumTests/file3.txt", "mediumTests/file4.txt", "mediumTests/file5.txt",
    "mediumTests/file6.txt", "mediumTests/file7.txt", "mediumTests/file8.txt", "mediumTests/file9.txt");
    clock_t stopMakingMedium = clock();
    printf("%s%ld%s", "making medium array required: ", stopMakingMedium - startMakingMedium, "time\n");
    clock_t startUpdates = clock();
    for(int i = 0; i<5; i++){
        deleteOneBlockFromArray(medium, i);
        addOneBlock(medium, "mediumTests/file5.txt", "mediumTests/file6.txt");
    }
    clock_t stopUpdates = clock();
    printf("%s%ld%s", "making updates on medium array required: ", stopUpdates - startUpdates, "time\n");
    clock_t startDeletingMedium = clock();
    deleteBlockArray(medium);
    clock_t stopDeletingMedium = clock();
    printf("%s%ld%s", "deleting medium array required: ", stopDeletingMedium - startDeletingMedium, "time\n");
    return 0;
}