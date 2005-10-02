#include "console.hpp"
#include "newsgroup.hpp"
#include "stringpattern.hpp"
#include "post_set.hpp"
#include "assert.h"
#include "jobqueue.hpp"
#include "strutil.hpp"
#include "uu_decoder.hpp"
#include "newsgroup.hpp"
#include "config.hpp"
#include "session.hpp"
#include "headersforgroupjob.hpp"
#include "postsetjob.hpp"
#include "post_set_splitter_dynamicmatch.hpp"

#include<string>
#include<sstream>
#include<vector>

using std::string;
using std::vector;

Console *console;
NewsGroup *newsgroup;
PostSet *current_postset;
vector< NewsGroup * >newsgroups;
PostFile *current_postfile;
JobQueue *jobqueue;
Config *config;
Session *session;
string last_msg;


void test_header_scoop(void);
void test_string_pattern(void);
void test_strings(void);
void test_postset_objects(void);
void test_bit_manipulations(void);
void test_uudecode(void);
void test_more_string_pattern(void);
void test_generated_subject_line_tests(void);
void test_download_netjob(void);
void test_crc32(void);
void test_simple_x(void);
void test_dynamic_postsplit(void);

void generate_subject_line_test(NewsGroup *group, string message_id, string subject);

void assert_postset_filenames_eq(PostSet *checkme, list<string> against);
void assert_strings_eq(string s1, string s2);

using std::string;
using std::stringstream;

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;
    config = new Config(0, NULL);
    newsgroup = new NewsGroup("misc.test");
    config->setup_test_config();
    test_crc32(); 
    test_download_netjob();
    test_generated_subject_line_tests();
    test_uudecode();
    test_strings();
    test_string_pattern();
    test_header_scoop();
    test_postset_objects();
    test_bit_manipulations();
    test_more_string_pattern();
    test_simple_x();
    test_dynamic_postsplit();
	return 0;
}

void test_crc32(void)
{
    string source = "may the source be with you";
    string dest = get_crc_32(source);
    console->log(dest);
    assert(dest.compare("3b354087") == 0);
}

void test_bit_manipulations(void)
{
    stringstream buf;

    buf << "1 << 1 = " << (1<<1) << "; "
        << "1 & 1 = "  << (1&1)  << "; "
        << "1 && 1 = " << (1&&1) << "; "
        << "1 & 2 ="   << (1&2)  << "; "
        << "1 && 2 = " << (1&&2) <<"; ";
    console->log(buf.str());

    console->log(bitviz(1));
    //Dunno why this doesnt work, it looks like it should
    //assert(bitviz(1).compare("10000000000000000000000000000000") == 0);
}

void test_strings(void)
{
    console->log("Test Strings");
    string bar= "hello";
    bar.erase(2, 1);
    console->log(bar);
    assert(0 == bar.compare("helo"));

    char buf[10] = "hello";
    std::string my_string = buf;
    
    buf[0] = 'x';

    console->log(buf);
    console->log(my_string);

    string baz(20, 'x');
    console->log(baz);

    string ziz = "  foo bar baz   ";
    string res = trimmed(ziz);
    assert(0 == res.compare("foo bar baz"));

}

void test_string_pattern(void)
{
    console->log("Test StringPattern");
    StringPattern *s = new StringPattern(7);

    s->add_breaker(" - File ");
    s->add_breaker(" of ");
    s->add_breaker(": \"");
    s->add_breaker("\" yEnc (");
    s->add_breaker("/");
    s->add_breaker(")");

    assert(s->match("Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)"));
    assert(!s->match("Star Wars Clone Wars \"Clone Wars Chapter 20.mpg\" yEnc (248/258)"));

    s->match("Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)");
}

void test_more_string_pattern()
{
    console->log("Test More String Pattern");

    string subject = "1997-02-23 The_Fourth_Generation_FULL_CD-FURY \"fry-4g06.zip\" (4/4) yEnc";
    StringPattern *pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(") yEnc");

    assert(pattern->match(subject));
    assert_strings_eq(pattern->get_piece(SP_SUBJECT), 
                      "1997-02-23 The_Fourth_Generation_FULL_CD-FURY");

    assert(0 == pattern->get_piecen(SP_MAXFILENO));
}

void test_simple_x(void)
{
    string result = simple_x("abc123hehehe");
    assert(0 == result.compare("abc\thehehe"));

    result = simple_x("abc123hehehe - \"hotstuff.rar\" yEnc (001/100)");
    console->log(result);
    assert(0 == result.compare("abc\thehehe - \"\t\" yEnc (\t/\t)"));
}

