#include<signal.h>
#include<ncurses.h>
#include<pthread.h>

#include<algorithm>

#include"tcpconnection.hpp"
#include"post_set.hpp"
#include"config.hpp"
#include"console.hpp"
#include"jobqueue.hpp"
#include"session.hpp"
#include"subjectfileloaderjob.hpp"
#include"netcentral.hpp"


Console *console;
Config *config;
NewsGroup *newsgroup;
vector< NewsGroup * >newsgroups;
PostSet *current_postset;
PostFile *current_postfile;
JobQueue *jobqueue;
Session *session;
NetCentral *netcentral;

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
    netcentral = new NetCentral();

    do_init();

    string input("");
    if(0 != config->load_file.compare("")){
        jobqueue->active_jobs.push_back(new SubjectFileLoaderJob(config->load_file));
    }else if(0 != config->load_group.compare("")){
//        console->log("Loading groups from " + config->load_group);
//        load_groups_from(config->load_group);  Job-ify me FIXME
    }

    while(1){
        jobqueue->process_jobs();
        netcentral->tick();

        int key = getch();
        if(key != ERR){
          //  stringstream buf;
          //  buf << "Recieved key: " << (int) key;
          //  console->log(buf.str());
            session->handle_input(key);
        }

        erase();
        session->render();
        refresh();

    }
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
