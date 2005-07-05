#include "input_thread.hpp"
#include"globals.hpp"
#include<stdio.h>
#include<ncurses.h>

#define KEY_ESC         27
#define KEY_Q           113
#define KEY_LEFTARROW   260
#define KEY_RIGHTARROW  261

InputThread::InputThread(NetThread *thread) 
{
    net_thread = thread;
}
    
InputThread::~InputThread() 
{
}

void InputThread::Execute(void)
{
    list<PostSet *>::iterator i;

    while(1){
//        usleep(10);
        int key = getch();
        if(key != ERR){
//        printf("%i\n", key);
            switch(key){
                case 13:
                    if(current_postset){
                        net_thread->set_retrieve();
                    }
                    break;
                case KEY_LEFTARROW:
                    i = find(newsgroup->postsets.begin(), newsgroup->postsets.end(), current_postset);
                    if(newsgroup->postsets.begin() != i){
                        i--;
                        current_postset = *i;
                    }
                    break;
                case KEY_RIGHTARROW:
                    i = find(newsgroup->postsets.begin(), newsgroup->postsets.end(), current_postset);
                    if(newsgroup->postsets.end() != i){
                        i++;
                        if(newsgroup->postsets.end() != i)
                            current_postset = *i;
                    }
                    break;
                case KEY_Q:
                case KEY_ESC:
                    endwin();
                    delete console;
                    exit(0);
                    break;
            }
        }
        console->render();
        refresh();
    }
}

