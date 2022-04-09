#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include<unistd.h>
#include <time.h>
#include "data_monitor.h"
#include "com_monitor.h"


MonitorData md;
MonitorCommand mc;
int running = true;
void producer_fun();
void fibonacci_fun();
void random_fun();
/* ----- Biniet ----- */
//do oobliczania wyrazu ciagu fibbonaciego
unsigned long biniet(unsigned long n) {

 return (int)((double)(1) / sqrt(5.0) * pow((1 + sqrt(5.0)) / 2, (double)(n)))
 ;
}

void producer_fun() {
    time_t start = time(NULL);
    time_t seconds = 6; 
    time_t stop = time(NULL);
    while (true){ 
        stop = time(NULL);
        if(stop - start > seconds) {
            running = 0;
            break;}
        Command* c = mc.get_command(); 
        Data *data = new Data; 
        int insert_idx;
        if(c->command_code == 0) { 
            data->i = biniet(c->optional_arg);
            printf("[PROD] prepared for FIBO\n");
        }
        else if(c->command_code == 1) { 
            data->d = (double)rand() / (double)RAND_MAX;
            printf("[PROD] prepared for RAND\n");
        }
        
        md.set_data(data, c->buff_index); 
        delete c;
    }
    printf("[PROD] Ended work\n");  
    return;   
}
/* ----- Fibonacci ----- */
void fibonacci_fun() {
    while(running) { 
        Command* c = new Command;
        c->command_code = 0;
        c->buff_index = 0; 
        c->optional_arg = rand() % 100; 
        mc.set_command(c); 
        printf("ASKING: [FIBO] asked for num: %i\n", c->optional_arg);
        Data* data = md.get_data(0); 
        printf("[FIBO] recieved: %lu\n", data->i);
        delete data; 
        sleep(1);
    }
    
    printf("[FIBO] Ended work\n"); 
    return ; 
}
/* ----- Random ----- */ //tutaj analogicznie do fibonacciego wszystko
void random_fun() {
    while(running) {
        Command* c = new Command;
        c->command_code = 1;
        c->buff_index = 1; 
        mc.set_command(c);
        printf("ASKING: [RAND] asked \n");
        Data* data = md.get_data(1);
        printf("[RAND] recieved: %f\n", data->d);
        delete data;
        sleep(2);
    }
    
    printf("[RAND] Ended work\n");
    return;
}


int main() {



    printf("Creating threads \n"); 
    std::thread producer(producer_fun);
    std::thread fibonacci(fibonacci_fun);
    std::thread randomt(random_fun);
    
    producer.join();
    printf("[PROD] Joined\n");
    fibonacci.join();
    printf("[FIBO] Joined\n");
    randomt.join();
    printf("[RAND] Joined\n");
    printf("Exiting... \n");
    return 0;
}
