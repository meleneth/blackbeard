#include<stdio.h>
#include<signal.h>
#include<ncurses.h>
#include<pthread.h>

#include"tcpconnection.hpp"
#include"netthread.hpp"
#include"decoder_thread.hpp"
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

int main(int argc, char *argv[])
{
    do_init();

    std::string input("");

    NetThread *net_thread = new NetThread(config);
    net_thread->Start();

    DecoderThread *decoder_thread = new DecoderThread();
    decoder_thread->Start();

    while(1){
//        usleep(10);
        int key = getch();
        if(key != ERR){
            if(key == 13){
                std::string line;
                line = "> " + input;
                console->log(line);
                net_thread->connection->TCPConnection::send_command(input);
                input = "";
            }else {
                input += (char)key;
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
