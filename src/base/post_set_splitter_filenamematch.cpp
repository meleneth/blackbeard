#include "post_set_splitter_filenamematch.hpp"
#include "console.hpp"
#include "post_file.hpp"
#include "netcentral.hpp"

PostSetSplitterFilenameMatch::PostSetSplitterFilenameMatch(NewsGroup *group):PostSetSplitter(group)
{
    pattern = new StringPattern(3);
    pattern->add_breaker("\"");
    pattern->add_breaker("\"");
}

PostSetSplitterFilenameMatch::~PostSetSplitterFilenameMatch()
{
}

void PostSetSplitterFilenameMatch::process_header(MessageHeader *header)
{
    if(pattern->match(header->subject)) {
        string filename = pattern->results[1];
        //        console->log("Filename: " + filename + " -*- " + header->subject);
        // when we get a new base par2 file, fetch it.  The job we make will call us when it is finished downloading.
        PSSFMPostFilesbyPoster *poster = get_poster(header->posted_by);
        PostFile *file = poster->get_postfile(filename);

        file->saw_message_id(header->article_no, header->msg_id, header->num_bytes);
        file->update_status_from_pieces();

        if(is_par(filename)) {
            //            console->log(" * file is PAR2 file");
            if(is_base_par(filename)) {
                if(!file->post_set){
                    PostSet *set = new PostSet(header->subject);
                    set->poster = header->posted_by;
                    set->group = group;
                    group->postsets.push_back(set);
                    set->add_file(file);
                    set->main_par = file;

                    string base_par = base_par_filename(filename);
                    vector<PostFile *>::iterator i;
                    for(i = poster->postfiles.begin(); i!=poster->postfiles.end();++i){
                        if(is_par((*i)->filename)){
                            if(0 == base_par.compare(base_par_filename((*i)->filename))){
                                PostFile *matching_file = *i;
                                vector<PostFile *>::iterator d;
                                d = i;
                                --i;
                                poster->postfiles.erase(d);
                                set->add_file(matching_file);
                            }
                        }
                    }

                }
                //                console->log(" * is base PAR2 file");
                //                console->log(" * REQUEST: " + file->filename);
                netcentral->add_job(new PSSFMParJob(file, this));
                delete header;


                return;
            }
            if(!file->post_set) {
                if(is_recovery_par(filename)) {
                    vector<PostSet *>::iterator i;
                    string base_par = base_par_filename(filename);
                    for(i=poster->postsets.begin(); i!=poster->postsets.end(); ++i){
                        if((*i)->main_par){
                            if(0 == base_par_filename((*i)->main_par->filename).compare(base_par)){
                                // remove this PostFile from postfiles, and add it to the postset.
                                vector<PostFile *>::iterator pf;
                                for(pf=poster->postfiles.begin(); pf!=poster->postfiles.end(); ++pf){
                                    if(*pf == file){
                                        vector<PostFile *>::iterator d = pf;
                                        --pf;
                                        poster->postfiles.erase(d);
                                    }
                                }
                                (*i)->add_file(file);
                            }
                        }
                    }
                }
            }
        }
    }
    delete header;
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

PSSFMPostFilesbyPoster::PSSFMPostFilesbyPoster(string poster)
{
    this->poster = poster;

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
    vector<PostFile *>::iterator v;
    string base_par = base_par_filename(set->main_par->filename);
    for(v=postfiles.begin(); v!=postfiles.end(); ++v){
        if(is_par((*v)->filename)){
            if(0 == base_par.compare(base_par_filename((*v)->filename))){
                set->add_file(*v);
                vector<PostFile *>::iterator d = v;
                --v;
                postfiles.erase(d);
            }
        }
    }

}

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
    vector<PostFile *>::iterator i;
    for(i = postfiles.begin(); i!=postfiles.end(); ++i){
        if((*i)->filename == filename) {
            return *i;
        }
    }
    vector<PostSet *>::iterator j;
    for(j=postsets.begin(); j!=postsets.end(); ++j){
        PostSet *set = *j;
        for(i = set->files.begin(); i!=set->files.end(); ++i){
            if((*i)->filename == filename) 
                return *i;
        }
    }
    PostFile *file = new PostFile(NULL);
    file->filename = filename;
    if(is_par(filename)) {
        string name = base_par_filename(filename);
        for(j=postsets.begin(); j!=postsets.end(); ++j){
            if((*j)->main_par){
                string short_comp = base_par_filename((*j)->main_par->filename);
                if(0 == name.compare(short_comp)){
                    (*j)->add_file(file);
                    return file;
                }
             }
        }
    }
    postfiles.push_back(file);
    return file;
}

PostSet *PSSFMPostFilesbyPoster::get_postset(string subject)
{
    vector<PostSet *>::iterator i;
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
