#include "webrequest.hpp"
#include "console.hpp"
#include "stringpattern.hpp"
#include "strutil.hpp"

#include <sstream>

using std::stringstream;

WebRequest::WebRequest(TCPConnection *tcp)
{
    client = tcp;
    request_string = tcp->get_line();
    defaults();
    parse_uri(request_string);
    content_type = "text/html";
}

WebRequest::WebRequest(string uri)
{
    request_string = uri;
    client = NULL;
    defaults();
    parse_uri(uri);
    content_type = "text/html";
}

WebRequest::~WebRequest()
{
    if(client){
        delete client;
    }
}

void WebRequest::defaults()
{
    path = "/";
    filename = "index.html";
    http_minor_version = 0;
    has_cgi_params = 0;
    param_names.push_back("");
    param_values.push_back("");
}


void WebRequest::parse_uri(string uri)
{
//    console->log("URI: " + uri);
    StringPattern get_with_version = StringPattern(3);
    get_with_version.add_breaker("GET ");
    get_with_version.add_breaker(" HTTP/");

    StringPattern raw_get = StringPattern(2);
    raw_get.add_breaker("get ");

    if(get_with_version.match(uri)){
        //console->log(get_with_version.results[2]);
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
    } else {
        // cough hax
        split_request_uri(uri);
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
        //console->log("Saw header: " + line);
        line = client->get_line();
    }
}

string WebRequest::param(string name)
{
    Uint32 index = param_index(name);
    if(index) {
        return param_values[index];
    }
    return "";
}

Uint32 WebRequest::paramn(string name)
{
    return atoi(param(name).c_str());
}

Uint32 WebRequest::param_index(string name)
{
    Uint32 max_no = param_names.size();

    for(Uint32 i=1; i<max_no; ++i){
        if(0 == param_names[i].compare(name)){
            return i;
        }
    }
    return 0;
}

void WebRequest::param(string name, string value)
{
    Uint32 index = param_index(name);
    has_cgi_params = 1;
    if(index) {
        param_values[index] = value;
    } else {
        param_names.push_back(name);
        param_values.push_back(value);
    }
}

void WebRequest::param(string name, Uint32 value)
{
    stringstream s;
    s << value;
    param(name, s.str());
}


string WebRequest::get_uri(void)
{
    stringstream s;
    string split_char = "?";

    s << path << filename;

    if (has_cgi_params) {
        Uint32 max_no = param_names.size();
        for(Uint32 i = 1; i<max_no; ++i){
            s << split_char;
            split_char = ";";

            s << param_names[i]
              << "="
              << param_values[i];
        }
    }
    return s.str();
}

void WebRequest::delete_param(string name)
{
    vector<string>::iterator n = param_names.begin();
    vector<string>::iterator v = param_values.begin();
    for(;n != param_names.end();){
        if(0 == (*n).compare(name)){
            vector<string>::iterator p = n;
            --n; param_names.erase(p);
            p = v;
            --v; param_values.erase(p);
        }
        ++n;
        ++v;
    }
}

PostSet *WebRequest::postset(void)
{
    PostSet *set = newsgroup()->postsets[paramn("psi")];
    set->needs_full_info();
    return set;
}

PostFile *WebRequest::postfile(void)
{
    return postset()->files[paramn("pfi")];
}

NewsGroup *WebRequest::newsgroup(void)
{
    NewsGroup *group = newsgroups[paramn("ngi")];
    group->needs_postsets();
    return group;

}



