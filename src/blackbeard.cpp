#include"console.hpp"
#include"tcpconnection.hpp"

Console *console;

int main(int argc, char *argv[])
{
    std::string servername = "localhost";
    std::string groupname = "alt.binaries.moocows";

    console = new Console();
    console->print_logs = 1;

    TCPConnection *connection = new TCPConnection("localhost", 119);

    console->log("Connected");
    console->log("Connecting to " + servername + " to grab article list for group " + groupname);


}
