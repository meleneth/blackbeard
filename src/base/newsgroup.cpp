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
#include <sqlite3.h>


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
    db_index = 0;

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

void NewsGroup::expire_old_postsets(Uint32 low_msg_id)
{
    vector <PostSet *> still_around;
    Uint32 max_no = postsets.size();
    for(Uint32 i=0; i<max_no; i++){
        if(postsets[i]->min_msg_id() > low_msg_id){
            still_around.push_back(postsets[i]);
        } else {
            postsets[i]->expire();
        }
    }
    postsets = still_around;
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

void NewsGroup::subscribe(void)
{
    is_subscribed = 1;
    if(db_index)
        return;

    //save_newsgroup_to_db(this);
}

void NewsGroup::unsubscribe(void)
{
    is_subscribed = 0;
}
