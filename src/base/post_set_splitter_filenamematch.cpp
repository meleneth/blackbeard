#include "post_set_splitter_filenamematch.hpp"
#include "console.hpp"
#include "post_file.hpp"
#include "netcentral.hpp"

#include <string>

using std::stringstream;

PostSetSplitterFilenameMatch::PostSetSplitterFilenameMatch(NewsGroup *group):PostSetSplitter(group)
{
    pattern = new StringPattern(3);
    pattern->add_breaker("\"");
    pattern->add_breaker("\"");
}

PostSetSplitterFilenameMatch::~PostSetSplitterFilenameMatch()
{
}

void PostSetSplitterFilenameMatch::process_base_par_header(string filename, MessageHeader *header)
{
    PSSFMPostFilesbyPoster *poster = get_poster(header->posted_by);

    list<PostSet *>::iterator p;
    for(p = poster->postsets.begin(); p!= poster->postsets.end(); ++p) {
        if((*p)->main_par){
            if(0 == (*p)->main_par->filename.compare(filename)){
                (*p)->main_par->saw_message_id(header->article_no, header->msg_id, header->num_bytes);
                (*p)->main_par->update_status_from_pieces();
                netcentral->add_job(new PSSFMParJob((*p)->main_par, this));
                delete header;
                return;
            }
        }
    }

    PostSet *set = group->postset_for_subject(header->subject);
    poster->postsets.push_back(set);
    set->poster = header->posted_by;
    PostFile *file = set->file(filename);
    set->main_par = file;

    file->saw_message_id(header->article_no, header->msg_id, header->num_bytes);

    poster->find_stray_slice_files(set);

    netcentral->add_job(new PSSFMParJob(file, this));
    delete header;

    return;
}

void PostSetSplitterFilenameMatch::process_par_header(string filename, MessageHeader *header)
{
}

void PostSetSplitterFilenameMatch::process_header(MessageHeader *header)
{
    if(pattern->match(header->subject)) {
        string filename = pattern->results[1];
        // when we get a new base par2 file, fetch it.  The job we make will call us when it is finished downloading.

        if(is_par(filename)) {
            if(is_base_par(filename)) {
                return process_base_par_header(filename, header);
            }

        }
        PSSFMPostFilesbyPoster *poster = get_poster(header->posted_by);

        PostFile *file = poster->get_postfile(filename);
        file->saw_message_id(header->article_no, header->msg_id, header->num_bytes);
        file->update_status_from_pieces();

    }
    delete header;
}

void PostSetSplitterFilenameMatch::log_info(void)
{
    stringstream buf;
    buf << "Filename PostSetSplitter has " << posters.size() << " posters.  Poster info follows.";
    console->log(buf.str());
    buf.str("");

    console->log(" *** start Poster info");
    vector<PSSFMPostFilesbyPoster *>::iterator i;
    for(i = posters.begin(); i != posters.end(); ++i) {
        (*i)->log_info();
    }
    console->log(" *** end Poster info");

}

void PostSetSplitterFilenameMatch::process_par2(PostFile *parfile)
{
 // Parse par2 file.
 // Go through the list of filenames and suck those up from the list of files from this poster.
 // Create a postset and link the parfile to it so we can add any further pieces that come in
 // Files don't get removed from the postfiles list, they just get added to a postset when the par2
 // is parsed.

 // They totally need to get removed tho, and I need to search postsets.  Prolly something ninja like search back to front

 // ok one step at a time

    ParArchive *pfile = new ParArchive(parfile->par_mangled_filename());
    if(pfile->is_corrupt) {
        delete pfile;
        return;
    }

    PSSFMPostFilesbyPoster *poster = get_poster(parfile->post_set->poster);
    vector<string>::iterator i;
    for(i = pfile->par_filenames.begin(); i != pfile->par_filenames.end(); ++i) {
        PostFile *file = poster->get_postfile(*i);
        if(!file->post_set)
            parfile->post_set->add_file(file);
    }
    delete pfile;

    poster->find_stray_slice_files(parfile->post_set);
}

//----------------------------------------------------------------------

PSSFMPostFilesbyPoster::PSSFMPostFilesbyPoster(string poster)
{
    this->poster = poster;
    _last_postfile = NULL;
    _last_postset = NULL;
}

void PSSFMPostFilesbyPoster::add_group(NewsGroup *group)
{
    int max_length = group->postsets.size();
    vector<PostFile *>::iterator v;
    for(int i = 0; i < max_length; ++i){
        PostSet *set = group->postsets[i];
        if(0 == poster.compare(set->poster)){
            set->needs_full_info();
            for (v = set->files.begin(); v != set->files.end(); ++v){
                if(is_base_par((*v)->filename)){
                    set->main_par = *v;
                }
            }
            postsets.push_back(set);
        }
    }

}

