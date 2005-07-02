#include<stdio.h>
#include<signal.h>
#include<ncurses.h>
#include<pthread.h>

#include<algorithm>

#include"tcpconnection.hpp"
#include"netthread.hpp"
#include"decoder_thread.hpp"
#include"post_set.hpp"
#include"globals.hpp"


Console *console;
Config *config;
NewsGroup *newsgroup;
PostSet *current_postset;
PostFile *current_postfile;
JobQueue *jobqueue;

void do_init(void);
static void finish(int sig);
void create_nntp_thread(void);
void shut_down(void);

using std::string;
using std::stringstream;
using std::list;

#define KEY_LEFTARROW 260
#define KEY_RIGHTARROW 261

int main(int argc, char *argv[])
{
    do_init();

    string input("");

    NetThread *net_thread = new NetThread(config);
    net_thread->Start();

    DecoderThread *decoder_thread = new DecoderThread();
    decoder_thread->Start();
    list<PostSet *>::iterator i;

    while(1){
//        usleep(10);
        int key = getch();
        if(key != ERR){
            switch(key){
                case 13:
                    //enter
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
            }
        }
        console->render();
        mvaddstr(LINES-1, 0, input.c_str());
        refresh();
    }
    finish(0);

}

void do_init(void)
{
    // ncurses
    signal(SIGINT, finish);
    initscr();
    keypad(stdscr, TRUE);
    nonl();
    cbreak();
    noecho();
    nodelay(stdscr, 1);
    
    // global objects
    current_postset = NULL;
    current_postfile = NULL;
    console = new Console(COLS, LINES);
    console->print_on_delete = 1;
    config = new Config();
    config->read_config_file();
    jobqueue = new JobQueue();

    newsgroup = new NewsGroup(config->news_group);
}

static void finish(int sig)
{
    endwin();
    delete console;
    exit(0);
}

void shut_down(void)
{
    finish(0);
}
