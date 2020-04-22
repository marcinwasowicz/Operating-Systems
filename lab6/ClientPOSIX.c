#define _POSIX_C_SOURCE 200809L
#include "MessengerPOSIX.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<string.h>
#include<mqueue.h>
#include<errno.h>
#include<stdbool.h>

mqd_t myQueue = -1;
mqd_t serverQueue = -1;
mqd_t friendQueue = -1;
pid_t myPid = -1;
pid_t friendPid = -1;
int myID = -1;
char* myQueuePath;
unsigned int priroty;
message messageBuffer;
bool signalFixing = false;

void setMessage(int ID, int queueInfo, char* text){
    messageBuffer.ID = ID;
    messageBuffer.queueInfo = queueInfo;
    if(text != NULL){
        strcpy(messageBuffer.text, text);
    }
}

void deleteQueue(){
    mq_close(myQueue);
    mq_unlink(myQueuePath);
}

void handleDisconnectionSignal(int sig){
    if(friendPid != -1 && friendQueue != -1){
        mq_close(friendQueue);
        friendPid = -1;
        friendQueue = -1;
    }
    setMessage(myID, myQueue, NULL);
    mq_send(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, DISCONNECT);
    signalFixing = true;
}

void handleConnectionSignal(int sig){
    if(friendPid == -1 || friendQueue == -1){
        mq_receive(myQueue, (char*) &messageBuffer, MESSAGE_SIZE, &priroty);
        friendPid = messageBuffer.ID;
        friendQueue = mq_open(messageBuffer.text, O_WRONLY);
        printf("%s%d\n", "you are connected to a friend of ID: ", messageBuffer.queueInfo);
    }
    signalFixing = true;
}

void handleStop(int sig){
    setMessage(myID, myQueue, NULL);
    mq_send(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, STOP);
    if(friendPid!=-1 && friendQueue != -1){
        kill(friendPid, SIGTSTP);
        mq_close(friendQueue);
    }
    exit(0);
}

void handleDisconnection(){
    if(friendPid != -1 && friendQueue != -1){
        kill(friendPid, SIGTSTP);
        mq_close(friendQueue);
        friendPid = -1;
        friendQueue = -1;
    }
    setMessage(myID, myQueue, NULL);
    mq_send(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, DISCONNECT);
}

void handleConnection(){
    int wanted;
    scanf("%d", &wanted);
    setMessage(myID, wanted, NULL);
    mq_send(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, CONNECT);
}

void handleReadingMailBox(){
    struct mq_attr newAttr;
    struct mq_attr oldAttr;

    mq_getattr(myQueue, &oldAttr);

    newAttr.mq_curmsgs = oldAttr.mq_curmsgs;
    newAttr.mq_flags = O_NONBLOCK;
    newAttr.mq_maxmsg = oldAttr.mq_maxmsg;
    newAttr.mq_msgsize = oldAttr.mq_msgsize;

    mq_setattr(myQueue, &newAttr, NULL);

    int succes = mq_receive(myQueue,(char*) &messageBuffer, MESSAGE_SIZE, &priroty);

    mq_setattr(myQueue, &oldAttr, NULL);

    if(succes != -1 ){
        printf("%s\n", messageBuffer.text);
    }
    else{
        printf("%s\n", "no messages");
    }
}

void handleListing(){
    setMessage(myID, myQueue, NULL);
    mq_send(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, LIST);
}

void handleTextingFriend(){
    if(friendPid == -1 || friendQueue == -1){
        printf("%s\n", "no friends available");
    }
    else{
        scanf("%s", messageBuffer.text);
        mq_send(friendQueue, (char*) &messageBuffer, MESSAGE_SIZE, TEXT_FRIEND);
    }
}

void getID(){
    printf("%d\n", myID);
}

void initializeClient(){
    signal(SIGINT, handleStop);
    atexit(deleteQueue);

    struct sigaction action;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    action.sa_handler = handleDisconnectionSignal;
    sigaction(SIGTSTP, &action, NULL);

    action.sa_handler = handleConnectionSignal;
    sigaction(SIGUSR1, &action, NULL);

    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_msgsize = MESSAGE_SIZE;
    attr.mq_maxmsg = MAX_MESSAGE_NUM;

    serverQueue = mq_open(SERVER_PATH, O_WRONLY);
    myQueue = mq_open(myQueuePath, O_RDONLY|O_CREAT, FLAG, &attr);
    myPid = getpid();

    setMessage(myPid, myQueue, myQueuePath);
    mq_send(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, INIT);
    mq_receive(myQueue, (char*) &messageBuffer, MESSAGE_SIZE, &priroty);

    if(messageBuffer.ID == -1){
        printf("%s\n", "No place available");
        exit(0);
    }

    myID = messageBuffer.ID;
}

void handleRequest(){
    signalFixing = false;
    int request;
    scanf("%d", &request);
    if(signalFixing){
        return;
    }
    if(request == LIST){
        handleListing();
        return;
    }
    if(request == STOP){
        handleStop(SIGINT);
        return;
    }
    if(request == CONNECT){
        handleConnection();
        return;
    }
    if(request == DISCONNECT){
        handleDisconnection();
        return;
    }
    if(request == READ_MAILBOX){
        handleReadingMailBox();
        return;
    }
    if(request == TEXT_FRIEND){
        handleTextingFriend();
        return;
    }
    if(request == GET_ID){
        getID();
        return;
    }
}

int main(int argc, char* argv[]){
    myQueuePath = argv[1];

    initializeClient();

    while(TRUE){
        handleRequest();
    }
}