#include "newsgroup.hpp"
#include "news_constants.hpp"
#include "console.hpp"
#include "config.hpp"
#include "strutil.hpp"
#include "post_set_splitter_dynamicmatch.hpp"
#include "messageheader.hpp"
#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <sstream>


using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;

// Public data members go here.
NewsGroup::NewsGroup(string group_name) // Constructor
{
    name = group_name;
    is_subscribed = 0;
    console->log("Creation of object for " + group_name);
    first_article_number = 0;
    first_article_number--;
    last_article_number = 0;

    splitter = new PostSetSplitterDynamicMatch(this);
}

    
NewsGroup::~NewsGroup() // Destructor
{
    delete splitter;
    Uint32 max_no = postsets.size();
    for(Uint32 i=0; i<max_no; ++i){
        delete postsets[i];
    }
    postsets.clear();
}

void NewsGroup::header_scoop(string xover_line)
{
    vector<string> header_pieces;
    vector<string>::iterator i;

    Tokenize(xover_line, header_pieces, "\t");
    MessageHeader *info = new MessageHeader(this, atoi(header_pieces[0].c_str()), header_pieces[1], header_pieces[2]);
    splitter->process_header(info);
}

Uint32 NewsGroup::postset_index(PostSet *set)
{
    Uint32 max_no = postsets.size();
    for(Uint32 i=0; i<max_no; ++i) {
       if(set == postsets[i])
           return i;
    }
    return 0;
}

PostSet *NewsGroup::postset_for_subject(string subject)
{
    int max_length = postsets.size();
    for(int i = 0; i < max_length; ++i){
        if(0 == subject.compare((postsets[i])->subject)){
            return postsets[i];
        }
    }
    PostSet *new_post = new PostSet(subject);
    postsets.push_back(new_post);
    new_post->group = this;
    return new_post;
}

int NewsGroup::status_code()
{
	return return_code;
}

string NewsGroup::status_string()
{
    stringstream buf;

    buf << "(" << postsets.size() << ") " << name << "     " 
        << first_article_number << "-" << last_article_number
        << ", " << last_article_number - first_article_number << " possible"; 

    return buf.str();
}

void NewsGroup::load_from_file(string filename)
{
}

void load_groups_from(string filename)
{
    StringPattern pattern = StringPattern(3);
    pattern.add_breaker(0);
    pattern.add_breaker(" ");
    pattern.add_breaker(1);
    pattern.add_breaker(" ");
    pattern.add_breaker(2);
    
    char linebuffer[1024];
    ifstream in;

    in.open(filename.c_str(), ios::in);
    if(in.is_open()){
        in.getline(linebuffer, 1024);

        while(!in.eof()){
            if(strlen(linebuffer)){
                if(pattern.match(linebuffer)){
                    NewsGroup *group = group_for_name(pattern.results[2]);
                    group->first_article_number = pattern.get_piecen(0);
                    group->last_article_number = pattern.get_piecen(1);
                }else{
                    group_for_name(linebuffer);
                }
            }
            in.getline(linebuffer, 1024);
        }
        in.close();
    }
}

void NewsGroup::save_postsets(void)
{
    ofstream out;
    string filename = config->blackbeard_data_dir + "/postsets." + name;

    out.open(filename.c_str(), ios::out);

    if(out.is_open()){
        Uint32 max_no = postsets.size();
        for(Uint32 i=0; i<max_no; i++)
        {
            PostSet *set = postsets[i];
            out << set->min_msg_id()    << " " 
                << set->max_msg_id()    << " " 
                << set->max_num_files() << " "
                << set->subject << endl;
        }
        out.close();
    }
    save_postsets_to_db();
}

