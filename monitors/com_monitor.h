#ifndef MON_COM_H
#define MON_COM_H
#include <semaphore.h>
#include <condition_variable>
#include <iostream>
#include <thread>
#include "command.h"
class MonitorCommand{
    private:
        std::mutex guard_mutex;
        std::condition_variable set_com, get_com;
    public:
        void set_command(Command *c);
        Command *get_command();
};
#endif

