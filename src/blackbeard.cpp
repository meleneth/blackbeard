#include"console.hpp"
#include"tcpconnection.hpp"
#include"netcentral.hpp"
#include<stdio.h>
#include<signal.h>
#include<ncurses.h>

Console *console;
void do_init(void);
static void finish(int sig);

int main(int argc, char *argv[])
{
    std::string servername = "localhost";
    std::string groupname = "misc.test";

    do_init();
    console = new Console(COLS, LINES);

    NetCentral *netcentral = new NetCentral();

    TCPConnection *connection = new TCPConnection("localhost", 119);

    netcentral->add_connection(connection);

    

    console->log("Connected");
    console->log("Connecting to " + servername + " to grab article list for group " + groupname);

    connection->send_command("mode reader");
    console->log("Switched to reader mode.");

    console->log("Requesting article list for " + groupname);
    connection->send_command("group " + groupname);

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
    initscr();
    keypad(stdscr, TRUE);
    nonl();
    cbreak();
    noecho();
    nodelay(stdscr, 1);
}

static void finish(int sig)
{
    delete console;
    endwin();
    printf("\n\n\nBomb out!\n\n\n");
    exit(0);
}