void NewsGroup::save_postsets_to_db(void)
{
    string filename = config->blackbeard_data_dir + "/" + name + ".db";
    int rc;
    sqlite3* db;

    rc = sqlite3_open(filename.c_str(), &db);
    setup_newsgroup_tables(db);
    Uint32 max_no = postsets.size();
    for(Uint32 i=0; i<max_no; i++)
    {
        postsets[i]->save_postsets(db);
    }
    sqlite3_close(db);
}

void NewsGroup::setup_newsgroup_tables(sqlite3 *db)
{
    vector<string> queries;
    queries.push_back("CREATE TABLE post_files (postfile_no INTEGER, postset_no INTEGER, name VARCHAR)");
    queries.push_back("CREATE TABLE post_sets (postset_no INTEGER, name VARCHAR)");
    queries.push_back("CREATE TABLE file_pieces (file_piece_no INTEGER, postfile_no INTEGER, status VARCHAR)");
    Uint32 max_no = queries.size();
    for(Uint32 i=0; i<max_no; ++i) {
        int rc = sqlite3_exec(db, queries[i].c_str(), NULL, NULL, NULL);
        if(rc){
            console->log("Death :/");
        }
    }
}

#define NG_PSL_BUFFER_SIZE 40000

void NewsGroup::load_postsets(void)
{
    ifstream in;
    char linebuffer[NG_PSL_BUFFER_SIZE];

    StringPattern *pattern = new StringPattern(4);
    pattern->add_breaker(0);
    pattern->add_breaker(" ");
    pattern->add_breaker(1);
    pattern->add_breaker(" ");
    pattern->add_breaker(2);
    pattern->add_breaker(" ");
    pattern->add_breaker(3);

    string filename = config->blackbeard_data_dir + "/postsets." + name;

    in.open(filename.c_str(), ios::in);

    if(in.is_open()){
        in.getline(linebuffer, NG_PSL_BUFFER_SIZE);
        while(!in.eof()){
            if(strlen(linebuffer)) {
                if(pattern->match(linebuffer)){
                    PostSet *set = postset_for_subject(pattern->get_piece(3));
                    set->_min_msg_id = pattern->get_piecen(0);
                    set->_max_msg_id = pattern->get_piecen(1);
                    set->_max_num_files = pattern->get_piecen(2);
                    set->has_msg_ids = 0;
                    set->group = this;
                }
            }
            in.getline(linebuffer, NG_PSL_BUFFER_SIZE);
        }
    }
}

void NewsGroup::expire_old_postsets(Uint32 low_msg_id)
{
    vector <PostSet *> still_around;
    Uint32 max_no = postsets.size();
    for(Uint32 i=0; i<max_no; i++){
        if(postsets[i]->min_msg_id() > low_msg_id){
            still_around.push_back(postsets[i]);
        }
    }
    postsets = still_around;
}

void save_subscribed_groups_to(string filename)
{
    ofstream out;

    out.open(filename.c_str(), ios::out);

    if(out.is_open()){
        Uint32 max_no = newsgroups.size();
        for(Uint32 i=0; i<max_no; i++)
        {
            NewsGroup *group = newsgroups[i];
            if(newsgroups[i]->is_subscribed){
                out << group->first_article_number << " " << group->last_article_number << " " << group->name << endl;
                newsgroups[i]->save_postsets();
            }
        }
    }
}

NewsGroup *group_for_name(string groupname)
{
    Uint32 max_no = newsgroups.size();

    for(Uint32 i=0; i<max_no; ++i){
        if(0 == groupname.compare(newsgroups[i]->name)){
            return newsgroups[i];
        }
    }

    NewsGroup *news = new NewsGroup(groupname);
    newsgroups.push_back(news);
    return news;
}

bool NewsGroup::compare(const NewsGroup *a, const NewsGroup *b)
 {
    return (a->name < b->name);
}

Uint32 NewsGroup::index()
{
    Uint32 max_no = newsgroups.size();
    for(Uint32 i=0; i<max_no; ++i){
        if(newsgroups[i] == this) {
            return i;
        }
    }
    exit(1);
}
