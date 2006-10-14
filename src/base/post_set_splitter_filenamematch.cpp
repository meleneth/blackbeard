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
        console->log("Filename: " + filename + " -*- " + header->subject);
// when we get a new base par2 file, fetch it.  The job we make will call us when it is finished downloading.
        PSSFMPostFilesbyPoster *poster = get_poster(header->posted_by);
        PostFile *file = poster->get_postfile(filename);

        file->saw_message_id(header->article_no, header->msg_id, header->num_bytes);

        if(is_par(filename)) {
            console->log(" * file is PAR2 file");
            if(is_base_par(filename)) {
                console->log(" * is base PAR2 file");
                netcentral->add_job(new PSSFMParJob(file, this, header));
                return;
            }
            if(is_recovery_par(filename)) {
                console->log(" * is slice data");
            }
        }

        
    } else {
        console->log("Filename NOT FOUND - " + header->subject);
    }

    delete header;
}

void PostSetSplitterFilenameMatch::process_par2(PostFile *parfile, MessageHeader *header)
{
 // Parse par2 file.
 // Go through the list of filenames and suck those up from the list of files from this poster.
 // Create a postset and link the parfile to it so we can add any further pieces that come in
 // Files don't get removed from the postfiles list, they just get added to a postset when the par2
 // is parsed.

    ParArchive *pfile = new ParArchive(parfile->filename);
    if(pfile->is_corrupt) {
        delete pfile;
        return;
    }

    PSSFMPostFilesbyPoster *poster = get_poster(header->posted_by);
    PostSet *set = new PostSet(header->subject);
    vector<string>::iterator i;
    for(i = pfile->par_filenames.begin(); i != pfile->par_filenames.end(); ++i) {
        PostFile *file = poster->get_postfile(*i);
        set->add_file(file);
    }
    group->postsets.push_back(set);

    delete header;
}

PSSFMPostFilesbyPoster::PSSFMPostFilesbyPoster(string poster)
{
    this->poster = poster;
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
    PostFile *file = new PostFile(NULL);
    file->filename = filename;
    postfiles.push_back(file);
    return file;
}

PSSFMParJob::PSSFMParJob(PostFile *post_file, PostSetSplitterFilenameMatch *splitter, MessageHeader *header) : PostfileJob(post_file)
{
    this->splitter = splitter;
    this->header = header;
}

void PSSFMParJob::finish(void)
{
    splitter->process_par2(postfile, header);
}
