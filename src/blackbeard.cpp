#include"console.hpp"
#include"tcpconnection.hpp"

Console *console;

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;

    TCPConnection *connection = new TCPConnection("localhost", 119);
    console->log("Connected");

}
