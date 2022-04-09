#include "value_sem.h"
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

struct ValueSem { 
    void* value; 
    sem_t* full;
    sem_t* empty;

};

ValueSem* Create(void) {
    ValueSem* var = malloc(sizeof(ValueSem)); 
    var->value = NULL; 
    var->full = malloc(sizeof(sem_t)); 
    var->empty = malloc(sizeof(sem_t));
    sem_init(var->full, 0, 1); 
    sem_init(var->empty, 0, 0); 
    return var;
}

void Destroy(ValueSem* var) {

    sem_destroy(var->full);
    sem_destroy(var->empty);
    free(var->full);
    free(var->empty);
    if(var->value) {
        free(var->value); 
    }
    free(var); // calosc
}


void Insert(ValueSem* var, void* value) {  
    sem_wait(var->full);                  
    var->value = value;
    sem_post(var->empty);
}

void* Export(ValueSem* var) { 
    sem_wait(var->empty); 
    void* v = var->value; 
    var->value = NULL; 
    sem_post(var->full);
    return v;  
}

