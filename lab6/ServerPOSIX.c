#define _POSIX_C_SOURCE 200809L
#include "MessengerPOSIX.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>
#include<string.h>
#include<mqueue.h>
#include<errno.h>

mqd_t serverQueue = -1;
message messageBuffer;
client clients[MAX_CLIENTS];
unsigned int priority;

void setMessage(int ID, int queue, char* text){
    messageBuffer.ID = ID;
    messageBuffer.queueInfo = queue;
    if(text != NULL){
        strcpy(messageBuffer.text, text);
    }
}

void deleteQueue(){
    mq_close(serverQueue);
    mq_unlink(SERVER_PATH);
}

void handleStop(){
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i].clientID != -1){
            mq_close(clients[i].cliendtQueueID);
            kill(clients[i].pid, SIGINT);
        }
    }
    exit(0);
}

void handleInit(){
    mqd_t queue = mq_open(messageBuffer.text, O_WRONLY);
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i].clientID == -1){
            clients[i].clientID = i;
            clients[i].cliendtQueueID = queue;
            clients[i].pid = messageBuffer.ID;
            clients[i].connected = 0;
            strcpy(clients[i].queuePath, messageBuffer.text);
            setMessage(i, serverQueue, NULL);
            mq_send(queue, (char*) &messageBuffer, MESSAGE_SIZE, INIT);
            return;
        } 
    }
    setMessage(-1, serverQueue, NULL);
    mq_send(queue, (char*) &messageBuffer, MESSAGE_SIZE, INIT);
    mq_close(queue);
}

void handleStopClient(){
    int clientID = messageBuffer.ID;
    mq_close(clients[clientID].cliendtQueueID);
    clients[clientID].clientID = -1;
}

void handleConnection(){
    int clientID = messageBuffer.ID;
    int wantedID = messageBuffer.queueInfo;

    if(clients[clientID].connected || clients[wantedID].clientID == -1 || clients[wantedID].connected){
        setMessage(1, -1, "can not connect or you are already conected\n");
        mq_send(clients[clientID].cliendtQueueID, (char*) &messageBuffer, MESSAGE_SIZE, CONNECT);
    }
    else{
        clients[clientID].connected = 1;
        clients[wantedID].connected = 1;

        setMessage(clients[wantedID].pid, wantedID, clients[wantedID].queuePath);
        mq_send(clients[clientID].cliendtQueueID, (char*) &messageBuffer, MESSAGE_SIZE, CONNECT);
        setMessage(clients[clientID].pid,clientID, clients[clientID].queuePath);
        mq_send(clients[wantedID].cliendtQueueID, (char*) &messageBuffer, MESSAGE_SIZE, CONNECT);
    }
}

void handleDisconnection(){
    clients[messageBuffer.ID].connected = 0;
}

void handleListing(){
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i].clientID!=-1 && clients[i].connected == 0){
            printf("%d\n",i);
        }
    }
}

void handleRequest(){
    int n = mq_receive(serverQueue, (char*) &messageBuffer, MESSAGE_SIZE, &priority);
    if(priority ==INIT){
        handleInit();
        return;
    }
    if(priority == STOP){
        handleStopClient();
        return;
    }
    if(priority == LIST){
        handleListing();
        return;
    }
    if(priority == CONNECT){
        handleConnection();
        return;
    }
    if(priority == DISCONNECT){
        handleDisconnection();
        return;
    }
}

void initializeServer(){
    signal(SIGINT, handleStop);
    atexit(deleteQueue);

    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGE_NUM;
    attr.mq_msgsize = MESSAGE_SIZE;

    serverQueue = mq_open(SERVER_PATH, O_CREAT|O_RDONLY,FLAG, &attr);
    for(int i = 0; i<MAX_CLIENTS; i++){
        clients[i].clientID = -1;
    }
}

int main(){
    initializeServer();
    while(TRUE){
        handleRequest();
    }
}



