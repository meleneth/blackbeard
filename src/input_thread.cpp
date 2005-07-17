#include "input_thread.hpp"
#include"globals.hpp"
#include"screen.hpp"
#include"postsetdetailscreen.hpp"
#include<stdio.h>
#include<ncurses.h>

#include<sstream>

using std::stringstream;

InputThread::InputThread(NetThread *thread) 
{
    net_thread = thread;
}
    
InputThread::~InputThread() 
{
}

void InputThread::Execute(void)
{
    while(1){
//        usleep(10);
        int key = getch();
        if(key != ERR){
            stringstream buf;
            buf << "Recieved key: " << (int) key;
            console->log(buf.str());
            session->handle_input(key);
        }
        erase();
        session->render();
        refresh();
    }
}

