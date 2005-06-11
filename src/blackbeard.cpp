#include"console.hpp"
#include"tcpconnection.hpp"
#include"netcentral.hpp"

Console *console;

int main(int argc, char *argv[])
{
    std::string servername = "localhost";
    std::string groupname = "misc.test";

    console = new Console();
    console->print_logs = 1;

    NetCentral *netcentral = new NetCentral();

    netcentral->add_connection(new TCPConnection("localhost", 119));

    console->log("Connected");
    console->log("Connecting to " + servername + " to grab article list for group " + groupname);


}
