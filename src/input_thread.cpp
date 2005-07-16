#include "input_thread.hpp"
#include"globals.hpp"
#include"screen.hpp"
#include"postsetdetailscreen.hpp"
#include<stdio.h>
#include<ncurses.h>


InputThread::InputThread(NetThread *thread) 
{
    net_thread = thread;
}
    
InputThread::~InputThread() 
{
}

void InputThread::Execute(void)
{
    Screen *current_screen = new PostSetDetailScreen();

    while(1){
//        usleep(10);
        int key = getch();
        if(key != ERR){
            current_screen->handle_input(key);
        }
        erase();
        current_screen->render();
        refresh();
    }
}

