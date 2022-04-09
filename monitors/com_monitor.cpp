#include <condition_variable>
#include <iostream>
#include <thread>
#include "com_monitor.h"
Command *command = nullptr;
void MonitorCommand::set_command(Command *c){
        std::unique_lock<std::mutex> lock(guard_mutex); 
        get_com.wait(lock, []{return command == nullptr;}); 
        command = c;
        set_com.notify_all(); 
    }
Command* MonitorCommand::get_command(){
       std::unique_lock<std::mutex> lock(guard_mutex); 
       set_com.wait(lock, []{return command!= nullptr;});
       Command *c = command;
       command = nullptr;
       get_com.notify_all();
       return c;

    }
