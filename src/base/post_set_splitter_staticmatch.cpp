#include "post_set_splitter_staticmatch.hpp"
#include "config.hpp"
#include "console.hpp"

PostSetSplitterStaticMatch::PostSetSplitterStaticMatch(NewsGroup *group) : PostSetSplitter(group)
{

    this->group = group;
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

//Noir vol 2 as reguested [003of102] - "VIDEO_TS.part01.rar" yEnc (001/118)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" [");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("of");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "damacy";

//Ghost in the Shell 2: Innocence DVD5 [Day 1 of 4] [02/26] - yEnc "gits2-dpimp.part002.rar" (62/79)
//
    pattern = new StringPattern(SP_LASTPART +3);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" [Day ");
    pattern->add_breaker(" of ");
    pattern->add_breaker("] [");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - yEnc \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "gitsy";

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

//(REQ: R.O.D. TV VOLS1,2,4,5,6 HERE IS VOL 3)[001/228] - ROD_TV_VOL3.par2 (1/1)
//
    pattern = new StringPattern(SP_LASTPART +2);
    pattern->add_breaker("(");
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(")[");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker("] - ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("(");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "lu-wow";
}

PostSetSplitterStaticMatch::~PostSetSplitterStaticMatch()
{
}

void PostSetSplitterStaticMatch::process_header(MessageHeader *header)
{
    list< StringPattern * >::iterator sp;
    if(config->debug_logging)
        console->log("Subject: " + header->subject);

    if(header->message_id < group->first_article_number)
        group->first_article_number = header->message_id;
    if(header->message_id > group->last_article_number)
        group->last_article_number = header->message_id;

    //"[DVD9]" evil delete it >.<
    size_t s = header->subject.find("[DVD9]");
    while(s < string::npos){
        header->subject.replace(s, 6, "_DVD9_");
        s=header->subject.find("[DVD9]");
    }

    for (sp = yenc_subject_patterns.begin(); sp != yenc_subject_patterns.end(); ++sp){
        if((*sp)->match(header->subject)){
            PostSet *postset = group->postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            postset->has_msg_ids = 1;

            PostFile *postfile = postset->file((*sp)->get_piecen(SP_FILENO), 
                                                     (*sp)->get_piecen(SP_MAXFILENO), 
                                                     (*sp)->get_piece(SP_FILENAME));
            if(!postfile){
                return;
            }
            postfile->decoder_type = DT_YENC;
            postfile->part((*sp)->get_piecen(SP_PARTNO), 
                                   (*sp)->get_piecen(SP_MAXPARTNO), header->message_id);
            return;
        }
    }

    for (sp = uu_subject_patterns.begin(); sp != uu_subject_patterns.end(); ++sp){
        if((*sp)->match(header->subject)){
            PostSet *postset = group->postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            postset->has_msg_ids = 1;
            PostFile *postfile = postset->file((*sp)->get_piecen(SP_FILENO), 
                                             (*sp)->get_piecen(SP_MAXFILENO), 
                                             (*sp)->get_piece(SP_FILENAME));
            if(!postfile){
                return;
            }

            postfile->decoder_type = DT_UUDECODE;
            postfile->part((*sp)->get_piecen(SP_PARTNO), 
                                   (*sp)->get_piecen(SP_MAXPARTNO), header->message_id);
            return;
        }
    }
    return;
}

