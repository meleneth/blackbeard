#include "post_set_splitter_filenamematch.hpp"
#include "console.hpp"

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
        if(is_par(filename)) {
            console->log(" * file is PAR2 file");
            if(is_base_par(filename)) {
                console->log(" * is base PAR2 file");
            }
            if(is_recovery_par(filename)) {
                console->log(" * is slice data");
            }
        }

        
    } else {
        console->log("Filename NOT FOUND - " + header->subject);
    }
}

void PostSetSplitterFilenameMatch::process_par2(PostFile *parfile)
{
 // Parse par2 file.
 // Go through the list of filenames and suck those up from the list of files from this poster.
 // Create a postset and link the parfile to it so we can add any further pieces that come in
}

