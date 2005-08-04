#include "input_thread.hpp"
#include"screen.hpp"
#include"postsetdetailscreen.hpp"
#include"session.hpp"
#include"post_set.hpp"
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
          //  stringstream buf;
          //  buf << "Recieved key: " << (int) key;
          //  console->log(buf.str());
            session->handle_input(key);
        }
        erase();
        lock_jobs();
        lock_postsets();
        session->render();
        unlock_postsets();
        unlock_jobs();
        refresh();
    }
}

