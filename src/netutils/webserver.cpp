#include "webserver.hpp"
#include "webrequest.hpp"
#include "console.hpp"
#include "file_handle.hpp"
#include "webfilefetcher.hpp"
#include "webjoblist.hpp"
#include "webnewsgroups.hpp"
#include "webpostsets.hpp"
#include "webpostfiles.hpp"
#include "netcentral.hpp"
#include "headersforgroupjob.hpp"
#include "groupupdater.hpp"
#include "postsetjob.hpp"
#include "postfilejob.hpp"
#include "webviewfile.hpp"
#include "websearch.hpp"

WebServer::WebServer(string web_root, int port_no)
{
    listener = new TCPListener(port_no);
    this->web_root = web_root;
}

WebServer::~WebServer()
{
    delete listener;
}

void WebServer::handle_request(WebRequest *request)
{
   if(0 == request->path.compare("/")) {
       if(0 == request->filename.compare("joblist")){
//           console->log("Handling joblist request:");
           handlers.push_back(new WebJobList(request));
           return;
       }
       if(0 == request->filename.compare("newsgroups")){
//           console->log("Handling newsgroup request:");
           handlers.push_back(new WebNewsGroups(request));
           return;
       }
       if(0 == request->filename.compare("postsets")) {
//           console->log("Handling postsets request:");
           handlers.push_back(new WebPostSets(request));
           return;
       }
       if(0 == request->filename.compare("postfiles")) {
//           console->log("Handling postfiles request:");
           handlers.push_back(new WebPostFiles(request));
           return;
       }
       if(0 == request->filename.compare("search")) {
//           console->log("Handling search request");
           handlers.push_back(new WebSearch(request));
           return;
       }
       if(0 == request->filename.compare("viewfile")) {
//           console->log("Handling viewfile request:");
           handlers.push_back(new WebViewFile(request));
           return;
       }
       if(0 == request->filename.compare("updatepostset")) {
//           console->log("Handling update postset request");
//           console->log("Making job for " + request->filename);
           PostSet *set = request->postset();
           Job *new_job = new HeadersForGroupJob(request->newsgroup(), set->_min_article_no, set->_max_article_no);
           high_priority_jobs->add_job(new_job);
           delete request;
           return;
       }
       if(0 == request->filename.compare("downloadpostset")) {
//           console->log("Handling download postset request");
           PostSet *set = request->postset();
           set->tick = config->tick;
           Job *new_job = new PostsetJob(set);
           metajobs->add_job(new_job);
           delete request;
           return;
       }
       if(0 == request->filename.compare("downloadpostfile")) {
//           console->log("Handling download postfile request");
           PostFile *file = request->postfile();
           file->tick = config->tick;
           Job *new_job = new PostfileJob(file);
           //high_priority_jobs->add_job(new_job);
           netcentral->add_job(new_job);
           delete request;
           return;
       }
       if(0 == request->filename.compare("change_file_status")){
//           console->log("Changing file status");
           PostFile*file = request->postfile();
           file->tick = config->tick;
           if(0 == file->status.compare("Finished")){
               file->switch_seen_statuses(SEEN);
               file->update_status_from_pieces();
           }
       }
       if(0 == request->filename.compare("update_newsgroup")) {
//           console->log("Handling update newsgroup request");
           Job *new_job = new GroupUpdater(request->newsgroup());
           //high_priority_jobs->add_job(new_job);
           netcentral->add_job(new_job);
           delete request;
           return;
       }

   }

   string filename = web_root + request->path + request->filename;
//   console->log("Sending file: " + filename);

   handlers.push_back(new WebFileFetcher(request, filename));
}

void WebServer::tick(void)
{
    list <TCPConnection *>::iterator i;

    for(i = connections.begin(); i != connections.end(); ++i) {
        if((*i)->has_data_waiting()){
            handle_request(new WebRequest(*i));
            list <TCPConnection *>::iterator p = i;
            --i;
            connections.erase(p);
        }
    }

    list<WebDataFetcher *>::iterator h;

    for(h = handlers.begin(); h != handlers.end() ; ++h) {
        if (!(*h)->tick()) {
            list<WebDataFetcher *>::iterator j = h;
            WebDataFetcher *r = (*h);
            --h;
            delete r;
            handlers.erase(j);
        }
    }
}

void WebServer::log_to_file(string filename)
{
}

void WebServer::handle_new_connection(void)
{
    connections.push_back(listener->get_waiting_connection());
}
