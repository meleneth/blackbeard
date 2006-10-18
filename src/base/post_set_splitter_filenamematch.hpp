#ifndef ___post_set_splitter_filenamematch_inc
#define ___post_set_splitter_filenamematch_inc 1

#include "SDL_types.h"
#include "post_set_splitter.hpp"
#include "stringpattern.hpp"
#include "postfilejob.hpp"

class PSSFMPostFilesbyPoster {
    public:
        PSSFMPostFilesbyPoster(string poster);

        PostFile *get_postfile(string filename);
        PostSet  *get_postset(string subject);
        void add_group(NewsGroup *group);
        void find_stray_slice_files(PostSet *set);

        string poster;
        vector<PostFile *> postfiles;
        vector<PostSet *> postsets;
};

class PostSetSplitterFilenameMatch : public PostSetSplitter {
    public:
        PostSetSplitterFilenameMatch(NewsGroup *group);
        ~PostSetSplitterFilenameMatch();

        virtual void process_header(MessageHeader *header);
        void process_par2(PostFile *parfile);
        PSSFMPostFilesbyPoster *get_poster(string poster);

        vector<PSSFMPostFilesbyPoster *> posters;
        StringPattern *pattern;
        
    private:
    protected:
};

class PSSFMParJob : public PostfileJob {
    public: 
        PSSFMParJob(PostFile* post_file, PostSetSplitterFilenameMatch *splitter);
        virtual void finish(void);

        PostSetSplitterFilenameMatch *splitter;

};

#endif
