#include<signal.h>
#include<ncurses.h>
#include<pthread.h>

#include<algorithm>

#include"tcpconnection.hpp"
#include"net_thread.hpp"
#include"decoder_thread.hpp"
#include"input_thread.hpp"
#include"post_set.hpp"
#include"console.hpp"
#include"jobqueue.hpp"
#include"session.hpp"


Console *console;
Config *config;
NewsGroup *newsgroup;
vector< NewsGroup * >newsgroups;
PostSet *current_postset;
PostFile *current_postfile;
JobQueue *jobqueue;
Session *session;

void do_init(void);
static void finish(int sig);
void create_nntp_thread(void);
void shut_down(void);

using std::string;
using std::stringstream;
using std::list;


int main(int argc, char *argv[])
{
    console = new Console();
    config = new Config(argc, argv);
    config->read_config_file();
    InputThread *input_thread = NULL;

    do_init();

    string input("");

    NetThread *net_thread = new NetThread(config);
    net_thread->Start();

    DecoderThread *decoder_thread = new DecoderThread();
    decoder_thread->Start();

    if(0 == config->debug_mode){
        input_thread = new InputThread(net_thread);
        input_thread->Start();
    }

    if(0 != config->load_file.compare("")){
        console->log("Loading subjects from " + config->load_file);
        NewsGroup *newsgroup = group_for_name("alt.mama");
        newsgroups.push_back(newsgroup);
        newsgroup->load_from_file(config->load_file);
        console->log("All subjects loaded");
    }else if(0 != config->load_group.compare("")){
        console->log("Loading groups from " + config->load_group);
        load_groups_from(config->load_group);
    }

    if(input_thread){
        pthread_join(input_thread->ThreadId, NULL);
    } 

    pthread_join(net_thread->ThreadId, NULL);
    pthread_join(decoder_thread->ThreadId, NULL);
    
    // will never get here, too many infinite loops in the threads
    
    finish(0);

}

void do_init(void)
{
    if(!config->debug_mode){
        // ncurses
        signal(SIGINT, finish);
        initscr();
        start_color();
        init_pair(1,COLOR_WHITE,0);
        init_pair(2,COLOR_GREEN,0);
        keypad(stdscr, TRUE);
        nonl();
        cbreak();
        noecho();
        nodelay(stdscr, 1);
    }
    
    // global objects
    current_postset = NULL;
    current_postfile = NULL;
    jobqueue = new JobQueue();
    newsgroup = group_for_name(config->news_group);
    session = new Session();
    config->load_persistant_data();
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