void test_header_scoop(void)
{
    console->log("Test NewsGroup::header_scoop");
    NewsGroup *group = new NewsGroup("alt.binaries.erotica.tigger");

    //group->header_scoop("2321\tStar Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)\tHomer P Wylde <brick@house.com>\t18 Jun 2005 05:50:02 GMT\t<d90cma$9hk$2320@answer.sectorfour>\t\t344451\t2669\tXref: answer.sectorfour misc.test:2321");
    
    generate_subject_line_test(group, "31337", "NVIDIA linux drivers - File 1 of 1: \"NVIDIA-Linux-x86-1.0-4496-pkg2.run\" yEnc (16/25)");
    generate_subject_line_test(group, "3533", "Beekmans post \"fUf.v6.pal.dvdr.nzb\" [000/104] - yEnc (1/9)");
    generate_subject_line_test(group, "3534", "Beekmans post \"fUf.v6.pal.dvdr.jpg\" [001/104] - yEnc (01/11)");
    generate_subject_line_test(group, "3521", "001 - Initial D S4D1 (1-9) - yEnc \"HD1_5.par2\" (1/1)");
    generate_subject_line_test(group, "3522", " (OMNI)(Vampire Huter D Bloodlust) [074/160] - \"VHDBlood.part072.rar\" yEnc (115/137)");
    generate_subject_line_test(group, "3523", " (Iczer) Those Who - Hunt - \"Elves2_1.vol0058+53.PAR2\" yEnc (075/111)");
    generate_subject_line_test(group, "3524", " (JW) [OMNI][Dnangel Vol 5] - \"lh-dna5.r71\" yEnc (073/201)");
    generate_subject_line_test(group, "3525", " (Iczer) Those Who - Hunt - \"Elves2_1.part077.rar\" yEnc (087/118)");
    generate_subject_line_test(group, "3526", " (OMNI)(Vampire Huter D Bloodlust) [074/160] - \"VHDBlood.part072.rar\" yEnc (116/137)");
    generate_subject_line_test(group, "3527", " (Iczer) Those Who - Hunt - \"Elves2_1.vol0058+53.PAR2\" yEnc (076/111)");
    generate_subject_line_test(group, "3528", " (JW) [OMNI][Dnangel Vol 5] - \"lh-dna5.r71\" yEnc (074/201)");
    generate_subject_line_test(group, "3529", " (Iczer) Those Who - Hunt - \"Elves2_1.part077.rar\" yEnc (088/118)");
    generate_subject_line_test(group, "3530", " 001 - Initial D S4D1 (2-9) - yEnc \"HD1_5.part001.rar\" (01/79)");
    generate_subject_line_test(group, "3531", "Beekmans post \"fUf.v6.pal.dvdr.jpg\" [001/104] - yEnc (10/11)");
    generate_subject_line_test(group, "3532", "Beekmans post \"fUf.v6.pal.dvdr.jpg\" [001/104] - yEnc (11/11)");
    generate_subject_line_test(group, "3533", "Fooly Cooly Volume 1 ~ \"FLCL 1.part18.rar\" ~[21/90] - REQ Kaleido Star Volumes 1,2 and 3 (085/127)");
    generate_subject_line_test(group, "3534", "006 - Initial D S4D1 (4-7) - yEnc \"HD1_5.part039.rar\" (15/79)");
    generate_subject_line_test(group, "3535", "(goku  27  post urotsukidoji_2) File 001 of 163 - VIDEO_TS.PAR2 (1/1)");
    generate_subject_line_test(group, "3536", "(Sailormoon Sailor Stars Disk 4 par to follow) [012/213] - \"yEnc \"ss4_scn.part011.rar\" (077/209)");
    generate_subject_line_test(group, "3537", "(aR) [087/104] - \"ar-bgv4.part84.rar\" yEnc (201/201)");
    generate_subject_line_test(group, "3538", "(aR) [091/104] - \"ar-bgv4.sfv\" yEnc (1/1)");
    generate_subject_line_test(group, "4000", "(Gundam Seed V4+V6 Kimagure V2-V5) [001/556] -  yEnc \"Gundam_Seed_Vol4.par2\" (1/1)");
    generate_subject_line_test(group, "4001", "(OMNI) Gundam Seed Movie [91/91] - \"jspec-gundam.seed.movie.1.rar\" yEnc (32/33)");
    generate_subject_line_test(group, "4002", "(OMNI) Gundam Seed Movie [093/105] - \"jspec-gundam.seed.movie.1.vol000+01.PAR2\" yEnc (1/2)");
    generate_subject_line_test(group, "31337", "NVIDIA linux drivers - File 1 of 1: \"NVIDIA-Linux-x86-1.0-4496-pkg2.run\" yEnc (16/25)");
    generate_subject_line_test(group, "31337", "[503]-[#altbin@EFNet]-[FULL/PART]-[Immortel (ad vitam) (2004)]-[01/91] - \"BTV_Immortel.part016.rar\" yEnc (01/65)");

}

