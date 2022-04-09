// #include <semaphore.h>
#include <condition_variable>
#include <iostream>
#include <thread>
#include "data_monitor.h"


Data *buffor[2] = {nullptr, nullptr};


void MonitorData::set_data(Data* value, int idx){
        std::unique_lock<std::mutex> lock(guard_mutex);
        get_val.wait(lock, [idx]{return buffor[idx] == nullptr;});
        buffor[idx] = value;
        set_val.notify_all();
    }

Data* MonitorData::get_data(int idx){
       std::unique_lock<std::mutex> lock(guard_mutex);
       set_val.wait(lock, [idx]{return buffor[idx]!= nullptr;});
       Data *d = buffor[idx];
       buffor[idx] = nullptr;
       get_val.notify_all();
       return d;

    }

