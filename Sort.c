#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[]){

    char command[500];

    strcat(command, "cat");
    strcat(command, " ");
    strcat(command, argv[1]);
    strcat(command, " | sort");

    FILE* pip = popen(command, "w");
    pclose(pip);

    return 0;
}