void PSSFMPostFilesbyPoster::find_stray_slice_files(PostSet *set)
{
    list<PostFile *>::iterator v;
    string base_par = base_par_filename(set->main_par->filename);
    for(v=postfiles.begin(); v!=postfiles.end(); ++v){
        if(is_par((*v)->filename)){
            if(0 == base_par.compare(base_par_filename((*v)->filename))){
                set->add_file(*v);
                list<PostFile *>::iterator d = v;
                --v;
                postfiles.erase(d);
            }
        }
    }
}

void PSSFMPostFilesbyPoster::log_info(void)
{
    stringstream buf;

    console->log(" Info for poster: " + poster);
    buf << " I have " << postfiles.size() << " postfiles and " << postsets.size() << " postsets";
    console->log(buf.str()); buf.str("");
    
    list<PostSet *>::iterator i;
    vector<PostFile *>::iterator j;

    for(i = postsets.begin(); i!=postsets.end(); ++i){
        console->log("  PostSet: " + (*i)->subject);
        if((*i)->main_par)
            console->log("  MainPar: " + (*i)->main_par->filename);
        for(j = (*i)->files.begin(); j!=(*i)->files.end(); ++j){
            console->log("   PostFile:" + (*j)->filename);
            console->log((*j)->status_string());
        }
    }

    list<PostFile *>::iterator k;
    for(k = postfiles.begin(); k!=postfiles.end(); ++k){
            console->log("  PostFile:" + (*k)->filename);
            console->log((*k)->status_string());
    }


}

//---------------------------------------------------------------------------------

PSSFMPostFilesbyPoster *PostSetSplitterFilenameMatch::get_poster(string poster)
{
    vector<PSSFMPostFilesbyPoster *>::iterator i;
    for(i = posters.begin(); i != posters.end(); ++i) {
        if(poster == (*i)->poster) {
            return *i;
        }
    }
    PSSFMPostFilesbyPoster *p = new PSSFMPostFilesbyPoster(poster);
    p->add_group(group);
    posters.push_back(p);
    return p;
}

PostFile *PSSFMPostFilesbyPoster::get_postfile(string filename)
{
    if(_last_postfile) {
        if(0 == _last_postfile->filename.compare(filename)) {
            return _last_postfile;
        }
    }
    if(_last_postset) {
        vector<PostFile *>::iterator i;
        for(i = _last_postset->files.begin(); i!=_last_postset->files.end(); ++i){
            if((*i)->filename == filename) {
                _last_postfile = *i;
                return *i;
            }
        }
    }
    list<PostSet *>::iterator j;
    if(is_par(filename)) {
        string name = base_par_filename(filename);
        for(j=postsets.begin(); j!=postsets.end(); ++j){
            if((*j)->main_par){
                string short_comp = base_par_filename((*j)->main_par->filename);
                if(0 == name.compare(short_comp)){
                    PostFile *file = (*j)->file(filename);
                    _last_postfile = file;
                    _last_postset  = *j;
                    return file;
                }
             }
        }
    }

    list<PostFile *>::iterator i;
    for(i = postfiles.begin(); i!=postfiles.end(); ++i){
        if((*i)->filename == filename) {
            _last_postfile = *i;
            return *i;
        }
    }
    vector<PostFile *>::iterator k;
    for(j=postsets.begin(); j!=postsets.end(); ++j){
        PostSet *set = *j;
        for(k = set->files.begin(); k!=set->files.end(); ++k){
            if(0 == (*k)->filename.compare(filename)) {
                _last_postfile = *k;
                _last_postset = set;
                return *k;
            }
        }
    }


    PostFile *file = new PostFile(NULL);
    file->filename = filename;
    postfiles.push_back(file);
    _last_postfile = file;
    return file;
}

PostSet *PSSFMPostFilesbyPoster::get_postset(string subject)
{
    list<PostSet *>::iterator i;
    for(i = postsets.begin(); i!=postsets.end(); ++i){
        if((*i)->subject == subject) {
            return *i;
        }
    }
    PostSet *set = new PostSet(subject);
    set->poster = poster;
    postsets.push_back(set);
    return set;
}

PSSFMParJob::PSSFMParJob(PostFile *post_file, PostSetSplitterFilenameMatch *splitter) : PostfileJob(post_file)
{
    this->splitter = splitter;
}

void PSSFMParJob::finish(void)
{
    Job::finish();
    splitter->process_par2(postfile);
}

