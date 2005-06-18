#include<stdio.h>
#include<signal.h>
#include<ncurses.h>
#include<pthread.h>

#include"console.hpp"
#include"tcpconnection.hpp"
#include"netthread.hpp"
#include"config.hpp"


Console *console;
Config *config;
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

    while(1){
       // pth_nap(pth_time(0, 10));
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
    console = new Console(COLS, LINES);
    console->print_on_delete = 1;
    config = new Config();
    config->read_config_file();
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
