#ifndef MON_H
#define MON_H
#include <semaphore.h>
#include <condition_variable>
#include <iostream>
#include <thread>
#include "data.h"
class MonitorData{
    private:
        std::mutex guard_mutex;
        std::condition_variable set_val, get_val;
    public:
        void set_data(Data* value, int idx);
        Data *get_data(int idx);
};
#endif