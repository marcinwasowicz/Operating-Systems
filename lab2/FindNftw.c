#define _XOPEN_SOURCE 500
#include<ftw.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<stdint.h>

#define FTW_CONTINUE 0
#define FTW_STOP 1
#define FTW_SKIP_SUBTREE 2
#define FTW_ACTIONRETVAL 16

int maxDepth;
char* root;
char* fileToSearch;
char* type;
int num;
time_t date;

bool compareEnding(char* filePath, char* name){
    int flen = strlen(filePath);
    int nlen = strlen(name);
    if(flen <= nlen){
        return false;
    }
    for(int i = flen-1, j = nlen-1; j>=0; i--, j--){
        if(name[j]!=filePath[i]){
            return false;
        }
    }
    return true;
}

void printFileInfo(char* path, struct stat* buff){
    printf("%s%s%s", "path: ", path, "\n");
    printf("%s%ld%s", "size in bytes: ", buff->st_size, "\n");
    printf("%s%ld%s", "number of links: ", buff->st_nlink, "\n");
    printf("%s%ld%s", "last access date: ", buff->st_atime, "\n");
    printf("%s%ld%s", "last modification date: ", buff->st_mtime, "\n");
    printf("%s", "file type: ");
    mode_t m = buff->st_mode;
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

bool checkModificationTime(struct stat* buff){
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

bool checkAccessTime(struct stat* buff){
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

int find(const char* filePath, const struct stat* fileData, int flags, struct FTW* ftwstruct){
    if(ftwstruct->level > maxDepth){
        return FTW_SKIP_SUBTREE;
    }
    if(compareEnding(filePath, fileToSearch)){
        printFileInfo(filePath, fileData);
        return FTW_STOP;
    }
    return FTW_CONTINUE;
}

int mtime(const char* filePath, const struct stat* fileData, int flags, struct FTW* ftwstruct){
    if(ftwstruct->level > maxDepth){
        return FTW_SKIP_SUBTREE;
    }
    if(checkModificationTime(filePath)){
        printFileInfo(filePath, fileData);
    }
    return FTW_CONTINUE;
}

int atime(const char* filePath, const struct stat* fileData, int flags, struct FTW* ftwstruct){
    if(ftwstruct->level > maxDepth){
        return FTW_SKIP_SUBTREE;
    }
    if(checkAccessTime(filePath)){
        printFileInfo(filePath, fileData);
    }
    return FTW_CONTINUE;
}

int main(int argc, char* argv[]){
    root = argv[2];
    maxDepth = atoi(argv[4]);
    if(strcmp(argv[1], "maxdepth") == 0){
        fileToSearch = argv[3];
        nftw(root, &find, maxDepth, FTW_ACTIONRETVAL);
    }
    else{
        time(&date);
        struct tm* timeHandler = localtime(&date);
        type = argv[3];
        num = atoi(argv[5]);
        if(strcmp(type, "gt") == 0){
            num++;
        }
        timeHandler->tm_mday -= num;
        date = mktime(timeHandler);
        if(strcmp(argv[1], "mtime") == 0){
            nftw(root, &mtime, maxDepth, FTW_ACTIONRETVAL);
        }
        else if(strcmp(argv[1], "atime") == 0){
            nftw(root, &atime, maxDepth, FTW_ACTIONRETVAL);
        }
    }
}




