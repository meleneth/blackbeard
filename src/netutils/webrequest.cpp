#include "webrequest.hpp"

WebRequest::WebRequest(TCPConnection *tcp)
{
    client = tcp;
}

WebRequest::~WebRequest()
{
}

