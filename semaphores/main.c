#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "value_sem.h"

/* ----- Incjalizacja ----- */
typedef struct { // struktura command z zadania 
    int command_code; 
    int buff_index; 
    int optional_arg;
} Command;
//union na dane
typedef union {
    unsigned long int i;
    double d;
} Data;

//inicjacja uchwytow watkow i danych wspoldzielonych
int stop_running = 1; // 1 
static pthread_t producer;
static pthread_t fibonacci;
static pthread_t randomt;
static ValueSem* command;
static ValueSem* buffer[2];

/* ----- Biniet ----- */
//do oobliczania wyrazu ciagu fibbonaciego
static unsigned long biniet(unsigned long n) {

 return (int)((double)(1) / sqrt(5.0) * pow((1 + sqrt(5.0)) / 2, (double)(n)))
 ;
}

/* ----- Producer ----- */
static void* producer_fun(void* args) {
    time_t start = time(NULL);
    time_t seconds = 5; 
    time_t stop = time(NULL);
    while (true){ 
        stop = time(NULL);
        if(stop - start > seconds) break; 

        Command* c = Export(command); 
        Data* data = malloc(sizeof(Data)); 

        if(c->command_code == 0) {
            data->i = biniet(c->optional_arg);
            printf("[PROD] prepared for FIBO\n");
        }
        else if(c->command_code == 1) { 
            data->d = (double)rand() / (double)RAND_MAX;
            printf("[PROD] prepared for RAND\n");
        }

        Insert(buffer[c->buff_index], data); 
        free(c); 
    }

    stop_running = 0; 
    return NULL;   
}
/* ----- Fibonacci ----- */
static void* fibonacci_fun(void* args) {
    while(stop_running) { 
        Command* c = malloc(sizeof(Command)); 
        c->command_code = 0;
        c->buff_index = 0; 
        c->optional_arg = rand() % 100; 
        Insert(command, c); 
        printf("[FIBO] asked for num: %i\n", c->optional_arg);
        Data* data = Export(buffer[c->buff_index]); 
        printf("[FIBO] recieved: %lu\n", data->i);
        free(data);
        sleep(1);
    }
    
    printf("[FIBO] Ended work\n"); //po zakonczeniu petli 
    return NULL;
}
/* ----- Random ----- */ //tutaj analogicznie do fibonacciego wszystko
static void* random_fun(void* args) {
    while(stop_running) {
        Command* c = malloc(sizeof(Command));
        c->command_code = 1;
        c->buff_index = 1; 
        Insert(command, c);
        printf("[RAND] asked \n");
        Data* data = Export(buffer[c->buff_index]);
        printf("[RAND] recieved: %lu\n", data->i);
        free(data);
        sleep(2);
    }
    
    printf("[RAND] Ended work\n");
    return NULL;
}


int main(void) {


    srand(time(NULL)); // init randomowosci

    printf("Init semaphores\n"); //init semaforow 
    command = Create();
    for (int i = 0; i <2; i++){
        buffer[i] = Create();
    }

    printf("Creating threads \n"); //init watkow
    pthread_create(&producer, NULL, producer_fun, NULL); 
    pthread_create(&fibonacci, NULL, fibonacci_fun, NULL);
    pthread_create(&randomt, NULL, random_fun, NULL); 

    // printf("Waiting for threads...\n"); 
    pthread_join(producer, NULL);  
    pthread_join(fibonacci, NULL);
    pthread_join(randomt, NULL);

    printf("Destroying... \n");
    Destroy(command); 
    for(int i = 0; i < 2; i++) {
        Destroy(buffer[i]); 
    }

    printf("Exiting... \n");
    return 0;
}
