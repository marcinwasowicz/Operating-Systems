#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>
#include<sys/resource.h>

int min(int x, int y){
    if(x<y){
        return x;
    }
    return y;
}

int max(int x, int y){
    if(x>y){
        return x;
    }
    return y;
}

void createTriple(char* a, char* b, char* c){
    
    char ma[256];
    char mb[256];
    char mc[256];
   
    strcpy(ma, a);
    strcpy(mb, b);
    strcpy(mc, c);

    strcat(ma, "bin");
    strcat(mb, "bin");
    strcat(mc, "bin");
     
    int rowsA;
    int columnsA;
    int rowsB;
    int columnsB;

    FILE* A = fopen(a, "r");
    FILE* B = fopen(b, "r");

    FILE* cA = fopen(ma, "w+");
    FILE* cB = fopen(mb, "w+");

    fscanf(A, "%d %d\n", &rowsA, &columnsA);
    fscanf(B, "%d %d\n", &rowsB, &columnsB);
    
    int* buffA = (int*)malloc(sizeof(int)*columnsA*rowsA);
    int* buffB = (int*)malloc(sizeof(int)*columnsB*rowsB);

    for(int i = 0; i<rowsA; i++){
        for(int j = 0; j<columnsA; j++){
            fscanf(A, "%d", &buffA[i*columnsA+j]);
        }
    }

    for(int i = 0; i<rowsB; i++){
        for(int j = 0; j<columnsB; j++){
            fscanf(B, "%d", &buffB[i*columnsB+j]);
        }
    }

    fwrite(buffA, sizeof(int), rowsA*columnsA, cA);
    fwrite(buffB, sizeof(int), rowsB*columnsB, cB);

    free(buffA);
    free(buffB);
    FILE* output = fopen(mc, "w+");

    fclose(B);
    fclose(A);
    fclose(cB);
    fclose(cA);
    fclose(output);
}

void multiplyOneColumn(char* matrixA, char* matrixB,char* outputMatrix, int rowsA, int columnsA, int columnsB, int col){
    FILE* mA = fopen(matrixA, "r");
    FILE* mB = fopen(matrixB, "r");
    FILE* out = fopen(outputMatrix, "r+");

    int* buffA = (int*)malloc(sizeof(int)*columnsA);
    int buffB = 0;
    int buffOut = 0;

    for(int i = 0; i<rowsA; i++){
        fread(buffA, sizeof(int), columnsA, mA);
        buffOut = 0;
        for(int j = 0; j<columnsA; j++){
            buffB = 0;
            fseek(mB, sizeof(int)*(j*columnsB+col), 0);
            fread(&buffB, sizeof(int), 1, mB);
            int temp = buffA[j] * buffB;
            buffOut+=temp;
        }
        fseek(out, sizeof(int)*(i*columnsB+col), 0);
        fwrite(&buffOut, sizeof(int), 1, out);
    }

    free(buffA);
    fclose(mA);
    fclose(mB);
    fclose(out);

}

void multiplyFragment(char* matrixA, char* matrixB,char* outputMatrix, int rowsA, int columnsA, int columnsB, int idx, int num){
    num = min(num, columnsB);
    if(idx > num){
        return;
    }
    
    int work = columnsB/num;
    int res = columnsB%num;
    int from = (idx-1)*work + min(res, idx-1);
    int to = from + work + min(1, max(0, res - (idx-1))) - 1; 

    for(int i = from; i<=to; i++){
        multiplyOneColumn(matrixA, matrixB, outputMatrix, rowsA, columnsA, columnsB, i);
    }
}

void multiplyFragmentToFile(char* matrixA, char* matrixB, char* out, int rowsA, int columnsA, int columnsB, int idx, int num){
    num = min(num, columnsB);
    if(idx > num){
        return;
    }
    
    int work = columnsB/num;
    int res = columnsB%num;
    int from = (idx-1)*work + min(res, idx-1);
    int to = from + work + min(1, max(0, res - (idx-1))) - 1;

    FILE* a = fopen(matrixA, "r");
    FILE* b = fopen(matrixB, "r");
    FILE* o = fopen(out, "w+");

    int* buffA = (int*)malloc(sizeof(int)*columnsA);
    int buffB;
    int* buffOut = (int*)malloc(sizeof(int)*(to-from+1));

    for(int i = 0; i<rowsA; i++){
        fread(buffA, sizeof(int), columnsA, a);
        for(int col = from; col<=to; col++){
            buffOut[col-from] = 0;
            for(int j = 0; j<columnsA; j++){
                fseek(b, sizeof(int)*(j*columnsB+col), 0);
                fread(&buffB, sizeof(int), 1, b);
                buffOut[col-from]+=(buffA[j]*buffB);
            }
        }
        for(int col = from; col<to; col++){
            fprintf(o, "%d ", buffOut[col-from]);
        }
        fprintf(o, "%d\n", buffOut[to-from]);
    }

    fclose(a);
    fclose(b);
    fclose(o);
    free(buffA);
    free(buffOut);
}

