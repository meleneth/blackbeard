#include "newsgroup.hpp"
#include "news_constants.hpp"
#include"console.hpp"
#include"config.hpp"
#include "strutil.hpp"
#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include<sstream>

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
  
    StringPattern *pattern;
//1997-11-19 Atripolis_2097-MIRAGE "ATRIPOLI.R10" (4/4) yEnc
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(") yEnc");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Harold";

//001 - Initial D S4D1 (2-9) - yEnc "HD1_5.part001.rar" (01/79)
    pattern = new StringPattern(SP_LASTPART +4);
    pattern->add_breaker(" - ");
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" (");
    pattern->add_breaker("-");
    pattern->add_breaker(") - yEnc \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "zPimplez";

// Beekmans post "fUf.v4.pal.dvdr.part024.rar" [027/144] - yEnc (140/201)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" [");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Maude";


// another fine blb post - File 1 of 1: "blb" yEnc (1/4)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - File ");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker(" of ");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker(": \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Harriet";

//new and improved blackbeard arrr - "blb" yEnc (1/6)
//GuNdaM wiNG v. 10 [par2]  - "gundam_wing_v10.vol0986+115.par2" yEnc (099/120)
//
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker("- \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Matilda";

//GuNdaM wiNG v. 10 [063/108]  - "gundam_wing_v10.part061.rar" yEnc (004/115)
    pattern = new StringPattern(SP_LASTPART +2);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker("[");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("]");
    pattern->add_breaker("- \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "poostabber";


//(Gundam Seed V4+V6 Kimagure V2-V5) [293/556] -  yEnc "Kimagure_Vol3.part11.rar" (66/69)
    pattern = new StringPattern(SP_LASTPART + 2);
    pattern->add_breaker("(");
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(") [");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] -  yEnc \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "RzrBladeZombie";

//(Sailormoon Sailor Stars Disk 4 par to follow) [012/213] - "yEnc "ss4_scn.part011.rar" (077/209)
    pattern = new StringPattern(SP_LASTPART +2);
    pattern->add_breaker("(");
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(") [");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - \"yEnc \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "fishHookZombie";

//(OMNI) E's Otherwise vol.3 [107/114] - "jspec-es.otherwise.vol3.vol0189+131.PAR2" yEnc (054/137)
//(OMNI) Case.Closed.Case.04.vol.05.r1.dvdr-kif[30/97] - "case.closed.case.04.vol.05.r1.dvdr-kif.r27" yEnc (072/201)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker("[");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "switch";

//UUDecode patterns

//SDL for those in need - SDL-1.2.7.tar.gz (1/8)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "Stumpy";

//SDL for those in need - File 1 of 1: SDL-1.2.7.tar.gz (1/8)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - File ");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker(" of ");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker(": ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "Pally";

//Fooly Cooly Volume 1 ~ "FLCL 1.part18.rar" ~[21/90] - REQ Kaleido Star Volumes 1,2 and 3 (085/127)

    pattern = new StringPattern(SP_LASTPART +3);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" ~ \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" ~[");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - ");
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "Schnell";

//(goku  27  post urotsukidoji_2) File 162 of 163 - VIDEO_TS.vol306+23.PAR2 (096/120)
    pattern = new StringPattern(SP_LASTPART + 2);
    pattern->add_breaker("(");
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(") File ");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker(" of ");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker(" - ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "vonDerscmidt";
}

    
NewsGroup::~NewsGroup() // Destructor
{
}

void NewsGroup::header_scoop(string xover_line)
{
    vector<string> header_pieces;
    vector<string>::iterator i;

    Tokenize(xover_line, header_pieces, "\t");

    digest_subject_line(header_pieces[0], header_pieces[1]);

}

PostFile *NewsGroup::digest_subject_line(string message_id, string subject)
{
    list< StringPattern * >::iterator sp;
    if(config->debug_logging)
        console->log("Subject: " + subject);

    //"[DVD9]" evil delete it >.<
    size_t s = subject.find("[DVD9]");
    while(s < string::npos){
        subject.replace(s, 6, "_DVD9_");
        s=subject.find("[DVD9]");
    }

    for (sp = yenc_subject_patterns.begin(); sp != yenc_subject_patterns.end(); ++sp){
        if((*sp)->match(subject)){
            PostSet *postset = postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            postset->has_msg_ids = 1;

            PostFile *postfile = postset->file((*sp)->get_piecen(SP_FILENO), 
                                                     (*sp)->get_piecen(SP_MAXFILENO), 
                                                     (*sp)->get_piece(SP_FILENAME));
            if(!postfile){
                return NULL;
            }
            postfile->decoder_type = DT_YENC;
            postfile->part((*sp)->get_piecen(SP_PARTNO), 
                                   (*sp)->get_piecen(SP_MAXPARTNO), atoi(message_id.c_str()));
            return postfile;
        }
    }

    for (sp = uu_subject_patterns.begin(); sp != uu_subject_patterns.end(); ++sp){
        if((*sp)->match(subject)){
            PostSet *postset = postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            postset->has_msg_ids = 1;
            PostFile *postfile = postset->file((*sp)->get_piecen(SP_FILENO), 
                                             (*sp)->get_piecen(SP_MAXFILENO), 
                                             (*sp)->get_piece(SP_FILENAME));
            if(!postfile){
                return NULL;
            }

            postfile->decoder_type = DT_UUDECODE;
            postfile->part((*sp)->get_piecen(SP_PARTNO), 
                                   (*sp)->get_piecen(SP_MAXPARTNO), atoi(message_id.c_str()));
            return postfile;
        }
    }
    return NULL;
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

void NewsGroup::load_from_file(string filename)
{
}

void load_groups_from(string filename)
{
    StringPattern *pattern = new StringPattern(4);
    pattern->add_breaker(0);
    pattern->add_breaker(" ");
    pattern->add_breaker(1);
    pattern->add_breaker(" ");
    pattern->add_breaker(2);
    pattern->add_breaker(" ");
    pattern->add_breaker(3);
    
    char linebuffer[1024];
    ifstream in;

    in.open(filename.c_str(), ios::in);
    if(in.is_open()){
        in.getline(linebuffer, 1024);

        while(!in.eof()){
            if(strlen(linebuffer)){
                if(pattern->match(linebuffer)){
                    NewsGroup *group = group_for_name(pattern->results[0]);
                    group->last_article_number = pattern->get_piecen(1);
                    group->first_article_number = pattern->get_piecen(2);
                }else{
                    group_for_name(linebuffer);
                }
            }
            in.getline(linebuffer, 1024);
        }
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
            out << set->min_msg_id() << " " << set->max_msg_id() << " " << set->subject << endl;
        }
    }
}

void NewsGroup::load_postsets(void)
{
    ifstream in;
    char linebuffer[1024];

    StringPattern *pattern = new StringPattern(3);
    pattern->add_breaker(0);
    pattern->add_breaker(" ");
    pattern->add_breaker(1);
    pattern->add_breaker(" ");
    pattern->add_breaker(2);

    string filename = config->blackbeard_data_dir + "/postsets." + name;

    in.open(filename.c_str(), ios::in);

    if(in.is_open()){
        in.getline(linebuffer, 1024);
        while(!in.eof()){
            if(strlen(linebuffer)) {
                if(pattern->match(linebuffer)){
                    PostSet *set = postset_for_subject(pattern->get_piece(2));
                    set->_min_msg_id = pattern->get_piecen(0);
                    set->_max_msg_id = pattern->get_piecen(1);
                    set->has_msg_ids = 0;
                    set->group = this;
                }
            }
            in.getline(linebuffer, 1024);
        }
    }
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
                out << group->name << endl;
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

