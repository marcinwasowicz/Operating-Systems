#ifndef MESSENGER_POSIX_HEADER

#define MAX_CLIENTS 10
#define MAX_MESSAGE 512
#define MESSAGE_SIZE sizeof(int) + sizeof(mqd_t) + 512*sizeof(char)
#define SERVER_PATH "/serverposix"
#define FLAG 0666
#define MAX_MESSAGE_NUM 10

#define TRUE 1

#define GET_ID 0
#define STOP 7
#define INIT 6
#define DISCONNECT 5
#define LIST 4
#define CONNECT 3
#define READ_MAILBOX 2
#define TEXT_FRIEND 1

typedef struct{
    int clientID;
    int cliendtQueueID;
    int connected;
    int pid;
    char queuePath[256];
}client;

typedef struct{
    int queueInfo;
    int ID;
    char text[MAX_MESSAGE];
}message;

#endif 