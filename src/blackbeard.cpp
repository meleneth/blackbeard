#include <stdio.h>
#include <signal.h>
#include <ncurses.h>

#include"console.hpp"
#include"tcpconnection.hpp"
#include"netcentral.hpp"
#include"config.hpp"


Console *console;
Config *config;
void do_init(void);
static void finish(int sig);
void create_nntp_thread(void);

int main(int argc, char *argv[])
{
    do_init();

    NetCentral *netcentral = new NetCentral();
    NNTPServer *connection = new NNTPServer(config->news_server, 119);

    netcentral->add_connection(connection);
    console->log("Connected");
    console->log("Connecting to " + config->news_server + " to grab article list for group " + config->news_group);

    console->log("Logging into server");
    connection->login("arnuga", "leper56");

    console->log("Selecting group " + config->news_group);
    connection->group(config->news_group);
    connection->xover();
    connection->last();
    connection->help();
    connection->date();
    connection->next();
    connection->stat();

    long art_id = 1;
    connection->xover(1);
    connection->article(art_id);
    connection->head(art_id);
    connection->body(art_id);

    std::string input("");

    while(1){
        // Select loop here
        netcentral->tick();

        SDL_Delay(10);

        int key = getch();
        if(key != ERR){
            if(key == 13){
                std::string line;
                line = "> " + input;
                console->log(line);
                connection->send_command(input);
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
    config = new Config();
    config->read_config_file();
}

static void finish(int sig)
{
    delete console;
    endwin();
    printf("\n\n\nBomb out!\n\n\n");
    exit(0);
}
