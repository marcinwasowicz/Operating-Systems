#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
// Simplest version at this moment. Just searches for a file and, if found, prints its statistics
typedef struct{
    DIR* dir;
    char* name;
}MyDIR;

MyDIR* construct(DIR* dir, char* name){
    MyDIR* res = (MyDIR*)malloc(sizeof(MyDIR));
    res->dir = dir;
    res->name = (char*)malloc(sizeof(char)*strlen(name));
    strcpy(res->name, name);
    return res;
}

void destruct(MyDIR* d){
    free(d->name);
    free(d);
}

typedef struct{
    MyDIR** list;
    int maxSize;
    int top;
}stack;

stack* init(int maxSize){
    stack* res = (stack*)malloc(sizeof(stack));
    res->list = (MyDIR**) malloc(sizeof(MyDIR*)*maxSize);
    res->maxSize = maxSize;
    res->top = -1;
    return res;
}

void deleteStack(stack* s){
    for(int i = 0; i<=s->top; i++){
        closedir(s->list[i]->dir);
        destruct(s->list[i]);
    }
    free(s->list);
    free(s);
}

bool isEmpty(stack* s){
    return s->top == -1;
}

bool isFull(stack* s){
    return s->top == s->maxSize-1;
}

void push(stack* s, MyDIR* el){
    if(isFull(s)){
        return;
    }
    s->top++;
    s->list[s->top] = el;
}

MyDIR* pop(stack* s){
    if(isEmpty(s)){
        return NULL;
    }
    s->top--;
    return s->list[s->top+1];
}

MyDIR* top(stack* s){
    if(isEmpty(s)){
        return NULL;
    }
    return s->list[s->top];
}

void cutString(char* s1, char* s2){
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    s1[l1-l2-1] = '\0';
}

void appendString(char* s1, char* s2){
    strcat(s1, "/");
    strcat(s1, s2);
}

bool omitt(char* s){
    return strcmp(s, ".") == 0 || strcmp(s, "..") == 0;
}

void printFileInfo(char* workspace,char* fileName){
    appendString(workspace, fileName);
    struct stat buff;
    int n = stat(workspace, &buff);
    if(n!=0){
        printf("%s", "error: couldn't open file\n");
        return;
    }
    printf("%s%s%s", "path: ", workspace, "\n");
    printf("%s%ld%s", "size in bytes: ", buff.st_size, "\n");
    printf("%s%ld%s", "number of links: ", buff.st_nlink, "\n");
    printf("%s%ld%s", "last access date: ", buff.st_atime, "\n");
    printf("%s%ld%s", "last modification date: ", buff.st_mtime, "\n");
    printf("%s", "file type: ");
    mode_t m = buff.st_mode;
    if(S_ISREG(m)){
        printf("%s", "file\n");
        return;
    }
    if(S_ISDIR(m)){
        printf("%s", "dir\n");
        return;
    }
    if(S_ISCHR(m)){
        printf("%s", "char dev\n");
        return;
    }
    if(S_ISBLK(m)){
        printf("%s", "block dev\n");
        return;
    }
    if(S_ISFIFO(m)){
        printf("%s", "fifo\n");
        return;
    }
    if(S_ISLNK(m)){
        printf("%s", "slink\n");
        return;
    }
    if(S_ISSOCK(m)){
        printf("%s", "sock\n");
        return;
    }
}

void find(char* startingDirectory, char* fileName, int maxDepth){
    DIR* dir = opendir(startingDirectory);
    if(dir == NULL){
        printf("%s", "error: invalid directory");
        closedir(dir);
        return;
    }
    char* workspace = (char*) malloc(sizeof(char)*200);
    strcat(workspace, startingDirectory);
    stack* S = init(maxDepth);
    push(S, construct(dir, startingDirectory));    
    while(!isEmpty(S)){
        struct dirent* temp = readdir(top(S)->dir);
        if(temp == NULL){
            closedir(top(S)->dir);
            MyDIR* d = pop(S);
            cutString(workspace, d->name);
            destruct(d);
        }
        else if(omitt(temp->d_name)){
            continue;
        }
        else{
            if(strcmp(temp->d_name, fileName) == 0){
                printFileInfo(workspace, fileName);
                break;
            }
            appendString(workspace, temp->d_name);
            DIR* el = opendir(workspace);
            if(el!=NULL){
                push(S,construct(el, temp->d_name));
            }
            else{
                cutString(workspace, temp->d_name);
            }
        }
    }
    free(workspace);
    deleteStack(S);
}

int main(int argc, char* argv[]){
    find(argv[1], argv[2], atoi(argv[3]));
    return 0;
}