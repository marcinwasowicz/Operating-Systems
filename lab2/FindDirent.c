#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>

void appendFileName(char* s1, char* s2){
    strcat(s1, "/");
    strcat(s1, s2);
}

void cutFileName(char* s1, char* s2){
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    s1[l1-l2-1] = '\0';
}

bool omit(char* s){
    return strcmp(s, ".") == 0 || strcmp(s, "..") == 0;
}

void printFileInfo(char* path,char* fileName){
    appendFileName(path, fileName);
    struct stat buff;
    int n = stat(path, &buff);
    if(n!=0){
        printf("%s", "error: couldn't open file\n");
        return;
    }
    printf("%s%s%s", "path: ", path, "\n");
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

bool checkModificationTime(struct stat* buff, char* type, int num, time_t date){
    int diff = difftime(date, mktime(localtime(&(buff->st_mtime))));
    if(strcmp(type, "eq") == 0 && diff == 0){
        return true;
    }
    if(strcmp(type, "lt") == 0 && diff < 0){
        return true;
    }
    if(strcmp(type, "gt") == 0 && diff > 0){
        return true;
    }
    return false;
}

bool checkAccessTime(struct stat* buff, char* type, int num, time_t date){
    int diff = difftime(date, mktime(localtime(&(buff->st_atime))));
    if(strcmp(type, "eq") == 0 && diff == 0){
        return true;
    }
    if(strcmp(type, "lt") == 0 && diff < 0){
        return true;
    }
    if(strcmp(type, "gt") == 0 && diff > 0){
        return true;
    }
    return false;
}

void find(char* currPath,char* fileName, int depth, int maxDepth, bool* found){
    if(depth > maxDepth){
        return;
    }
    DIR* dir = opendir(currPath);
    if(!dir){
        closedir(dir);
        return;
    }
    struct dirent* child = readdir(dir);
    while(!(*found)&&child){
        if(omit(child->d_name)){
            child = readdir(dir);
            continue;
        }
        if(strcmp(fileName, child->d_name) == 0){
            printFileInfo(currPath, child->d_name);
            *found = true;
            break;
        }
        appendFileName(currPath, child->d_name);
        find(currPath, fileName, depth+1, maxDepth, found);
        cutFileName(currPath, child->d_name);
        child = readdir(dir);
    }
    closedir(dir);
}

void mtime(char* currPath, char* type, int num, time_t date){
    struct stat buff;
    stat(currPath, &buff);
    if(checkModificationTime(&buff, type, num, date)){
        printf("%s%s", currPath, " ");
        printf("%d%s", localtime(&(buff.st_mtime))->tm_mday, "\n");
    }
    DIR* dir = opendir(currPath);
    if(!dir){
        closedir(dir);
        return;
    }
    struct dirent* child = readdir(dir);
    while(child){
        if(omit(child->d_name)){
            child = readdir(dir);
            continue;
        }
        appendFileName(currPath, child->d_name);
        mtime(currPath, type, num, date);
        cutFileName(currPath, child->d_name);
        child = readdir(dir);
    }
    closedir(dir);
}

void atime(char* currPath, char* type, int num, time_t date){
    struct stat buff;
    stat(currPath, &buff);
    if(checkAccessTime(&buff, type, num, date)){
        printf("%s%s", currPath, " ");
        printf("%d%s", localtime(&(buff.st_atime))->tm_mday, "\n");
    }
    DIR* dir = opendir(currPath);
    if(!dir){
        closedir(dir);
        return;
    }
    struct dirent* child = readdir(dir);
    while(child){
        if(omit(child->d_name)){
            child = readdir(dir);
            continue;
        }
        appendFileName(currPath, child->d_name);
        atime(currPath, type, num, date);
        cutFileName(currPath, child->d_name);
        child = readdir(dir);
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
    char* path = (char*)malloc(sizeof(char)*300);
    strcat(path, argv[2]);
    if(strcmp(argv[1], "maxdepth") == 0){
        bool found = false;
        find(path, argv[3],1, atoi(argv[4]), &found);
    }
    else{
        time_t bareTime;
        time(&bareTime);
        struct tm* timeHandler = localtime(&bareTime);
        timeHandler->tm_mday -= atoi(argv[4]);
        if(strcmp(argv[1], "mtime") == 0){
            timeHandler->tm_mday--;
            mtime(path, argv[3], atoi(argv[4]), mktime(timeHandler));
        }
        else if(strcmp(argv[1], "atime") == 0){
            atime(path, argv[3], atoi(argv[4]), mktime(timeHandler));   
        }
    }
    free(path);
}
