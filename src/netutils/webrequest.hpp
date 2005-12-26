#ifndef ___webrequest_inc
#define ___webrequest_inc 1

#include "SDL_types.h"
#include "tcpconnection.hpp"
#include <string>
#include <vector>

#include "post_set.hpp"
#include "post_file.hpp"
#include "newsgroup.hpp"

using std::string;
using std::vector;

class WebRequest {
    public:
        WebRequest(TCPConnection *client);
        WebRequest(string uri);
        ~WebRequest();

        void parse_uri(string uri);
        void parse_headers(void);
        void split_request_uri(string uri);
        void defaults(void);
        string get_uri(void);
        string param(string name);
        Uint32 paramn(string name);
        Uint32 param_index(string name);
        void delete_param(string name);
        void param(string name, string value);
        void param(string name, Uint32 value);

        // BLB specific methods
        PostSet   *postset();
        PostFile  *postfile();
        NewsGroup *newsgroup();
        // BLB specific methods END

        TCPConnection *client;
        string request_string;
        string path;
        string filename;
        string content_type;
        Uint32 http_minor_version;
        Uint32 http_major_version;
        Uint32 has_cgi_params;

        vector<string> param_names;
        vector<string> param_values;

    private:
    protected:
};

#endif
