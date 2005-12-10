#include "webrequest.hpp"
#include "console.hpp"
#include "stringpattern.hpp"
#include "strutil.hpp"

WebRequest::WebRequest(TCPConnection *tcp)
{
    client = tcp;
    request_string = tcp->get_line();
    defaults();
    parse_uri(request_string);
}

WebRequest::WebRequest(string uri)
{
    request_string = uri;
    client = NULL;
    defaults();
    parse_uri(uri);
}

WebRequest::~WebRequest()
{
    delete client;
}

void WebRequest::defaults()
{
    path = "/";
    filename = "index.html";
    http_minor_version = 0;
    has_cgi_params = 0;
}


void WebRequest::parse_uri(string uri)
{
    console->log("URI: " + uri);
    StringPattern get_with_version = StringPattern(3);
    get_with_version.add_breaker("GET ");
    get_with_version.add_breaker(" HTTP/");

    StringPattern raw_get = StringPattern(2);
    raw_get.add_breaker("get ");

    if(get_with_version.match(uri)){
        console->log("Examine");
        console->log(get_with_version.results[2]);
        split_request_uri(get_with_version.results[1]);
        http_minor_version = 3;
        StringPattern split = StringPattern(2);
        split.add_breaker(0);
        split.add_breaker(".");
        split.add_breaker(1);
        if(split.match(get_with_version.results[2])){
            http_major_version = split.get_piecen(0);
            http_minor_version = split.get_piecen(1);
            if((http_minor_version == 1) && (http_minor_version == 1)){
                if(client){
                    parse_headers();
                }
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

    // split CGI params off of the request
    string::size_type pos = filename.find_first_of("?");
    if(pos != string::npos){
        string params = filename.substr(pos + 1, filename.length());
        filename = filename.substr(0, pos);
        has_cgi_params = 1;

        vector<string> all_params;
        Tokenize(params, all_params, ";");

        Uint32 max_param_no = all_params.size();
        StringPattern splitter = StringPattern(2);
        splitter.add_breaker(0);
        splitter.add_breaker("=");
        splitter.add_breaker(1);

        for(Uint32 i=0; i<max_param_no; ++i){
            splitter.match(all_params[i]);
            param_names.push_back(splitter.get_piece(0));
            param_values.push_back(splitter.get_piece(1));
        }
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

string WebRequest::param(string name)
{
    Uint32 max_no = param_names.size();

    for(Uint32 i=0; i<max_no; ++i){
        if(0 == param_names[i].compare(name)){
            return param_values[i];
        }
    }

    return "";
}

Uint32 WebRequest::paramn(string name)
{
    return atoi(param(name).c_str());
}

