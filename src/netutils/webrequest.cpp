#include "webrequest.hpp"
#include "console.hpp"
#include "stringpattern.hpp"

WebRequest::WebRequest(TCPConnection *tcp)
{
    client = tcp;
    request_string = tcp->get_line();
    path = "/";
    filename = "index.html";
    http_minor_version = 0;

    parse_uri(request_string);

    console->log("Got request: " + request_string);
}

WebRequest::WebRequest(string uri)
{
    request_string = uri;
    path = "/";
    filename = "index.html";
    http_minor_version = 0;
    parse_uri(uri);
}

WebRequest::~WebRequest()
{
}

void WebRequest::parse_uri(string uri)
{
    StringPattern get_with_version = StringPattern(3);
    get_with_version.add_breaker("GET ");
    get_with_version.add_breaker(" HTTP");

    StringPattern raw_get = StringPattern(2);
    raw_get.add_breaker("get ");

    if(get_with_version.match(uri)){
        split_request_uri(get_with_version.get_piece(1));
        http_minor_version = get_with_version.get_piecen(2);
    } else if(raw_get.match(uri)){
        http_minor_version = 0;
        split_request_uri(raw_get.get_piece(2));
    }

}

void WebRequest::split_request_uri(string uri)
{
    Uint32 uri_length = uri.length();
    Uint32 last_slash = uri_length;
    while(uri[last_slash] != '/' && last_slash > 0) {
        --last_slash;
    }

    if(uri[last_slash] == '/') {
        path = uri.substr(0, last_slash);
    }
    
    if(uri_length != last_slash){
        filename = uri.substr(last_slash + 1, uri_length);
    }
}

