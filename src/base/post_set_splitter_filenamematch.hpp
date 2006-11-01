#ifndef ___post_set_splitter_filenamematch_inc
#define ___post_set_splitter_filenamematch_inc 1

#include "SDL_types.h"
#include "post_set_splitter.hpp"
#include "stringpattern.hpp"
#include "postfilejob.hpp"

class PSSFMPostFilesbyPoster {
    public:
        PSSFMPostFilesbyPoster(string poster);
        ~PSSFMPostFilesbyPoster();

        PostFile *get_postfile_parfile(string filename);
        PostFile *get_postfile_last_postset(string filename);
        PostFile *get_postfile_postsets(string filename);
        PostFile *get_postfile(string filename);
        PostFile *get_postfile_postfiles(string filename);
        PostSet  *get_postset(string subject);
        void clear_old_info(Uint32 oldest_article_no);
        void add_group(NewsGroup *group);
        void find_stray_slice_files(PostSet *set);
        void log_info(void);

        string poster;
        PostFile *_last_postfile;
        PostSet *_last_postset;
        list<PostFile *> postfiles;
        list<PostSet *> postsets;
};

class PostSetSplitterFilenameMatch : public PostSetSplitter {
    public:
        PostSetSplitterFilenameMatch(NewsGroup *group);
        ~PostSetSplitterFilenameMatch();

        virtual void process_header(MessageHeader *header);
        virtual void log_info(void);

        void process_base_par_header(string filename, MessageHeader *header);
        void process_par_header(string filename, MessageHeader *header);
        void clear_old_info(Uint32 oldest_article_no);

        void process_par2(PostFile *parfile);
        PSSFMPostFilesbyPoster *get_poster(string poster);

        vector<PSSFMPostFilesbyPoster *> posters;
        StringPattern *pattern;
        Uint32 header_count;
        
    private:
    protected:
};

class PSSFMParJob : public PostfileJob {
    public: 
        PSSFMParJob(PostFile* post_file, PostSetSplitterFilenameMatch *splitter);
        virtual void finish(void);

        virtual void process();
        PostSetSplitterFilenameMatch *splitter;

};

#endif
