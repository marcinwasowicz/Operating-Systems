#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>
#include<string.h>

#define MAXARG 12
#define EOL '\n'

typedef struct{
    char path[256];
    char* args[MAXARG];
}execParams;

void cleanExecParams(execParams* params){
    for(int i = 0; params->args[i]!=NULL; i++){
        free(params->args[i]);
    }
}

void handlePipesBegin(int outPipe[2], int inPipe[2]){
    close(outPipe[0]);
    dup2(outPipe[1], STDOUT_FILENO);
}

void handlePipesInter(int outPipe[2], int inPipe[2]){
    close(inPipe[1]);
    close(outPipe[0]);
    dup2(outPipe[1], STDOUT_FILENO);
    dup2(inPipe[0], STDIN_FILENO);
}

void handlePipesEnd(int outPipe[2], int inPipe[2]){
    close(outPipe[0]);
    close(outPipe[1]);
    close(inPipe[1]);
    dup2(inPipe[0], STDIN_FILENO);
}

void movePipes(int outPipe[2], int inPipe[2]){
    close(inPipe[1]);
    inPipe[0] = outPipe[0];
    inPipe[1] = outPipe[1];
    pipe(outPipe);
}

void waitChildren(int n){
    for(int i = 0; i<n; i++){
        wait(NULL);
    }
}

void nextProcess(char mode, int* proc){
    (*proc)++;
    if(mode == EOL || mode == EOF){
        waitChildren(*proc);
        (*proc) = 0;
    }
}

void action(execParams* params){
    execvp(params->path, params->args);
}

bool checkChar(char c){
    return c != ' ' && c != EOL && c != EOF && c != '|';
}

//assume that every line of the file (also the last one!) ends with '\n'. will not work otherwise

char getOneExecParams(execParams* params, FILE* file){
    char fileIter;
    int nameIter = 0;
    int argsIter = 0;

    for(fileIter = fgetc(file); checkChar(fileIter); fileIter = fgetc(file)){
        params->path[nameIter] = fileIter;
        nameIter++;
    }

    if(fileIter == EOF){
        return EOF;
    }

    params->path[nameIter] = '\0';
    params->args[argsIter] = (char*)malloc(sizeof(char)*256);
    strcpy(params->args[argsIter], params->path);
    argsIter++;

    if(fileIter!=EOL){
        for(fileIter = fgetc(file); checkChar(fileIter); fileIter = fgetc(file)){
            params->args[argsIter] = (char*)malloc(sizeof(char)*256);
            int i = 0;
            for(;checkChar(fileIter);fileIter = fgetc(file)){
                params->args[argsIter][i] = fileIter;
                i++;
            }
            params->args[argsIter][i] = '\0';
            argsIter++;
            if(fileIter == EOL || fileIter == EOF){
                break;
            }
        }
        if(fileIter == '|'){
            fgetc(file);
        }
    }

    params->args[argsIter] = NULL;
    return fileIter;
}

int main(int argc, char* argv[]){

    FILE* file = fopen(argv[1], "r");

    int inPipe[2];
    int outPipe[2];
    pipe(outPipe);

    execParams params;
    int proc = 0;

    for(char mode = getOneExecParams(&params, file);mode != EOF;mode = getOneExecParams(&params, file)){
        pid_t pid = fork();
        if(pid == 0){
            if(mode == EOL || mode == EOF){
                handlePipesEnd(outPipe, inPipe);
            }
            else if(proc == 0){
                handlePipesBegin(outPipe, inPipe);
            }
            else{
                handlePipesInter(outPipe, inPipe);
            }
            action(&params);
        }
        else{
            movePipes(outPipe, inPipe);
            cleanExecParams(&params);
            nextProcess(mode, &proc);
        }
    }

    fclose(file);
    return 0;
}
