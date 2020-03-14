#include "zadanie1Library.h"
#include<time.h>

int main(int argc, char* argv[]){
    char* nameSystem = stringConcat(2, argv[2], "System.txt");
    char* nameLibrary = stringConcat(2, argv[2], "Library.txt");
    if(argc == 5){
        int count = atoi(argv[3]);
        int rlen = atoi(argv[4]);
        if(strcmp(argv[1], "generate") == 0){
            srand(time(NULL));
            GenerateFileWithSystem(nameSystem, count, rlen);
            GenerateFileWithLibrary(nameLibrary, count, rlen); 
        }
        else if(strcmp("sort", argv[1]) == 0){
            QuickSortSystem(nameSystem, 0, count-1, rlen);
            QuickSortLibrary(nameLibrary, 0, count-1, rlen);  
        }
    }
    else if(strcmp("copy", argv[1]) == 0){
        CopyWithSystem(nameSystem, stringConcat(2, argv[3],"System.txt"), atoi(argv[4]), atoi(argv[5]));
        CopyWithLibrary(nameLibrary, stringConcat(2, argv[3],"Library.txt"), atoi(argv[4]), atoi(argv[5]));
    }
    free(nameSystem);
    free(nameLibrary);
    return 0;
}
