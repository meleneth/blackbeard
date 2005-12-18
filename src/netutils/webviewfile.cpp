#include "webviewfile.hpp"
#include "postfilejob.hpp"
#include "netcentral.hpp"
#include "console.hpp"

WebViewFile::WebViewFile(WebRequest *request) :WebDataFetcher(request)
{
    output_lines.clear();
    file = request->postfile();

    if(file->status.compare("Finished")){
        Job *job = new PostfileJob(file);
        add_child_job(job);
        high_priority_jobs->add_job(job);
    } else {
        read_file();
    }
}

WebViewFile::~WebViewFile()
{
}

int WebViewFile::tick(void)
{
    if(file->status.compare("Finished"))
        return 1;

    return WebDataFetcher::tick();
}

void WebViewFile::notify_child_finished(Job *job)
{
    if(0 == file->status.compare("Finished"))
      read_file();

    JobParent::notify_child_finished(job);
}

void WebViewFile::read_file()
{
    FileHandle *fh = file->open_file();
    if(fh->still_open) 
        console->log("Opened file");
    while(fh->still_open){
        output_lines.push_back(fh->get_line());
    }
    num_lines = output_lines.size();
    fh->close();
    close_finished_files();
}
