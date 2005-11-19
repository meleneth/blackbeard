#include<signal.h>
#ifdef __WIN32__
#include<curses.h>
#else
#include<ncurses.h>
#endif

#include"tcpconnection.hpp"
#include"config.hpp"
#include"console.hpp"
#include"jobqueue.hpp"
#include"session.hpp"
#include"subjectfileloaderjob.hpp"
#include"netcentral.hpp"


Console *console;
Config *config;
vector< NewsGroup * >newsgroups;
JobQueue *jobqueue;
Session *session;
NetCentral *netcentral;

void do_init(void);
static void finish(int sig);
void shut_down(void);
string last_msg;

using std::string;
using std::stringstream;
using std::list;


int main(int argc, char *argv[])
{
    console = new Console();
    config = new Config(argc, argv);
    config->read_config_file();

    do_init();

    string input("");
    if(0 != config->load_file.compare("")){
        jobqueue->jobs.push_back(new SubjectFileLoaderJob(config->load_file));
    }else if(0 != config->load_group.compare("")){
//        console->log("Loading groups from " + config->load_group);
//        load_groups_from(config->load_group);  Job-ify me FIXME
    }

    while(1){
        for(Uint32 process = 0; process < 8; ++process){
            jobqueue->process_jobs();
            netcentral->process_jobs();
        }

        int key = getch();
        if(key != ERR){
          //  stringstream buf;
          //  buf << "Recieved key: " << (int) key;
          //  console->log(buf.str());
            session->handle_input(key);
        }

        if(!config->debug_mode){
            erase();
            session->render();
            refresh();
        }

    }
    finish(0);
}

void do_init(void)
{
    last_msg = "";
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
    jobqueue = new JobQueue();
    session = new Session();
    netcentral = new NetCentral();
    config->load_persistant_data();
    netcentral->restore_saved_jobs();
}

static void finish(int sig)
{
    endwin();
    delete netcentral;
    delete jobqueue;
    delete session;
    Uint32 max_no = newsgroups.size();
    for(Uint32 i=0; i<max_no; ++i){
        delete newsgroups[i];
    }
#ifndef __WIN32__
    newsgroups.clear();
#endif
    delete console;
    printf("Finish called\n");
    printf(last_msg.c_str());
    exit(0);
}

void shut_down(void)
{
    finish(0);
}
