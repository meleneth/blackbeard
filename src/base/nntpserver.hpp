#ifndef ___nntpserver_inc
#define ___nntpserver_inc 1

#include"SDL_types.h"

#include "tcpconnection.hpp"
#include "newsgrouppost.hpp"
#include"newsgroup.hpp"
#include<string>
#include<vector>
#include<list>

using std::string;
using std::vector;

typedef enum { NS_IDLE, NS_CONNECTED, NS_LOGIN, NS_PASSWORD, NS_COMMAND_RESPONSE } NNTPServer_Status;

class NewsGroup;
class NNTPServer : public TCPConnection {
    public:
        // Public data members go here.
        NNTPServer(std::string hostname, int port); // Constructor
        ~NNTPServer(); // Destructor

        virtual void send_command(string cmd);

        void read_multiline_response(void);
        void read_xover_response();
        NewsGroupPost *read_body_response(void);
        string get_next_multi_line(void);
    
        string _current_command;
       int status();
       void login(string username, string password);
       void quit();
       void group(string groupname);
       void list();
       void listgroup();
       void xover();
       void xover(long article_id);
       void xover_format();
       void article(long article_id);
       void head(long article_id);
       NewsGroupPost *body(string article_id);
       void last();
       void help();
       void date();
       void next();
       void post();
       void stat();
       void tick(void);


	    NNTPServer_Status _status;
        string _username;
        string _password;
        Uint32 is_connected;
        Uint32 is_multiline_reading;
        NewsGroup *newsgroup;
        Uint32 last_command_failed;
        vector<string> responses; 
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
