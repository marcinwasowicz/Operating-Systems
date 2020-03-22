#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>

void appendFileName(char* currPath, char* fileName){
    strcat(currPath, "/");
    strcat(currPath, fileName);
}

void cutFileName(char* currPath, char* fileName){
    int len1 = strlen(currPath);
    int len2 = strlen(fileName);
    currPath[len1-len2-1] = '\0';
}

bool omit(char* fileName){
    return strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0;
}

int main(int argc, char* argv[]){
    char currPath[500];
    int maxDepth = atoi(argv[2]);
    strcat(currPath, argv[1]);
    bool readed = false;
    int depth = 0;

    while(!readed && (depth <= maxDepth)){
        char* command = (char*)malloc(sizeof(char)*(strlen(currPath)+6));
        strcat(command, "ls -l ");
        strcat(command, currPath);
        int id = getpid();
        printf("%s %d\n", currPath, id);
        system(command);
        free(command);
        DIR* dir = opendir(currPath);
        if(!dir){
            break;
        }
        for(struct dirent* file = readdir(dir); file!=NULL; file=readdir(dir)){
            if(!omit(file->d_name)){
                appendFileName(currPath, file->d_name);
                fork();
                if(getpid()!=id){
                    readed = false;
                    depth++;
                    break;
                }  
                else{
                    cutFileName(currPath, file->d_name);
                    readed = true;
                }
            }
        }
        closedir(dir);
    }
    return 0;
}