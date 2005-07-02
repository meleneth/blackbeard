#include<signal.h>
#include<ncurses.h>
#include<pthread.h>

#include<algorithm>

#include"tcpconnection.hpp"
#include"net_thread.hpp"
#include"decoder_thread.hpp"
#include"input_thread.hpp"
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


int main(int argc, char *argv[])
{
    do_init();

    string input("");

    NetThread *net_thread = new NetThread(config);
    net_thread->Start();

    DecoderThread *decoder_thread = new DecoderThread();
    decoder_thread->Start();

    InputThread *input_thread = new InputThread(net_thread);
    input_thread->Start();

    pthread_join(net_thread->ThreadId, NULL);
    pthread_join(decoder_thread->ThreadId, NULL);
    pthread_join(input_thread->ThreadId, NULL);
    
    // will never get here, too many infinite loops in the threads
    
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
