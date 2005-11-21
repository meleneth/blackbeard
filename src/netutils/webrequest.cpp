#include "webrequest.hpp"
#include "console.hpp"

WebRequest::WebRequest(TCPConnection *tcp)
{
    client = tcp;
    request_string = tcp->get_line();
    path = "/";
    filename = "index.html";

    console->log("Got request: " + request_string);
}

WebRequest::~WebRequest()
{
}

