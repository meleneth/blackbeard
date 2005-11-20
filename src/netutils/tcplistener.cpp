#include "tcplistener.hpp"

TCPListener::TCPListener(int port_num)
{
    port_no = port_num;
}

TCPListener::~TCPListener()
{
}

TCPConnection *get_waiting_connection()
{
    return NULL;
}

int has_connection_waiting()
{
    return 0;
}

