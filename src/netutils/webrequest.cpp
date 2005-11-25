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
    delete client;
}

void WebRequest::parse_uri(string uri)
{
    console->log("URI: " + uri);
    StringPattern *get_with_version = new StringPattern(3);
    get_with_version->add_breaker("GET ");
    get_with_version->add_breaker(" HTTP");

    StringPattern *raw_get = new StringPattern(2);
    raw_get->add_breaker("get ");

    if(get_with_version->match(uri)){
        split_request_uri(get_with_version->results[1]);
        http_minor_version = 3;
    } else if(raw_get->match(uri)){
        http_minor_version = 0;
        split_request_uri(raw_get->results[1]);
    }

    delete get_with_version;
    delete raw_get;

}

void WebRequest::split_request_uri(string uri)
{
    Uint32 uri_length = uri.length();
    Uint32 last_slash = uri_length;
    while(uri[last_slash] != '/' && last_slash > 0) {
        --last_slash;
    }

    if(uri[last_slash] == '/') {
        path = uri.substr(0, last_slash + 1);
    }
    
    if(uri_length != (last_slash + 1)){
        filename = uri.substr(last_slash + 1, uri_length);
    } else { 
        filename = "index.html";
    }
}

