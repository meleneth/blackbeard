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
    StringPattern get_with_version(3);
    get_with_version.add_breaker("GET ");
    get_with_version.add_breaker(" HTTP/");

    StringPattern raw_get(2);
    raw_get.add_breaker("get ");

    if(get_with_version.match(uri)){
        console->log("Examine");
        console->log(get_with_version.results[2]);
        split_request_uri(get_with_version.results[1]);
        http_minor_version = 3;
        StringPattern split(2);
        split.add_breaker(0);
        split.add_breaker(".");
        split.add_breaker(1);
        if(split.match(get_with_version.results[2])){
            http_major_version = split.get_piecen(0);
            http_minor_version = split.get_piecen(1);
            if((http_minor_version == 1) && (http_minor_version == 1)){
                parse_headers();
            }
        }
    } else if(raw_get.match(uri)){
        http_minor_version = 0;
        split_request_uri(raw_get.results[1]);
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
        path = uri.substr(0, last_slash + 1);
    }
    
    if(uri_length != (last_slash + 1)){
        filename = uri.substr(last_slash + 1, uri_length);
    } else { 
        filename = "index.html";
    }
}

void WebRequest::parse_headers()
{
    string line = client->get_line();
    while(line.compare("")){
        console->log("Saw header: " + line);
        line = client->get_line();
    }
}

