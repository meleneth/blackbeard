#ifndef ___webserver_inc
#define ___webserver_inc 1

#include"SDL_types.h"
#include<string>

using std::string;

class WebServer {
    public:
        WebServer(string document_root);
        ~WebServer();

        int Foo(void);

    private:
    protected:
};

#endif
