#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_SECONDS 5

pthread_mutex_t waitingRoomMutex;
pthread_cond_t sleepingCond;
int numClients;
int numChairs;
int freeChairs;
int* chairs; 
bool isSleeping = false;

void* barber(void* args) {
    for(int i = 0; i<numClients; i++){
        for(int go = 1; go && (pthread_mutex_lock(&waitingRoomMutex) || 1); go = (pthread_mutex_unlock(&waitingRoomMutex) || 0)) {
            while (freeChairs == numChairs) {
            printf("%s\n", "Barber : no one is waiting, going to sleep");
            isSleeping = true;
            pthread_cond_wait(&sleepingCond, &waitingRoomMutex);
        }
        isSleeping = false;
        int clientIndex = -1;
        for(int j = 0; j<numClients; j++){
            if(chairs[j]!=0){
                clientIndex = j;
                break;
            }
        }
        freeChairs++;
        printf("Barber : Waiting %d clients, serving client %u\n", numChairs - freeChairs, chairs[clientIndex]);
        chairs[clientIndex] = 0;
        }
        sleep(rand()%MAX_SECONDS);
    }
    printf("Barber : job done\n");
    return NULL;
}

void* client(void* args) {
    int* id = (int*) args;
    bool done = false;
    while(true){
        for (int go = 1; go && (pthread_mutex_lock(&waitingRoomMutex) || 1); go = (pthread_mutex_unlock(&waitingRoomMutex) || 0)){
            if (freeChairs == 0) {
                printf("Client %u : all seats taken\n", *id);
                continue;
            }
            int freeChairIndex = -1;
            for(int j = 0; j<numChairs; j++){
                if(chairs[j] == 0){
                    freeChairIndex = j;
                    break;
                }
            }
            chairs[freeChairIndex] = *id;
            printf("Client %u : Waitingroom, free chairs %d\n", *id, freeChairs);
            if(freeChairs == numChairs && isSleeping) {
                printf("Client %u : waking barber up\n", *id);
                pthread_cond_broadcast(&sleepingCond);
            }
            freeChairs--;
            done = true;
        }
        if(done){
            free(id);
            return NULL;
        }
        sleep(rand() % MAX_SECONDS);
    }
}

void setUpGlobalVariables(char* argv[]){
    numClients = atoi(argv[1]);
    numChairs= atoi(argv[2]);
    freeChairs = numChairs;
    srand(time(NULL));
    chairs = calloc(numChairs, sizeof *chairs);
    isSleeping = false;
    pthread_mutex_init(&waitingRoomMutex, NULL);
    pthread_cond_init(&sleepingCond, NULL);
}

int main(int argc, char** argv) {
    setUpGlobalVariables(argv);
    pthread_t barber_thread, *client_thread = malloc(numClients * sizeof  *client_thread);
    pthread_create(&barber_thread, NULL, barber, NULL);
    for(int i = 0; i<numClients; i++){
        int* arg = (int*)malloc(sizeof(int));
        *arg = i+1;
        pthread_create(&client_thread[i], NULL, &client, arg);
    }
    for(int i = 0; i<numClients; i++){ 
        pthread_join(client_thread[i], NULL);
    }
    pthread_join(barber_thread, NULL);
    return 0;
}