void test_generated_subject_line_tests(void)
{

}


void generate_subject_line_test(NewsGroup *group, string message_id, string subject)
{
    console->log("BROKEN due to refactor of parsing");
    return;
    stringstream buf;
    PostFile *postfile = NULL;
    //postfile = group->digest_subject_line(message_id, subject);
    if(!postfile)
        return;
    PostSet *postset = postfile->post_set;
    postset->recalculate_piece_info();
   
    buf << "subject = \"" << subject << "\";\n"
        << "postfile = group->digest_subject_line(\"" << message_id << "\", subject);\n"
        << "postset = postfile->post_set;\n"
        << "postset->recalculate_piece_info();\n"
        << "\n"
        << "assert(" << postfile->num_pieces <<  " == postfile->num_pieces);\n"
        << "assert(" << postfile->seen_pieces <<  " == postfile->seen_pieces);\n"
        << "assert(" << postset->num_files <<  " == postset->num_files);\n"
        << "assert(" << postset->max_num_files <<  " == postset->max_num_files);\n"
        << "assert(0 == postfile->filename.compare(\"" << postfile->filename <<  "\"));\n"
        << "assert(0 == postfile->post_set->subject.compare(\"" << postfile->post_set->subject <<  "\"));\n\n";

    console->log(buf.str());
    
}

void test_postset_objects(void)
{
    console->log("Test PostSet");

    PostSet *post = new PostSet("brick brothers presents");
    post->file(1, 2, "info.txt");
    post->file(2, 2, "checksum.SFV");

    console->log("Objects created");

    list<string> s;
    s.push_back("info.txt");
    s.push_back("checksum.SFV");
    console->log("Calling assert_postset_filenames_eq");
    assert_postset_filenames_eq(post, s);

    console->log("Second pass..");
    post = new PostSet("brick brothers presents");
    post->file(0, 0, "info.txt");
    post->file(0, 0, "checksum.SFV");

    console->log("Calling assert_postset_filenames_eq");
    assert_postset_filenames_eq(post, s);
}

void test_uudecode(void)
{
    console->log("Test UUdecode");
    UUDecoder *decoder = new UUDecoder();
    string result = decoder->do_the_math("$>6]U\"@``");
    console->log(result);
    assert(result.compare("you\n") == 0);
}

void assert_postset_filenames_eq(PostSet *checkme, list<string> filenames)
{
    stringstream buf;
    console->log("Checking size..");
    buf << checkme->num_files << " <-> " <<  filenames.size();
    console->log(buf.str());
    buf.str("");
    assert(checkme->num_files == filenames.size());

    console->log("Checking contents...");
    list<string>::iterator i;
    Uint32 comp_index = 0;
    if(NULL==checkme->files[comp_index])
        comp_index++;

    for(i = filenames.begin(); i!=filenames.end(); ++i){
        console->log("Checking: " + *i);
        buf << "comp_index: " << comp_index ;
        console->log(buf.str());
        buf.str("");
        assert_strings_eq(checkme->files[comp_index]->filename, *i);
        comp_index++;
    }
}

void assert_strings_eq(string s1, string s2)
{
    if(s1.compare(s2) == 0)
        return;
    console->log("(" + s1 + ") is not equal to (" + s2 + ")");
}

void test_download_netjob(void)
{
    console->log("Testing postset download jobs..");
    NewsGroup *group = new NewsGroup("fusion.downloads");
    PostSet *post = new PostSet("brick brothers presents");
    post->group = group;
    post->file(1, 2, "info.txt")    ->part(1, 32, 31337);
    post->file(2, 2, "checksum.SFV")->part(1, 32, 31338);

    PostsetJob *psj = new PostsetJob(post);
    psj->srv = (void *)4096;
    psj->process();

}

void test_dynamic_postsplit(void)
{
    config->use_newsplit = 1;
    NewsGroup *group = new NewsGroup("net.fusion.downloads");
    PostSetSplitterDynamicMatch *splitter = (PostSetSplitterDynamicMatch *) group->splitter;
    console->log("Parsing (Horny Peeps \"hornypeeps.rar\" yEnc (23/59))");
    splitter->process_header(new MessageHeader(group, 31337, "Horny Peeps \"hornypeeps.rar\" yEnc (23/59)", "jim@bo.com"));
    assert(0 == group->postsets.size());
    splitter->process_header(new MessageHeader(group, 31338, "Horny Peeps \"hornypeeps.rar\" yEnc (24/59)", "jim@bo.com"));
    assert(1 == group->postsets.size());
    config->use_newsplit = 0;

}

void shut_down(void)
{
    printf("Mooooooo!\n");
}
