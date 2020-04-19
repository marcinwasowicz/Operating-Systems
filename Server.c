#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<signal.h>
#include "Messenger.h"

int serverQueue = -1;

message messageBuffer;
client clients[MAX_CLIENTS];

void setMessage(long type, int info, int ID){
    messageBuffer.type = type;
    messageBuffer.info = info;
    messageBuffer.ID = ID;
}

void deleteQueue(){
    msgctl(serverQueue, IPC_RMID, NULL);
}

void handleStop(int sig){
    int clientCount = 0;
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i].clientID!=-1){
            kill(clients[i].pid, SIGINT);
            clientCount++;
        }
    }
    for(int i = 0; i<clientCount; i++){
        msgrcv(serverQueue, &messageBuffer, MESSAGE_SIZE, STOP, FLAG);
    }
    exit(0);
}

void handleInit(){
    int clientQueue = messageBuffer.info;
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i].clientID == -1){
            clients[i].clientID = 1;
            clients[i].cliendtQueueID = messageBuffer.info;
            clients[i].connected = 0;
            clients[i].pid = messageBuffer.ID;
            setMessage(INIT, i, 1);
            msgsnd(clientQueue, &messageBuffer, MESSAGE_SIZE, FLAG | IPC_NOWAIT);
            return;
        }
    }
    setMessage(INIT, -1, 1);
    msgsnd(clientQueue, &messageBuffer, MESSAGE_SIZE, FLAG);
}

void handleStopClient(){
    int clientID = messageBuffer.info;
    clients[clientID].clientID = -1;
}

void handleListing(){
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i].clientID!=-1 && clients[i].connected == 0){
            printf("%d\n",i);
        }
    }
}

void handleDissconection(){
    int clientID = messageBuffer.info;
    clients[clientID].connected = 0;
}

void handleConnection(){
    int clientID = messageBuffer.ID;
    int wantedID = messageBuffer.info;
    if(clients[wantedID].clientID == -1 || clients[wantedID].connected == 1){
        setMessage(CONNECT, -1, -1);
        msgsnd(clients[clientID].cliendtQueueID, &messageBuffer, MESSAGE_SIZE, FLAG);
    }
    else{
        setMessage(CONNECT, clients[wantedID].cliendtQueueID, 1);
        msgsnd(clients[clientID].cliendtQueueID, &messageBuffer, MESSAGE_SIZE, FLAG);
        setMessage(CONNECT, clients[clientID].cliendtQueueID, 1);
        msgsnd(clients[wantedID].cliendtQueueID, &messageBuffer, MESSAGE_SIZE, FLAG);
        clients[clientID].connected = 1;
        clients[wantedID].connected = 1;
    }
}

void initializeServer(){
    atexit(deleteQueue);
    signal(SIGINT, handleStop);
    serverQueue = msgget(ftok(PATH, SERVER), IPC_CREAT | 0666);
    
    for(int i = 0; i<MAX_CLIENTS; i++){
        clients[i].clientID = -1;
    }
}

void handleRequest(){
    msgrcv(serverQueue, &messageBuffer, MESSAGE_SIZE,ANY_TYPE, FLAG);
    if(messageBuffer.type ==INIT){
        handleInit();
        return;
    }
    if(messageBuffer.type == STOP){
        handleStopClient();
        return;
    }
    if(messageBuffer.type == LIST){
        handleListing();
        return;
    }
    if(messageBuffer.type == CONNECT){
        handleConnection();
        return;
    }
    if(messageBuffer.type == DISCONNECT){
        handleDissconection();
        return;
    }
}

int main(){
    initializeServer();
    while(TRUE){
        handleRequest();
    }
}