void multiplyFragmentsFromList(char* lista, int idx, int numOfPairs, int numOfProcesses, int* multiplications, int seconds){
    FILE* file = fopen(lista, "r");
    int begin = time(NULL);
    for(; (*multiplications)<numOfPairs; (*multiplications)++){
        char matrixA[256];
        char matrixB[256];
        char matrixC[256];
        int rowsA;
        int columnsA;
        int rowsB;
        int columnsB;
        fscanf(file, "%s %s %s\n", matrixA, matrixB, matrixC);
        FILE* a = fopen(matrixA, "r");
        FILE* b = fopen(matrixB, "r");
        fscanf(a, "%d %d\n", &rowsA, &columnsA);
        fscanf(b, "%d %d\n", &rowsB, &columnsB);
        fclose(a);
        fclose(b);
        strcat(matrixA, "bin");
        strcat(matrixB, "bin");
        strcat(matrixC, "bin");
        multiplyFragment(matrixA, matrixB, matrixC, rowsA, columnsA, columnsB, idx, numOfProcesses);
        if(time(NULL) - begin >=seconds){
            break;
        }
    }
    fclose(file);
}

void multiplyFragmentsFromListToFiles(char* lista, int idx, int numOfPairs, int numOfProcesses, int* multiplications, int seconds){
    FILE* file = fopen(lista, "r");
    int begin = time(NULL);
    for(; (*multiplications)<numOfPairs; (*multiplications)++){
        char matrixA[256];
        char matrixB[256];
        char matrixC[256];
        int rowsA;
        int columnsA;
        int rowsB;
        int columnsB;
        fscanf(file, "%s %s %s\n", matrixA, matrixB, matrixC);
        FILE* a = fopen(matrixA, "r");
        FILE* b = fopen(matrixB, "r");
        fscanf(a, "%d %d\n", &rowsA, &columnsA);
        fscanf(b, "%d %d\n", &rowsB, &columnsB);
        fclose(a);
        fclose(b);
        strcat(matrixA, "bin");
        strcat(matrixB, "bin");
        char buff[10];
        snprintf(buff, sizeof(buff), "%d", idx);
        strcat(matrixC, buff);
        multiplyFragmentToFile(matrixA, matrixB, matrixC, rowsA, columnsA, columnsB, idx, numOfProcesses);
        if(time(NULL) - begin >= seconds){
            break;
        }
    }
    fclose(file);
}

void createBinaries(char* list, int numOfPairs){
    FILE* file = fopen(list, "r");
    for(int i = 0; i<numOfPairs; i++){
        char matrixA[256];
        char matrixB[256];
        char matrixC[256];
        fscanf(file, "%s %s %s\n", matrixA, matrixB, matrixC);
        createTriple(matrixA, matrixB, matrixC);
    }
    fclose(file);
}

void createBinariesSlicesVersion(char* list, int numOfPairs){
    FILE* file = fopen(list, "r");
    for(int i = 0; i<numOfPairs; i++){
        char matrixA[256];
        char matrixB[256];
        char matrixC[256];
        fscanf(file, "%s %s %s\n", matrixA, matrixB, matrixC);
        createTriple(matrixA, matrixB, matrixC);
        strcat(matrixC, "bin");
        remove(matrixC);
    }
    fclose(file);
}

void setLimits(int seconds, int vmemory){
    struct rlimit timeLimit;
    struct rlimit memoryLimit;
    timeLimit.rlim_cur = seconds;
    timeLimit.rlim_max = seconds;
    memoryLimit.rlim_cur = vmemory;
    memoryLimit.rlim_max = vmemory;
    setrlimit(RLIMIT_CPU, &timeLimit);
    setrlimit(RLIMIT_AS, &memoryLimit);
}

void printUsage(struct rusage* usage){
    printf("%s %ld %s\n","used", usage->ru_utime.tv_usec, "user time");
    printf("%s %ld %s\n","used", usage->ru_stime.tv_sec, "system time");
}

int main(int argc, char* argv[]){
    int numOfPairs = atoi(argv[2]);
    int numOfProcesses = atoi(argv[3]);
    int managerPID = getpid();
    int seconds = atoi(argv[4]);
    int vmemory = atoi(argv[5]);
    //createBinariesSlicesVersion(argv[1], numOfPairs);
    createBinaries(argv[1], numOfPairs);
    for(int proces = 0; proces<=numOfProcesses; proces++){
        if(managerPID!=getpid()){
            setLimits(seconds, vmemory);
            //multiplyFragmentsFromListToFiles(argv[1], proces, numOfPairs, numOfProcesses, &mult, seconds);
            int mult = 0;
            multiplyFragmentsFromList(argv[1], proces, numOfPairs, numOfProcesses, &mult, seconds);
            exit(mult);
        }
        if(proces<numOfProcesses){
            fork();
        }
    }
    int stat;
    struct rusage usage;
    for(int i = 0; i<numOfProcesses; i++){
        pid_t pid = wait3(&stat, 0,&usage);
        printf("%s %d %s %d %s\n", "process of PID", pid, "completed", WEXITSTATUS(stat), "multiplications");
        printUsage(&usage);
    }
    return 0;
}
