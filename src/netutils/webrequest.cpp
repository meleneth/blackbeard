#include "webrequest.hpp"
#include "console.hpp"

WebRequest::WebRequest(TCPConnection *tcp)
{
    client = tcp;
    console->log("Got request: " + tcp->get_line());
}

WebRequest::~WebRequest()
{
}

