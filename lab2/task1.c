#include "QuickSortLibrary.h"
#include<sys/times.h>
#include<time.h>
#include<unistd.h>
int main(int argc, char* argv[]){
    srand(time(NULL));
    struct tms start;
    struct tms end;
    int clockTicks = sysconf(_SC_CLK_TCK);
    if(strcmp(argv[1], "sys") == 0){
        if(strcmp(argv[2], "generate") == 0){
            GenerateFileWithSystem(argv[3], atoi(argv[4]), atoi(argv[5]));
        }
        else if(strcmp(argv[2], "sort") == 0){
            times(&start);
            QuickSortSystem(argv[3], 0, atoi(argv[4]) -1, atoi(argv[5]));
            times(&end);
            printf("%s%.2f%s%.2f%s", "sorting with system functions took: ", 1.0*(end.tms_utime-start.tms_utime)/clockTicks, " user time and: ",
            1.0*(end.tms_stime-start.tms_stime)/clockTicks, " of system time\n");
        }
        else if(strcmp(argv[2], "copy") == 0){
            times(&start);
            CopyWithSystem(argv[3], argv[6], atoi(argv[4]), atoi(argv[5]));
            times(&end);
            printf("%s%.2f%s%.2f%s", "copying with system functions took: ", 1.0*(end.tms_utime-start.tms_utime)/clockTicks, " user time and: ",
            1.0*(end.tms_stime-start.tms_stime)/clockTicks, " of system time\n");
        }
    }
    else if(strcmp(argv[1], "lib") == 0){
        if(strcmp(argv[2], "generate") == 0){
            GenerateFileWithLibrary(argv[3], atoi(argv[4]), atoi(argv[5]));
        }
        else if(strcmp(argv[2], "sort") == 0){
            times(&start);
            QuickSortLibrary(argv[3], 0, atoi(argv[4]) -1, atoi(argv[5]));
            times(&end);
            printf("%s%.2f%s%.2f%s", "sorting with library functions took: ", 1.0*(end.tms_utime-start.tms_utime)/clockTicks, " user time and: ",
            1.0*(end.tms_stime-start.tms_stime)/clockTicks, " of system time\n");
        }
        else if(strcmp(argv[2], "copy") == 0){
            times(&start);
            CopyWithLibrary(argv[3], argv[6], atoi(argv[4]), atoi(argv[5]));
            times(&end);
            printf("%s%.2f%s%.2f%s", "copying with library functions took: ", 1.0*(end.tms_utime-start.tms_utime)/clockTicks, " user time and: ",
            1.0*(end.tms_stime-start.tms_stime)/clockTicks, " of system time\n");
        }
    }
}
