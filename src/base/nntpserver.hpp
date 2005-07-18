#ifndef ___nntpserver_inc
#define ___nntpserver_inc 1

#include"SDL_types.h"

#include "tcpconnection.hpp"
#include "newsgrouppost.hpp"
#include"newsgroup.hpp"
#include<string>
#include<list>

using std::string;

class NNTPServer : public TCPConnection {
    public:
        // Public data members go here.
        NNTPServer(std::string hostname, int port); // Constructor
        ~NNTPServer(); // Destructor

        virtual void send_command(string cmd);

        void read_multiline_response(void);
        void read_xover_response();
        NewsGroupPost *read_body_response(void);

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


	int server_status;
    NewsGroup *newsgroup;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
