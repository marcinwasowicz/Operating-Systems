#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int agrc, char* argv[]){
    int numPairs = atoi(argv[2]);
    char* list = argv[1];
    FILE* file = fopen(list, "r");
    for(int i = 0; i<numPairs; i++){
        char matrixA[256];
        char matrixB[256];
        char matrixC[256];
        char matrixCbin[256];
        int rowsA;
        int columnsA;
        int rowsB;
        int columnsB;
        fscanf(file, "%s %s %s\n", matrixA, matrixB, matrixC);
        FILE* a = fopen(matrixA, "r");
        FILE* b = fopen(matrixB, "r");
        fscanf(a, "%d", &rowsA);
        fscanf(a, "%d", &columnsA);
        fscanf(b, "%d", &rowsB);
        fscanf(b, "%d", &columnsB);
        fclose(a);
        fclose(b);
        strcpy(matrixCbin, matrixC);
        strcat(matrixA, "bin");
        strcat(matrixB, "bin");
        strcat(matrixCbin, "bin");
        remove(matrixA);
        remove(matrixB);
        FILE* out = fopen(matrixC, "w+");
        FILE* binar = fopen(matrixCbin, "r");

        int* buff = (int*)malloc(sizeof(int)*columnsB);
        for(int j = 0; j<rowsA; j++){
            fread(buff, sizeof(int), columnsB, binar);
            for(int k = 0; k<columnsB; k++){
                fprintf(out, "%d", buff[k]);
                fprintf(out, "%s", " ");
            }
            fprintf(out, "%s", "\n");
        }
        fclose(binar);
        fclose(out);
        remove(matrixCbin);
        free(buff);
    }
}