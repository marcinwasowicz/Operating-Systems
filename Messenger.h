#ifndef _MESSENGER_HEADER
#define _MESSENGER_HEADER

#define MAX_CLIENTS 10
#define MAX_MESSAGE 512
#define ANY_TYPE -1000
#define FLAG 0 | 0666
#define MESSAGE_SIZE sizeof(int)*2
#define FRIEND_MESSAGE_SIZE 512*sizeof(char)
#define PATH getenv("HOME")

#define TRUE 1

#define SERVER 1

#define GET_ID 0
#define STOP 1
#define INIT 2
#define DISCONNECT 3
#define LIST 4
#define CONNECT 5
#define READ_MAILBOX 6
#define TEXT_FRIEND 7

typedef struct{
    int clientID;
    int cliendtQueueID;
    int connected;
    int pid;
}client;

typedef struct{
    long type;
    int info;
    int ID;
}message;

typedef struct{
    long type;
    char text[MAX_MESSAGE];
}friendMessage;

#endif