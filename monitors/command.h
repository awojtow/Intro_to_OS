#ifndef COM_H 
#define COM_H
struct Command { 
    int command_code; 
    int buff_index; 
    int optional_arg;
};
#endif