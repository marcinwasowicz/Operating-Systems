#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include "Messenger.h"

int myQueue = -1;
int friendQueue = -1;
int serverQueue = -1;
int myID = -1;

message messageBuffer;
friendMessage chatBuffer;

void setMessage(long type, int info, int ID){
    messageBuffer.type = type;
    messageBuffer.info = info;
    messageBuffer.ID = ID;
}

void deleteQueue(){
    msgctl(myQueue, IPC_RMID, NULL);
}

void handleStop(int sig){
    setMessage(STOP, myID, myID);
    msgsnd(serverQueue, &messageBuffer, MESSAGE_SIZE, FLAG);
    exit(0);
}

void handleConnection(){
    int friendID;
    scanf("%d", &friendID);
    setMessage(CONNECT, friendID, myID);
    msgsnd(serverQueue, &messageBuffer, MESSAGE_SIZE, FLAG);
    msgrcv(myQueue, &messageBuffer, MESSAGE_SIZE, CONNECT, FLAG);
    if(messageBuffer.info == -1){
        printf("%s\n", "can not connect to user");
        return;
    }
    friendQueue = messageBuffer.info;
}

void handleDisconnection(){
    friendQueue = -1;
    setMessage(DISCONNECT, myID, myID);
    msgsnd(serverQueue, &messageBuffer,MESSAGE_SIZE, FLAG);
}

void handleListing(){
    setMessage(LIST, myID, myID);
    msgsnd(serverQueue, &messageBuffer, MESSAGE_SIZE, FLAG);
}

void handleReadingMailBox(){
    int serverMessage = msgrcv(myQueue, &messageBuffer, MESSAGE_SIZE, CONNECT, FLAG | IPC_NOWAIT);
    int friendMsg = msgrcv(myQueue, &chatBuffer, FRIEND_MESSAGE_SIZE, TEXT_FRIEND, FLAG | IPC_NOWAIT);
    if(serverMessage == -1){
        printf("%s\n", "no one was contacting you");
    }
    else{
        printf("%s\n", "a friend connected to you");
        friendQueue = messageBuffer.info;
    }
    if(friendMsg == -1){
        printf("%s\n", "no one was texting you");
    }
    else{
        printf("%s\n", chatBuffer.text);
    }
}

void handleTextingFriend(){
    if(friendQueue == -1){
        printf("%s\n", "no friends available");
    }
    else{
        scanf("%s", (&chatBuffer)->text);
        msgsnd(friendQueue, &chatBuffer, FRIEND_MESSAGE_SIZE, FLAG);
    }
}

void getID(){
    printf("%d\n", myID);
}

void initClient(){
    signal(SIGINT, handleStop);
    atexit(deleteQueue);
    serverQueue = msgget(ftok(PATH, SERVER),FLAG);
    myQueue = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    setMessage(INIT, myQueue, getpid());

    msgsnd(serverQueue, &messageBuffer, MESSAGE_SIZE, FLAG);
    msgrcv(myQueue, &messageBuffer, MESSAGE_SIZE, INIT, FLAG);

    if(messageBuffer.info == -1){
        printf("%s\n", "No place available");
        exit(0);
    }

    myID = messageBuffer.info;
    chatBuffer.type = TEXT_FRIEND;
}

void handleRequest(){
    int request;
    scanf("%d", &request);

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

int main(){
    initClient();

    while(TRUE){
        handleRequest();
    }

}
