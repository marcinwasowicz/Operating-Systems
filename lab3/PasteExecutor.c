#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char* argv[]){

    char* lista = argv[1];
    int numOfProcesses = atoi(argv[3]);
    int numOfPairs = atoi(argv[2]);
    FILE* file = fopen(lista, "r");

    int managerPID = getpid();

    for(int i = 0; i<numOfPairs; i++){
        char matrixA[256];
        char matrixB[256];
        char matrixC[256];
        fscanf(file, "%s %s %s\n", matrixA, matrixB, matrixC);
        strcat(matrixA, "bin");
        strcat(matrixB, "bin");
        remove(matrixA);
        remove(matrixB);

        char* av[100];
        av[0] = (char*)malloc(sizeof(char)*256);
        strcpy(av[0], "paste");
        int j = 1;
        for(; j<=numOfProcesses; j++){
            av[j] = (char*)malloc(sizeof(char)*256);
            char buff[10];
            snprintf(buff, sizeof(buff), "%d", j);
            strcpy(av[j], matrixC);
            strcat(av[j],buff);
            FILE* f = fopen(av[j], "r");
            if(f==NULL){
                free(av[j]);
                break;
            }
        }
        av[j] = NULL;
        fork();
        if(getpid()!=managerPID){
            FILE* f = fopen(matrixC, "w+");
            int no = fileno(f);
            fclose(file);
            dup2(no, fileno(stdout));
            execvp("paste", av);
        }
        for(int k = 0; k<j; k++){
            free(av[k]);
        }
    }
    fclose(file);
}

